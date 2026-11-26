/*
 * Rad Pro
 * STM32 events
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../stm32/device.h"
#include "../system/events.h"
#include "../system/power.h"

#if defined(CH32)
// CH32's do not wake USB on IRQ
#define canSleep() !isUSBPowered()
#else
#define canSleep() true
#endif

void initEvents(void)
{
    // SysTick
    nvic_set_priority(SysTick_IRQn, 0x80);
    SysTick->LOAD = AHB_FREQUENCY / SYSTICK_FREQUENCY - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    iwdg_start(); // Start IWDG first (see AN2606 4.8)

    iwdg_unlock(); // Unlocks both PR and RLR
    wait_until_bits_clear(IWDG->SR, IWDG_SR_PVU | IWDG_SR_RVU);
    IWDG->PR = 0b110; // Divider /256
    IWDG->RLR = (LSI_FREQUENCY / 256) - 1;
}

void SysTick_Handler(void)
{
    currentTick++;

    onTick();
}

void reloadWatchdog(void)
{
    iwdg_reload();
}

void sleep(uint32_t value)
{
    uint32_t targetTick = currentTick + value;
    bool waiting;

    do
    {
        iwdg_reload();

        // Check and sleep with IRQs masked, so a tick arriving between the
        // check and WFI is not missed (WFI still wakes on pending IRQs)
        __disable_irq();

        waiting = (int32_t)(currentTick - targetTick) < 0;
        if (waiting && canSleep())
            __WFI();

        __enable_irq();
    } while (waiting);
}

#endif
