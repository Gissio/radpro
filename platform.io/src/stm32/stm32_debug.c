/*
 * Rad Pro
 * STM32 debugging
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(DEBUG)

#include "device.h"

#include "../debug.h"
#include "../events.h"
#include "../power.h"

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
        resetWatchdog();
}

void onHardFault(const uint32_t *args)
{
    // args: R0, R1, R2, R3, R12, LR, PC, PSR

    // Send R12, LR, PC, PSR
    for (uint32_t i = 4; i < 8; i++)
        debugUInt32(args[i]);

    setPower(false);

    while (true)
        ;
}

#if DEBUG
void HardFault_Handler(void)
{
    __asm volatile(
        " movs r0, #4           \n"
        " mov r1, lr            \n"
        " tst r0, r1            \n"
        " bne HardFault_PSP     \n"
        " mrs r0, msp           \n"
        " b HardFault_call      \n"
        "HardFault_PSP:         \n"
        " mrs r0, psp           \n"
        "HardFault_call:        \n"
        " ldr r2,=onHardFault   \n"
        " bx r2                 \n");
}
#endif

#endif
