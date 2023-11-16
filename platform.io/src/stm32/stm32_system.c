/*
 * Rad Pro
 * STM32 system
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../buzzer.h"
#include "../display.h"
#include "../events.h"
#include "../system.h"
#include "../tube.h"

#include "stm32.h"

#if defined(STM32F0)

#define BOOT_VECTOR_TABLE ((struct STM32VectorTable *)0x1fffec00)

#elif defined(STM32F1)

#define BOOT_VECTOR_TABLE ((struct STM32VectorTable *)0x1ffff000)

#elif defined(STM32G0)

#include <libopencm3/stm32/syscfg.h>

#define BOOT_VECTOR_TABLE ((struct STM32VectorTable *)0x1fff0000)

#endif

struct STM32VectorTable
{
    uint32_t initialSP;
    void (*resetHandler)(void);
};

void initSystem(void)
{
    // Clock

#if defined(FS600) || defined(FS1000)

    rcc_set_hsisys_div(RCC_CR_HSIDIV_DIV2);
    rcc_ahb_frequency = rcc_apb1_frequency = SYS_FREQUENCY;

#elif defined(GC01)

    rcc_clock_setup_in_hse_8mhz_out_72mhz();

#endif

    // GPIO

#if defined(FS2011) && defined(STM32F0)

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOF);

#elif defined(FS2011) && defined(STM32F1)

    rcc_periph_clock_enable(RCC_AFIO);
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

#elif defined(FS600) || defined(FS1000)

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);

    // Disable UCPD strobes
    SYSCFG_CFGR1 |= (SYSCFG_CFGR1_UCPD1_STROBE | SYSCFG_CFGR1_UCPD2_STROBE);

#elif defined(GC01)

    rcc_periph_clock_enable(RCC_AFIO);
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, 0);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

#endif
}

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
