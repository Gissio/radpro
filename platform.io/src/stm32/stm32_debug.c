/*
 * Rad Pro
 * STM32 debugging
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "device.h"

#include "../debug.h"
#include "../events.h"
#include "../power.h"

#define DEBUG_USART_INTERFACE USART1
#define DEBUG_USART_APB_FREQUENCY APB2_FREQUENCY
#define DEBUG_USART_TX_PORT GPIOB
#define DEBUG_USART_TX_PIN 6
#define DEBUG_USART_TX_AF 0
#define DEBUG_USART_BAUDRATE 115200

void initDebug(void)
{
#if defined(STM32F0) || defined(STM32G0)
    gpio_setup_af(DEBUG_USART_TX_PORT,
                  DEBUG_USART_TX_PIN,
                  GPIO_OUTPUTTYPE_PUSHPULL,
                  GPIO_OUTPUTSPEED_50MHZ,
                  GPIO_PULL_FLOATING,
                  DEBUG_USART_TX_AF);
#elif defined(STM32F1)
    set_bits(AFIO->MAPR,
             AFIO_MAPR_USART1_REMAP);

    gpio_setup(DEBUG_USART_TX_PORT,
               DEBUG_USART_TX_PIN,
               GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);
#endif

    usart_setup_8n1(DEBUG_USART_INTERFACE,
                    (DEBUG_USART_APB_FREQUENCY + DEBUG_USART_BAUDRATE / 2) / DEBUG_USART_BAUDRATE);
}

void printDebug(char *s)
{
    while (true)
    {
        char c = *s;
        if (c == '\0')
            return;

        usart_send_blocking(DEBUG_USART_INTERFACE, c);
        if (c == '\n')
            usart_send_blocking(DEBUG_USART_INTERFACE, '\r');

        s++;
    }
}

uint32_t getGPIO(uint32_t index)
{
    switch (index)
    {
    case 0:
        return GPIOA->IDR;

    case 1:
        return GPIOB->IDR;

    case 2:
        return GPIOC->IDR;

    case 3:
        return GPIOD->IDR;

#if defined(GPIOE)
    case 4:
        return GPIOE->IDR;
#endif

#if defined(GPIOF)
    case 5:
        return GPIOF->IDR;
#endif

    default:
        return 0;
    }
}

void debugWait(uint32_t ms)
{
    uint32_t n = (SYSCLK_FREQUENCY / 8000000) * 200 * ms;

    for (uint32_t i = 0; i < n; i++)
        sleep(0);
}

void onHardFault(uint32_t *args)
{
    // args: R0, R1, R2, R3, R12, LR, PC, PSR

    // Send R12, LR, PC, PSR
    for (uint32_t i = 4; i < 8; i++)
        debugUInt32(args[i]);

    setPower(false);

    while (true)
        sleep(0);
}

void HardFault_Handler(void)
{
    __asm volatile(
        " movs r0, #4           \n"
        " mov r1, lr            \n"
        " tst r0, r1            \n"
        " bne HARDFAULT_PSP     \n"
        " mrs r0, msp           \n"
        " b HARDFAULT_CALL      \n"
        "HARDFAULT_PSP:         \n"
        " mrs r0, psp           \n"
        "HARDFAULT_CALL:        \n"
        " ldr r2,=onHardFault   \n"
        " bx r2                 \n");
}

#endif
