/*
 * Rad Pro
 * STM32 system
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../buzzer.h"
#include "../display.h"
#include "../events.h"
#include "../system.h"
#include "../tube.h"

#include "stm32.h"

typedef struct
{
    uint32_t initialSP;
    void (*resetHandler)(void);
} STM32VectorTable;

#if defined(STM32F0)

#define BOOT_VECTOR_TABLE ((STM32VectorTable *)0x1fffec00)

#elif defined(STM32F1)

#define BOOT_VECTOR_TABLE ((STM32VectorTable *)0x1ffff000)

#elif defined(STM32G0)

#include <libopencm3/stm32/syscfg.h>

#define BOOT_VECTOR_TABLE ((STM32VectorTable *)0x1fff0000)

#endif

uint32_t getDeviceId(void)
{
    return DESIG_UNIQUE_ID0 ^ DESIG_UNIQUE_ID1 ^ DESIG_UNIQUE_ID0;
}

void startBootloader(void)
{
    // Disable interrupts

    __asm__ volatile("CPSID I\n");

    // Disable systick

    STK_CSR = 0;

#if defined(STM32G0)

    // Reset RCC

    RCC_CR = 0x00000500;

#endif

    for (uint8_t i = 0; i < (NVIC_IRQ_COUNT + 31) / 32; i++)
    {
        NVIC_ICER(i) = 0xffffffff;
        NVIC_ICPR(i) = 0xffffffff;
    }

    // Enable interrupts

    __asm__ volatile("CPSIE I\n");

    // Set MSP

    __asm__ volatile("msr msp, %0"
                     :
                     : "r"(BOOT_VECTOR_TABLE->initialSP));

    // Jump to reset handler

    BOOT_VECTOR_TABLE->resetHandler();
}

#endif
