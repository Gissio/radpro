/*
 * FS2011 Pro
 * Debugging module
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "debug.h"
#include "events.h"
#include "main.h"

#ifdef ERROR_HANDLERS

void onHardFault(uint32_t *args);

#define DEBUG_BIT_LENGTH 100
#define DEBUG_BIT0_LENGTH 1
#define DEBUG_BIT1_LENGTH 15

static void debugWait(uint32_t ms)
{
    uint32_t n = 200 * ms;

    for (uint32_t i = 0; i < n; i++)
        waitSysTicks(0);
}

void debugBit(bool value)
{
    uint32_t onTime;
    uint32_t offTime;

    switch (value)
    {
    case 0:
        onTime = DEBUG_BIT0_LENGTH;
        offTime = DEBUG_BIT_LENGTH - DEBUG_BIT0_LENGTH;
        break;

    case 1:
        onTime = DEBUG_BIT1_LENGTH;
        offTime = DEBUG_BIT_LENGTH - DEBUG_BIT1_LENGTH;
        break;
    }

    setBuzzer(true);
    debugWait(onTime);
    setBuzzer(false);
    debugWait(offTime);
}

void debugUInt32(uint32_t value)
{
    debugWait(500);

    for (uint32_t j = 0; j < 32; j++)
    {
        debugBit(value >> 31);
        value <<= 1;
    }
}

// args: r0, r1, r2, r3, r12, lr, pc, psr
void onHardFault(uint32_t *args)
{
    debugUInt32(0);

    for (uint32_t i = 0; i < 8; i++)
        debugUInt32(args[i]);

    while (true)
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
