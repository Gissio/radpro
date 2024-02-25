/*
 * Rad Pro
 * STM32 USART/USB communications
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <string.h>

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "device.h"

#if defined(USART_INTERFACE)

#include <libopencm3/stm32/usart.h>

#elif defined(USB_INTERFACE)

#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/usbd.h>

#endif

#include "../comm.h"
#include "../cstring.h"
#include "../events.h"
#include "../system.h"

#if defined(USART_INTERFACE)

#if defined(STM32F0) || defined(STM32G0)

static bool usart_recv_ready(uint32_t usart)
{
    return USART_ISR(usart) & USART_ISR_RXNE;
}

static bool usart_send_ready(uint32_t usart)
{
    return USART_ISR(usart) & USART_ISR_TXE;
}

#elif defined(STM32F1)

static bool usart_recv_ready(uint32_t usart)
{
    return (USART_SR(usart) & USART_SR_RXNE);
}

static bool usart_send_ready(uint32_t usart)
{
    return USART_SR(usart) & USART_SR_TXE;
}

#endif

void initComm(void)
{
    // GPIO

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(USART_RX_PORT,
                    GPIO_MODE_AF,
                    GPIO_PUPD_NONE,
                    USART_RX_PIN);
    gpio_set_af(USART_RX_PORT,
                USART_RX_AF,
                USART_RX_PIN);

    gpio_mode_setup(USART_TX_PORT,
                    GPIO_MODE_AF,
                    GPIO_PUPD_NONE,
                    USART_TX_PIN);
    gpio_set_af(USART_TX_PORT,
                USART_TX_AF,
                USART_TX_PIN);

#elif defined(STM32F1)

    gpio_set_mode(USART_RX_PORT,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT,
                  USART_RX_PIN);
    gpio_set_mode(USART_TX_PORT,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  USART_TX_PIN);

#endif

    // USART

    rcc_periph_clock_enable(USART_RCC);

    // usart_set_baudrate(USART_INTERFACE, COMM_BAUDRATE);
    USART_BRR(USART_INTERFACE) =
        (APB1_FREQUENCY + COMM_BAUDRATE / 2) /
        COMM_BAUDRATE;

    // usart_set_databits(USART_INTERFACE, 8);
    // usart_set_parity(USART_INTERFACE, USART_PARITY_NONE);
    // usart_set_stopbits(USART_INTERFACE, USART_STOPBITS_1);
    // usart_set_flow_control(USART_INTERFACE, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART_INTERFACE, USART_MODE_TX_RX);

    usart_enable_rx_interrupt(USART_INTERFACE);

    nvic_set_priority(USART_IRQ, 0x80);
    nvic_enable_irq(USART_IRQ);

    usart_enable(USART_INTERFACE);
}

void transmitComm(void)
{
    comm.state = COMM_TX;

    if (comm.port == COMM_SERIAL)
        usart_enable_tx_interrupt(USART_INTERFACE);
}

void USART_IRQ_HANDLER(void)
{
    if (usart_recv_ready(USART_INTERFACE))
    {
        char c = usart_recv(USART_INTERFACE);

        if (!comm.enabled)
            return;

        comm.port = COMM_SERIAL;

        if (comm.state == COMM_RX)
        {
            if ((c >= ' ') && (comm.bufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.buffer[comm.bufferIndex++] = c;
            else if (c == '\n')
            {
                comm.buffer[comm.bufferIndex] = '\0';
                comm.bufferIndex = 0;

                comm.state = COMM_RX_READY;
            }
        }
    }

    if (usart_send_ready(USART_INTERFACE))
    {
        if (comm.state == COMM_TX)
        {
            if (comm.buffer[comm.bufferIndex] != '\0')
                usart_send(USART_INTERFACE, comm.buffer[comm.bufferIndex++]);
            else
            {
                usart_disable_tx_interrupt(USART_INTERFACE);

                comm.bufferIndex = 0;

                comm.state = COMM_TX_READY;
            }
        }
    }
}

void updateCommHardware(void)
{
}

#elif defined(USB_INTERFACE)

#define USB_CONTROL_PACKET_SIZE_MAX 16
#define USB_DATA_PACKET_SIZE_MAX 64

#define USB_DATA_ENDPOINT_IN 0x01
#define USB_DATA_ENDPOINT_OUT 0x82
#define USB_CONTROL_ENDPOINT_OUT 0x83

#define USB_DEVICE_RELEASE 0x0100

enum
{
    USB_CONTROL_INTERFACE_INDEX,
    USB_DATA_INTERFACE_INDEX,
};

enum
{
    USB_CONFIGURATION_INDEX = 1,
};

enum
{
    USB_MANUFACTURER_STRING = 1,
    USB_PRODUCT_STRING,
    USB_SERIALNUMBER_STRING,
};

static char usbSerialNumber[13];

static const char *const usbStrings[] = {
    FIRMWARE_AUTHOR,
    FIRMWARE_NAME,
    usbSerialNumber,
};

static usbd_device *usbdDevice = NULL;
static uint8_t usbControlBuffer[128];

// Control interface

static const struct usb_endpoint_descriptor usbControlEndpoints[] = {
    {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = USB_CONTROL_ENDPOINT_OUT,
        .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
        .wMaxPacketSize = USB_CONTROL_PACKET_SIZE_MAX,
        .bInterval = 16, // Polling interval (2 ^ bInterval - 1 ms)
    },
};

static const struct
{
    struct usb_cdc_header_descriptor header;
    struct usb_cdc_call_management_descriptor call_mgmt;
    struct usb_cdc_acm_descriptor acm;
    struct usb_cdc_union_descriptor cdc_union;
} __attribute__((packed)) usbControlFunctionalDescriptors = {
    .header = {
        .bFunctionLength = sizeof(struct usb_cdc_header_descriptor),
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_HEADER,
        .bcdCDC = 0x0110, // CDC specification 1.10
    },
    .call_mgmt = {
        .bFunctionLength = sizeof(struct usb_cdc_call_management_descriptor),
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,
        .bmCapabilities = 0, // No call management commands supported
        .bDataInterface = USB_DATA_INTERFACE_INDEX,
    },
    .acm = {
        .bFunctionLength = sizeof(struct usb_cdc_acm_descriptor), //
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_ACM,
        .bmCapabilities = 0, // No ACM commands supported
    },
    .cdc_union = {
        .bFunctionLength = sizeof(struct usb_cdc_union_descriptor),
        .bDescriptorType = CS_INTERFACE,
        .bDescriptorSubtype = USB_CDC_TYPE_UNION,
        .bControlInterface = USB_CONTROL_INTERFACE_INDEX,
        .bSubordinateInterface0 = USB_DATA_INTERFACE_INDEX,
    },
};

static const struct usb_interface_descriptor usbControlInterfaces[] = {
    {
        .bLength = USB_DT_INTERFACE_SIZE,
        .bDescriptorType = USB_DT_INTERFACE,
        .bInterfaceNumber = USB_CONTROL_INTERFACE_INDEX,
        .bAlternateSetting = 0,
        .bNumEndpoints = sizeof(usbControlEndpoints) /
                         sizeof(usbControlEndpoints[0]),
        .bInterfaceClass = USB_CLASS_CDC,
        .bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
        .bInterfaceProtocol = USB_CDC_PROTOCOL_NONE,
        .iInterface = 0, // Interface string descriptor: none
        .endpoint = usbControlEndpoints,
        .extra = &usbControlFunctionalDescriptors,
        .extralen = sizeof(usbControlFunctionalDescriptors),
    },
};

// Data interface

static const struct usb_endpoint_descriptor usbDataEndpoints[] = {
    {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = USB_DATA_ENDPOINT_IN,
        .bmAttributes = USB_ENDPOINT_ATTR_BULK,
        .wMaxPacketSize = USB_DATA_PACKET_SIZE_MAX,
        .bInterval = 0, // Polling interval (2 ^ bInterval - 1 ms)
    },
    {
        .bLength = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType = USB_DT_ENDPOINT,
        .bEndpointAddress = USB_DATA_ENDPOINT_OUT,
        .bmAttributes = USB_ENDPOINT_ATTR_BULK,
        .wMaxPacketSize = USB_DATA_PACKET_SIZE_MAX,
        .bInterval = 0, // Polling interval (2 ^ bInterval - 1 ms)
    },
};

static const struct usb_interface_descriptor usbDataInterfaces[] = {
    {
        .bLength = USB_DT_INTERFACE_SIZE,
        .bDescriptorType = USB_DT_INTERFACE,
        .bInterfaceNumber = USB_DATA_INTERFACE_INDEX,
        .bAlternateSetting = 0,
        .bNumEndpoints = sizeof(usbDataEndpoints) /
                         sizeof(usbDataEndpoints[0]),
        .bInterfaceClass = USB_CLASS_DATA,
        .bInterfaceSubClass = 0, // Unused with data interface
        .bInterfaceProtocol = 0, // Unused with data interface
        .iInterface = 0,         // Interface string descriptor: none
        .endpoint = usbDataEndpoints,
    },
};

// Interfaces

static const struct usb_interface usbInterfaces[] = {
    {
        .num_altsetting = sizeof(usbControlInterfaces) /
                          sizeof(usbControlInterfaces[0]),
        .altsetting = usbControlInterfaces,
    },
    {
        .num_altsetting = sizeof(usbDataInterfaces) /
                          sizeof(usbDataInterfaces[0]),
        .altsetting = usbDataInterfaces,
    },
};

// Configuration descriptor

static const struct usb_config_descriptor usbConfigurationDescriptor[] = {
    {
        .bLength = USB_DT_CONFIGURATION_SIZE,
        .bDescriptorType = USB_DT_CONFIGURATION,
        .wTotalLength = sizeof(struct usb_config_descriptor) +
                        sizeof(usbControlInterfaces) +
                        sizeof(usbControlEndpoints) +
                        sizeof(usbControlFunctionalDescriptors) +
                        sizeof(usbDataInterfaces) +
                        sizeof(usbDataEndpoints),
        .bNumInterfaces = sizeof(usbInterfaces) /
                          sizeof(usbInterfaces[0]),
        .bConfigurationValue = USB_CONFIGURATION_INDEX,
        .iConfiguration = 0, // Interface string descriptor: none
        .bmAttributes = USB_CONFIG_ATTR_DEFAULT,
        .bMaxPower = 50, // Maximum power consumption in 2 mA units [100 mA]
        .interface = usbInterfaces,
    },
};

// Device descriptor

static const struct usb_device_descriptor usbDeviceDescriptor = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x0200, // USB Version (2.00)
    .bDeviceClass = USB_CLASS_CDC,
    .bDeviceSubClass = 0, // Unused with communications device class
    .bDeviceProtocol = 0, // Unused with communications device class
    .bMaxPacketSize0 = 64,
    .idVendor = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = USB_DEVICE_RELEASE,
    .iManufacturer = USB_MANUFACTURER_STRING,
    .iProduct = USB_PRODUCT_STRING,
    .iSerialNumber = USB_SERIALNUMBER_STRING,
    .bNumConfigurations = sizeof(usbConfigurationDescriptor) /
                          sizeof(usbConfigurationDescriptor[0]),
};

static void onUSBDataReceived(usbd_device *usbd_dev,
                              uint8_t endpoint)
{
    char buffer[USB_DATA_PACKET_SIZE_MAX];
    uint32_t bufferSize = usbd_ep_read_packet(usbdDevice,
                                              USB_DATA_ENDPOINT_IN,
                                              buffer,
                                              USB_DATA_PACKET_SIZE_MAX);
    uint32_t bufferIndex = 0;

    if (!comm.enabled)
        return;

    if (comm.state == COMM_RX)
    {
        while (bufferIndex < bufferSize)
        {
            char c = buffer[bufferIndex++];

            if ((c >= ' ') &&
                (comm.bufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.buffer[comm.bufferIndex++] = c;
            else if (c == '\n')
            {
                comm.buffer[comm.bufferIndex] = '\0';
                comm.bufferIndex = 0;

                comm.state = COMM_RX_READY;
            }
        }
    }
}

static void onUSBStartOfFrame(void)
{
    if (comm.state == COMM_TX)
    {
        uint32_t bufferSize = usbd_ep_write_packet(usbdDevice,
                                                   USB_DATA_ENDPOINT_OUT,
                                                   comm.buffer,
                                                   strlen(comm.buffer));

        if (bufferSize)
        {
            comm.bufferIndex = 0;

            comm.state = COMM_TX_READY;
        }
    }
}

static enum usbd_request_return_codes
onUSBControl(usbd_device *usbd_dev,
             struct usb_setup_data *request,
             uint8_t **buffer,
             uint16_t *bufferSize,
             void (**callback)(usbd_device *usbd_dev,
                               struct usb_setup_data *request))
{
    switch (request->bRequest)
    {
    case USB_CDC_REQ_SET_CONTROL_LINE_STATE:
        return USBD_REQ_HANDLED;

    case USB_CDC_REQ_SET_LINE_CODING:
        if (*bufferSize < sizeof(struct usb_cdc_line_coding))
            return USBD_REQ_NOTSUPP;

        return USBD_REQ_HANDLED;
    }

    return USBD_REQ_NOTSUPP;
}

static void onUSBSetConfiguration(usbd_device *usbd_dev,
                                  uint16_t wValue)
{
    usbd_ep_setup(usbd_dev,
                  USB_DATA_ENDPOINT_IN,
                  USB_ENDPOINT_ATTR_BULK,
                  USB_DATA_PACKET_SIZE_MAX,
                  onUSBDataReceived);

    usbd_ep_setup(usbd_dev,
                  USB_DATA_ENDPOINT_OUT,
                  USB_ENDPOINT_ATTR_BULK,
                  USB_DATA_PACKET_SIZE_MAX,
                  NULL);

    usbd_ep_setup(usbd_dev,
                  USB_CONTROL_ENDPOINT_OUT,
                  USB_ENDPOINT_ATTR_INTERRUPT,
                  USB_CONTROL_PACKET_SIZE_MAX,
                  NULL);

    usbd_register_control_callback(usbdDevice,
                                   USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
                                   USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
                                   onUSBControl);
}

void initComm(void)
{
    // Force USB device reenumeration

    gpio_set_mode(USB_DP_PORT, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, USB_DP_PIN);
    gpio_clear(USB_DP_PORT, USB_DP_PIN);

    sleep(50);

    // Init USB

    strcpy(usbSerialNumber, "0000");
    strcatUInt32Hex(usbSerialNumber, getDeviceId());

    usbdDevice = usbd_init(&st_usbfs_v1_usb_driver,
                           &usbDeviceDescriptor,
                           usbConfigurationDescriptor,
                           usbStrings,
                           sizeof(usbStrings) / sizeof(usbStrings[0]),
                           usbControlBuffer,
                           sizeof(usbControlBuffer));

    usbd_register_set_config_callback(usbdDevice,
                                      onUSBSetConfiguration);
    usbd_register_sof_callback(usbdDevice,
                               onUSBStartOfFrame);

    nvic_set_priority(NVIC_USB_LP_CAN_RX0_IRQ, 0x80);
    nvic_enable_irq(NVIC_USB_LP_CAN_RX0_IRQ);
}

void transmitComm(void)
{
    comm.state = COMM_TX;
}

void usb_lp_can_rx0_isr()
{
    usbd_poll(usbdDevice);
}

void updateCommHardware(void)
{
}

#else

void initComm(void)
{
}

void transmitComm(void)
{
    comm.state = COMM_TX_READY;
}

void updateCommHardware(void)
{
}

#endif

#endif
