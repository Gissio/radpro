/*
 * Rad Pro
 * Simulator real-time clock
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <time.h>

#include "../peripherals/rtc.h"

// Device time = host time + offset
static uint32_t timeOffset;

void initRTC(void)
{
}

bool setDeviceTime(uint32_t value)
{
    timeOffset = value - (uint32_t)time(NULL);

    return true;
}

uint32_t getDeviceTime(void)
{
    return (uint32_t)time(NULL) + timeOffset;
}

uint32_t getDeviceTimeFast(void)
{
    return getDeviceTime();
}

#endif
