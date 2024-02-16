/*
 * Rad Pro
 * STM32 events
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/iwdg.h>

#include "../events.h"

#include "stm32.h"

extern volatile uint32_t eventsCurrentTick;

float timerCountToSeconds = (1.0F / DEADTIME_TIM_FREQUENCY);

void initEventsHardware(void)
{
    // SysTick

    systick_set_frequency(SYSTICK_FREQUENCY, AHB_FREQUENCY);
    systick_clear();

    nvic_set_priority(NVIC_SYSTICK_IRQ, 0xc0);

    systick_interrupt_enable();
    systick_counter_enable();

    // Watchdog

    iwdg_set_period_ms(500 * 40000 / 32768);
    iwdg_start();
}

void sys_tick_handler(void)
{
    onTick();

    eventsCurrentTick++;
}

void sleep(uint32_t value)
{
    iwdg_reset();

    uint32_t startTick = eventsCurrentTick;

    while ((eventsCurrentTick - startTick) < value)
    {
        __asm volatile("wfi");

        iwdg_reset();
    }
}

#endif
