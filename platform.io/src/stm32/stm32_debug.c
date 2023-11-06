/*
 * Rad Pro
 * STM32 debugging
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32_HARD_FAULT_HANDLER)

#include "../debug.h"

void onHardFault(uint32_t *args);

// args: r0, r1, r2, r3, r12, lr, pc, psr
void onHardFault(uint32_t *args)
{
    debugUInt32(0);

    for (uint32_t i = 0; i < 8; i++)
        debugUInt32(args[i]);

    while (true)
        sleep(0);
}

void hard_fault_handler(void)
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
