/*
 * Rad Pro
 * Debugging module
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "debug.h"
#include "events.h"

#define DEBUG_BIT_LENGTH 100
#define DEBUG_BIT0_LENGTH 1
#define DEBUG_BIT1_LENGTH 15

void debugBeep(void)
{
    setBuzzer(true);
    sleep(100);
    setBuzzer(false);
    sleep(400);
}

void debugWait(uint32_t ms)
{
    uint32_t n = 200 * ms;

    for (uint32_t i = 0; i < n; i++)
        sleep(0);
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
