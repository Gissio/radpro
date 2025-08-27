/*
 * Rad Pro
 * STM32 events
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../comm.h"
#include "../events.h"
#include "../power.h"

#include "device.h"

extern volatile uint32_t eventsTick;

float timerCountToSeconds = (1.0F / PULSE_MEASUREMENT_FREQUENCY);

void initEventsHardware(void)
{
    // SysTick
    NVIC_SetPriority(SysTick_IRQn, 0xc0);
    SysTick->LOAD = AHB_FREQUENCY / SYSTICK_FREQUENCY - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    iwdg_unlock();
    wait_until_bits_clear(IWDG->SR, IWDG_SR_PVU);
    IWDG->PR = 0b110; // Divider /256

    iwdg_unlock();
    wait_until_bits_clear(IWDG->SR, IWDG_SR_RVU);
    IWDG->RLR = (LSI_FREQUENCY / 256) - 1;

    iwdg_start();
}

void SysTick_Handler(void)
{
    onTick();

    eventsTick++;
}

void resetWatchdog(void)
{
    iwdg_reload();
}

void sleep(uint32_t value)
{
    uint32_t targetTick = eventsTick + value;

    while (true)
    {
        iwdg_reload();

        if (((int32_t)(eventsTick - targetTick)) >= 0)
            break;

#if defined(USB_INTERFACE) && defined(DATA_MODE)
        // CH32F103R8T6 does not wake USB on IRQ
        if (!settings.dataMode)
            __asm volatile("wfi");
#else
        __asm volatile("wfi");
#endif
    }
}

#endif
