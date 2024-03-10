/*
 * Rad Pro
 * STM32 events
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../events.h"
#include "../power.h"

#include "device.h"

extern volatile uint32_t eventsCurrentTick;

float timerCountToSeconds = (1.0F / PULSE_MEASUREMENT_FREQUENCY);

void initEventsController(void)
{
    SysTick->LOAD = AHB_FREQUENCY / SYSTICK_FREQUENCY - 1;
    SysTick->VAL = 0;
    NVIC_SetPriority(SysTick_IRQn, 0xc0);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    wait_until_bits_clear(IWDG->SR, IWDG_SR_PVU);
    iwdg_unlock();
    IWDG->PR = 1;
    wait_until_bits_clear(IWDG->SR, IWDG_SR_RVU);
    iwdg_unlock();
    IWDG->RLR = 1000 * LSI_FREQUENCY - 1;
    // +++ TEST
    // iwdg_start();
    // +++ TEST
}

void SysTick_Handler(void)
{
    onTick();

    eventsCurrentTick++;
}

void sleep(uint32_t value)
{
    iwdg_reload();

    uint32_t startTick = eventsCurrentTick;

    while ((eventsCurrentTick - startTick) < value)
    {
#if defined(GC01) && defined(CH32)
        // Disable sleep when charging as CH32F103 does not wake USB on IRQ
        // +++ TEST
        // if (!isBatteryCharging())
        //     __asm volatile("wfi");
        // +++ TEST
#else
        __asm volatile("wfi");
#endif

        iwdg_reload();
    }
}

#endif
