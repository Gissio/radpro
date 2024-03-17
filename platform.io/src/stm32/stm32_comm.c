/*
 * Rad Pro
 * STM32 communications
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <string.h>

#include "device.h"

#include "../comm.h"
#include "../cstring.h"
#include "../events.h"
#include "../system.h"

#if defined(USART_INTERFACE)

#define COMM_SERIAL_BAUDRATE 115200

void initComm(void)
{
#if defined(STM32F0) || defined(STM32G0)

    gpio_setup_af(USART_RX_PORT,
                  USART_RX_PIN,
                  GPIO_OUTPUTTYPE_PUSHPULL,
                  GPIO_OUTPUTSPEED_50MHZ,
                  GPIO_PULL_FLOATING,
                  USART_RX_AF);

    gpio_setup_af(USART_TX_PORT,
                  USART_TX_PIN,
                  GPIO_OUTPUTTYPE_PUSHPULL,
                  GPIO_OUTPUTSPEED_50MHZ,
                  GPIO_PULL_FLOATING,
                  USART_TX_AF);

#elif defined(STM32F1)

    gpio_setup(USART_RX_PORT,
               USART_RX_PIN,
               GPIO_MODE_INPUT_FLOATING);
    gpio_setup(USART_TX_PORT,
               USART_TX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);

#endif

    usart_setup_8n1(USART_INTERFACE,
                    (USART_APB_FREQUENCY + COMM_SERIAL_BAUDRATE / 2) / COMM_SERIAL_BAUDRATE);
    usart_enable_receive_interrupt(USART_INTERFACE);

    NVIC_SetPriority(USART_IRQ, 0x80);
    NVIC_EnableIRQ(USART_IRQ);
}

void transmitComm(void)
{
    comm.bufferIndex = 0;
    comm.state = COMM_TX;

    if (comm.port == COMM_SERIAL)
        usart_enable_transmit_interrupt(USART_INTERFACE);
}

void USART_IRQ_HANDLER(void)
{
    if (usart_is_receive_ready(USART_INTERFACE))
    {
        char c = usart_receive(USART_INTERFACE);

        if (!comm.enabled)
            return;

        comm.port = COMM_SERIAL;

        if (comm.state == COMM_RX)
        {
            if ((c >= ' ') &&
                (comm.bufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.buffer[comm.bufferIndex++] = c;
            else if (c == '\n')
            {
                comm.buffer[comm.bufferIndex] = '\0';
                comm.state = COMM_RX_READY;
            }
        }
    }

    if (usart_is_send_ready(USART_INTERFACE))
    {
        if (comm.state == COMM_TX)
        {
            if (comm.buffer[comm.bufferIndex] != '\0')
                usart_send(USART_INTERFACE,
                           comm.buffer[comm.bufferIndex++]);
            else
            {
                usart_disable_transmit_interrupt(USART_INTERFACE);

                comm.state = COMM_TX_READY;
            }
        }
    }
}

void updateCommController(void)
{
}

#elif defined(USB_INTERFACE)

#include "usb.h"
#include "usb_cdc.h"

#define USB_VID 0x0483
#define USB_PID 0x5740

#define USB_DATA_RECEIVE_ENDPOINT 0x01
#define USB_DATA_TRANSMIT_ENDPOINT 0x81
#define USB_CONTROL_ENDPOINT 0x82

#define USB_EP0_PACKETSIZE_MAX 0x08
#define USB_DATA_PACKETSIZE_MAX 0x40
#define USB_CONTROL_PACKETSIZE_MAX 0x08

// Declaration of the report descriptor

struct cdc_config
{
    struct usb_config_descriptor config;
    struct usb_iad_descriptor comm_iad;
    struct usb_interface_descriptor comm;
    struct usb_cdc_header_desc cdc_hdr;
    struct usb_cdc_call_mgmt_desc cdc_mgmt;
    struct usb_cdc_acm_desc cdc_acm;
    struct usb_cdc_union_desc cdc_union;
    struct usb_endpoint_descriptor comm_ep;
    struct usb_interface_descriptor data;
    struct usb_endpoint_descriptor data_eprx;
    struct usb_endpoint_descriptor data_eptx;
} __attribute__((packed));

// Device descriptor

static const struct usb_device_descriptor device_desc = {
    .bLength = sizeof(struct usb_device_descriptor),
    .bDescriptorType = USB_DTYPE_DEVICE,
    .bcdUSB = VERSION_BCD(2, 0, 0),
    .bDeviceClass = USB_CLASS_IAD,
    .bDeviceSubClass = USB_SUBCLASS_IAD,
    .bDeviceProtocol = USB_PROTO_IAD,
    .bMaxPacketSize0 = USB_EP0_PACKETSIZE_MAX,
    .idVendor = USB_VID,
    .idProduct = USB_PID,
    .bcdDevice = VERSION_BCD(1, 0, 0),
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = INTSERIALNO_DESCRIPTOR,
    .bNumConfigurations = 1,
};

// Device configuration descriptor

static const struct cdc_config config_desc = {
    .config = {
        .bLength = sizeof(struct usb_config_descriptor),
        .bDescriptorType = USB_DTYPE_CONFIGURATION,
        .wTotalLength = sizeof(struct cdc_config),
        .bNumInterfaces = 2,
        .bConfigurationValue = 1,
        .iConfiguration = NO_DESCRIPTOR,
        .bmAttributes = USB_CFG_ATTR_RESERVED | USB_CFG_ATTR_SELFPOWERED,
        .bMaxPower = USB_CFG_POWER_MA(500),
    },
    .comm_iad = {
        .bLength = sizeof(struct usb_iad_descriptor),
        .bDescriptorType = USB_DTYPE_INTERFASEASSOC,
        .bFirstInterface = 0,
        .bInterfaceCount = 2,
        .bFunctionClass = USB_CLASS_CDC,
        .bFunctionSubClass = USB_CDC_SUBCLASS_ACM,
        .bFunctionProtocol = USB_PROTO_NONE,
        .iFunction = NO_DESCRIPTOR,
    },
    .comm = {
        .bLength = sizeof(struct usb_interface_descriptor),
        .bDescriptorType = USB_DTYPE_INTERFACE,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 1,
        .bInterfaceClass = USB_CLASS_CDC,
        .bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,
        .bInterfaceProtocol = USB_PROTO_NONE,
        .iInterface = NO_DESCRIPTOR,
    },
    .cdc_hdr = {
        .bFunctionLength = sizeof(struct usb_cdc_header_desc),
        .bDescriptorType = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType = USB_DTYPE_CDC_HEADER,
        .bcdCDC = VERSION_BCD(1, 1, 0),
    },
    .cdc_mgmt = {
        .bFunctionLength = sizeof(struct usb_cdc_call_mgmt_desc),
        .bDescriptorType = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType = USB_DTYPE_CDC_CALL_MANAGEMENT,
        .bmCapabilities = 0,
        .bDataInterface = 1,

    },
    .cdc_acm = {
        .bFunctionLength = sizeof(struct usb_cdc_acm_desc),
        .bDescriptorType = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType = USB_DTYPE_CDC_ACM,
        .bmCapabilities = 0,
    },
    .cdc_union = {
        .bFunctionLength = sizeof(struct usb_cdc_union_desc),
        .bDescriptorType = USB_DTYPE_CS_INTERFACE,
        .bDescriptorSubType = USB_DTYPE_CDC_UNION,
        .bMasterInterface0 = 0,
        .bSlaveInterface0 = 1,
    },
    .comm_ep = {
        .bLength = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType = USB_DTYPE_ENDPOINT,
        .bEndpointAddress = USB_CONTROL_ENDPOINT,
        .bmAttributes = USB_EPTYPE_INTERRUPT,
        .wMaxPacketSize = USB_CONTROL_PACKETSIZE_MAX,
        .bInterval = 0xFF,
    },
    .data = {
        .bLength = sizeof(struct usb_interface_descriptor),
        .bDescriptorType = USB_DTYPE_INTERFACE,
        .bInterfaceNumber = 1,
        .bAlternateSetting = 0,
        .bNumEndpoints = 2,
        .bInterfaceClass = USB_CLASS_CDC_DATA,
        .bInterfaceSubClass = USB_SUBCLASS_NONE,
        .bInterfaceProtocol = USB_PROTO_NONE,
        .iInterface = NO_DESCRIPTOR,
    },
    .data_eprx = {
        .bLength = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType = USB_DTYPE_ENDPOINT,
        .bEndpointAddress = USB_DATA_RECEIVE_ENDPOINT,
        .bmAttributes = USB_EPTYPE_BULK,
        .wMaxPacketSize = USB_DATA_PACKETSIZE_MAX,
        .bInterval = 0x01,
    },
    .data_eptx = {
        .bLength = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType = USB_DTYPE_ENDPOINT,
        .bEndpointAddress = USB_DATA_TRANSMIT_ENDPOINT,
        .bmAttributes = USB_EPTYPE_BULK,
        .wMaxPacketSize = USB_DATA_PACKETSIZE_MAX,
        .bInterval = 0x01,
    },
};

static const struct usb_string_descriptor lang_desc = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC(FIRMWARE_AUTHOR);
static const struct usb_string_descriptor prod_desc_en = USB_STRING_DESC(FIRMWARE_NAME);

static const struct usb_string_descriptor *const dtable[] = {
    &lang_desc,
    &manuf_desc_en,
    &prod_desc_en,
};

usbd_device usbdDevice;
uint32_t usbdBuffer[0x20];

static struct usb_cdc_line_coding cdc_line = {
    .dwDTERate = 115200,
    .bCharFormat = USB_CDC_1_STOP_BITS,
    .bParityType = USB_CDC_NO_PARITY,
    .bDataBits = 8,
};

static usbd_respond onUSBGetDescription(usbd_ctlreq *req, void **address, uint16_t *length)
{
    const uint8_t dtype = req->wValue >> 8;
    const uint8_t dnumber = req->wValue & 0xFF;
    const void *desc;
    uint16_t len = 0;

    switch (dtype)
    {
    case USB_DTYPE_DEVICE:
        desc = &device_desc;

        break;

    case USB_DTYPE_CONFIGURATION:
        desc = &config_desc;
        len = sizeof(config_desc);

        break;

    case USB_DTYPE_STRING:
        if (dnumber < 3)
            desc = dtable[dnumber];
        else
            return usbd_fail;

        break;

    default:
        return usbd_fail;
    }

    if (len == 0)
        len = ((struct usb_header_descriptor *)desc)->bLength;

    *address = (void *)desc;
    *length = len;

    return usbd_ack;
}

static usbd_respond onUSBControl(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
    if (((USB_REQ_RECIPIENT | USB_REQ_TYPE) & req->bmRequestType) ==
            (USB_REQ_INTERFACE | USB_REQ_CLASS) &&
        req->wIndex == 0)
    {
        switch (req->bRequest)
        {
        case USB_CDC_SET_CONTROL_LINE_STATE:
            return usbd_ack;

        case USB_CDC_SET_LINE_CODING:
            memcpy(&cdc_line, req->data, sizeof(cdc_line));

            return usbd_ack;

        case USB_CDC_GET_LINE_CODING:
            dev->status.data_ptr = &cdc_line;
            dev->status.data_count = sizeof(cdc_line);

            return usbd_ack;

        default:
            return usbd_fail;
        }
    }

    return usbd_fail;
}

static void onUsbData(usbd_device *dev, uint8_t event, uint8_t ep)
{
    char receiveBuffer[USB_DATA_PACKETSIZE_MAX];
    int32_t receivedBytes;

    if (event == usbd_evt_eprx)
        receivedBytes = usbd_ep_read(dev,
                                     USB_DATA_RECEIVE_ENDPOINT,
                                     receiveBuffer,
                                     USB_DATA_PACKETSIZE_MAX);
    else
        receivedBytes = 0;

    if (!comm.enabled)
        return;

    if (comm.state == COMM_RX)
    {
        for (int32_t i = 0;
             i < receivedBytes;
             i++)
        {
            char c = receiveBuffer[i];

            if ((c >= ' ') &&
                (comm.bufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.buffer[comm.bufferIndex++] = c;
            else if (c == '\n')
            {
                comm.buffer[comm.bufferIndex] = '\0';
                comm.state = COMM_RX_READY;
            }
        }
    }

    if ((comm.state == COMM_RX_READY) ||
        (comm.state == COMM_TX_READY))
    {
        usbd_ep_write(dev,
                      USB_DATA_TRANSMIT_ENDPOINT,
                      NULL,
                      0);
    }
    else if (comm.state == COMM_TX)
    {
        char *sendBuffer = comm.buffer + comm.bufferIndex;
        int32_t sentBytes = usbd_ep_write(dev,
                                          USB_DATA_TRANSMIT_ENDPOINT,
                                          sendBuffer,
                                          strlen(sendBuffer));

        if (sentBytes >= 0)
        {
            comm.bufferIndex += sentBytes;

            if (comm.buffer[comm.bufferIndex] == '\0')
                comm.state = COMM_TX_READY;
        }
    }
}

static usbd_respond onUSBConfigure(usbd_device *dev, uint8_t cfg)
{
    switch (cfg)
    {
    case 0:
        // Deconfigure device

        usbd_ep_deconfig(dev, USB_CONTROL_ENDPOINT);
        usbd_ep_deconfig(dev, USB_DATA_TRANSMIT_ENDPOINT);
        usbd_ep_deconfig(dev, USB_DATA_RECEIVE_ENDPOINT);

        usbd_reg_endpoint(dev, USB_DATA_RECEIVE_ENDPOINT, NULL);
        usbd_reg_endpoint(dev, USB_DATA_TRANSMIT_ENDPOINT, NULL);

        return usbd_ack;

    case 1:
        // Configure device

        usbd_ep_config(dev, USB_DATA_RECEIVE_ENDPOINT, USB_EPTYPE_BULK, USB_DATA_PACKETSIZE_MAX);
        usbd_ep_config(dev, USB_DATA_TRANSMIT_ENDPOINT, USB_EPTYPE_BULK, USB_DATA_PACKETSIZE_MAX);
        usbd_ep_config(dev, USB_CONTROL_ENDPOINT, USB_EPTYPE_INTERRUPT, USB_CONTROL_PACKETSIZE_MAX);

        usbd_reg_endpoint(dev, USB_DATA_RECEIVE_ENDPOINT, onUsbData);
        usbd_reg_endpoint(dev, USB_DATA_TRANSMIT_ENDPOINT, onUsbData);

        return usbd_ack;

    default:
        return usbd_fail;
    }
}

void transmitComm(void)
{
    comm.bufferIndex = 0;
    comm.state = COMM_TX;
}

void USB_IRQ_HANDLER(void)
{
    usbd_poll(&usbdDevice);
}

void initComm(void)
{
    // Force USB device reenumeration
    gpio_setup(USB_DP_PORT,
               USB_DP_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
    gpio_clear(USB_DP_PORT, USB_DP_PIN);
    sleep(50);

    usbd_init(&usbdDevice,
              &usbd_hw,
              USB_EP0_PACKETSIZE_MAX,
              usbdBuffer,
              sizeof(usbdBuffer));
    usbd_reg_config(&usbdDevice, onUSBConfigure);
    usbd_reg_control(&usbdDevice, onUSBControl);
    usbd_reg_descr(&usbdDevice, onUSBGetDescription);

    NVIC_SetPriority(USB_IRQ, 0x80);
    NVIC_EnableIRQ(USB_IRQ);

    usbd_enable(&usbdDevice, true);
    usbd_connect(&usbdDevice, true);
}

void updateCommController(void)
{
}

#else

void initComm(void)
{
}

void updateCommController(void)
{
}

#endif

#endif
