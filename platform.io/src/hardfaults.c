/*
 * FS2011 Pro
 * Hard Fault handler
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "events.h"
#include "main.h"

#ifdef HARD_FAULT_HANDLER

#define HARDFAULT_BIT_LENGTH 20000

void onHardFault(uint32_t *args);

static void onHardFaultBit(uint32_t value)
{
    uint32_t onTime;
    uint32_t offTime;
    switch (value)
    {
    case 0:
        onTime = 100;
        offTime = HARDFAULT_BIT_LENGTH - 100;
        break;

    case 1:
        onTime = 1000;
        offTime = HARDFAULT_BIT_LENGTH - 1000;
        break;

    default:
        onTime = 0;
        offTime = 4 * HARDFAULT_BIT_LENGTH;
        break;
    }

    setBuzzer(true);
    for (uint32_t i = 0; i < onTime; i++)
        waitSysTicks(0);
    setBuzzer(false);
    for (uint32_t i = 0; i < offTime; i++)
        waitSysTicks(0);
}

static void onHardFaultInt(uint32_t value)
{
    onHardFaultBit(2);

    for (uint32_t j = 0; j < 32; j++)
    {
        onHardFaultBit(value >> 31);
        value <<= 1;
    }
}

// args: r0, r1, r2, r3, r12, lr, pc, psr
void onHardFault(uint32_t *args)
{
    onHardFaultInt(0);

    for (uint32_t i = 0; i < 8; i++)
        onHardFaultInt(args[i]);

    while(true)
        waitSysTicks(0);
}

void hard_fault_handler(void)
{
    __asm volatile(
        " movs r0, #4                                               \n"
        " mov r1, lr                                                \n"
        " tst r0, r1                                                \n"
        " bne HARDFAULT_PSP                                         \n"
        " mrs r0, msp                                               \n"
        " b HARDFAULT_CALL                                          \n"
        "HARDFAULT_PSP:                                             \n"
        " mrs r0, psp                                               \n"
        "HARDFAULT_CALL:                                            \n"
        " ldr r2,=onHardFault                                       \n"
        " bx r2                                                     \n");
}
#endif
