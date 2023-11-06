/*
 * Rad Pro
 * STM32 USART communications
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include "../comm.h"
#include "../system.h"

#include "stm32.h"

#if defined(FS2011) && defined(STM32F0) && defined(GD32)

const char *const commId = "FS2011 (GD32F150);" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(FS2011) && defined(STM32F0)

const char *const commId = "FS2011 (STM32F051);" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(FS2011) && defined(STM32F1)

const char *const commId = "FS2011 (GD32F103);" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(FS600)

const char *const commId = "Bosean FS-600;" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(FS1000)

const char *const commId = "Bosean FS-1000;" FIRMWARE_NAME " " FIRMWARE_VERSION;

#elif defined(GC01)

const char *const commId = "FNIRSI GC-01;" FIRMWARE_NAME " " FIRMWARE_VERSION;

#endif

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

    gpio_mode_setup(USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_RX_PIN);
    gpio_set_af(USART_RX_PORT, USART_RX_AF, USART_RX_PIN);

    gpio_mode_setup(USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_TX_PIN);
    gpio_set_af(USART_TX_PORT, USART_TX_AF, USART_TX_PIN);

#elif defined(STM32F1)

    gpio_set_mode(USART_RX_PORT, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT, USART_RX_PIN);
    gpio_set_mode(USART_TX_PORT, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, USART_TX_PIN);

#endif

    // USART

    rcc_periph_clock_enable(USART_RCC);

    usart_set_baudrate(USART_INTERFACE, COMM_BAUDRATE);
    // usart_set_databits(USART_INTERFACE, 8);
    // usart_set_parity(USART_INTERFACE, USART_PARITY_NONE);
    // usart_set_stopbits(USART_INTERFACE, USART_STOPBITS_1);
    // usart_set_flow_control(USART_INTERFACE, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART_INTERFACE, USART_MODE_TX_RX);

    usart_enable_rx_interrupt(USART_INTERFACE);

    nvic_set_priority(USART_IRQ, 0x7f);
    nvic_enable_irq(USART_IRQ);

    usart_enable(USART_INTERFACE);
}

void transmitComm(void)
{
    comm.state = COMM_TX;

    if (comm.port == COMM_UART)
        usart_enable_tx_interrupt(USART_INTERFACE);
}

void USART_IRQ_HANDLER(void)
{
    if (usart_recv_ready(USART_INTERFACE))
    {
        char c = usart_recv(USART_INTERFACE);

        comm.port = COMM_UART;

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

                comm.bufferIndex = '\0';

                comm.state = COMM_TX_READY;
            }
        }
    }
}

#endif
