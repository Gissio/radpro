/*
 * Rad Pro
 * STM32 events
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/comm.h"
#include "../stm32/device.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"

void initEventsHardware(void)
{
    // SysTick
    NVIC_SetPriority(SysTick_IRQn, 0x80);
    SysTick->LOAD = AHB_FREQUENCY / SYSTICK_FREQUENCY - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    iwdg_start(); // Start IWDG first (see AN2606 4.8)

    iwdg_unlock();
    wait_until_bits_clear(IWDG->SR, IWDG_SR_PVU);
    IWDG->PR = 0b110; // Divider /256

    iwdg_unlock();
    wait_until_bits_clear(IWDG->SR, IWDG_SR_RVU);
    IWDG->RLR = (LSI_FREQUENCY / 256) - 1;
}

void SysTick_Handler(void)
{
    currentTick++;

    onTick();
}

void resetWatchdog(void)
{
    iwdg_reload();
}

void sleep(uint32_t value)
{
    uint32_t targetTick = currentTick + value;

    while (true)
    {
        iwdg_reload();

        if (((int32_t)(currentTick - targetTick)) >= 0)
            break;

#if defined(CH32)
        // CH32's do not wake USB on IRQ
        if (!isUSBPowered())
            __WFI();
#else
        __WFI();
#endif
    }
}

#endif
