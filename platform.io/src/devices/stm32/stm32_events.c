/*
 * Rad Pro
 * STM32 event handler
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32F0) || defined(STM32F1) || defined(STM32G0)

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/iwdg.h>

#include "../../events.h"

static struct
{
    volatile uint32_t tick;
} eventsHardware;

void initEventsHardware(void)
{
    // SysTick
    systick_set_frequency(SYS_TICK_FREQUENCY, rcc_ahb_frequency);
    systick_clear();

    systick_interrupt_enable();
    systick_counter_enable();

    nvic_set_priority(NVIC_SYSTICK_IRQ, 255);

    // Watchdog
    // +++ TEST
    // iwdg_set_period_ms(500 * 40000 / 32768);
    iwdg_set_period_ms(30000);
    // --- TEST
    iwdg_start();
}

void sys_tick_handler(void)
{
    eventsHardware.tick++;

    onTick();
}

void sleep(uint32_t value)
{
    uint32_t tickStart = eventsHardware.tick;

    iwdg_reset();
    while ((eventsHardware.tick - tickStart) < value)
    {
        asm("wfi");
        iwdg_reset();
    }
}

#endif
