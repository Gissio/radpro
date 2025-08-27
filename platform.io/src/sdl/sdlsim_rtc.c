/*
 * Rad Pro
 * Simulator real-time clock
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>
#include <time.h>

#include "../rtc.h"

static int32_t timeDelta;

void initRTC(void)
{
}

static uint32_t getLocalTime(void)
{
    return time(NULL);
}

void setDeviceTime(uint32_t value)
{
    timeDelta = value - getLocalTime();
}

uint32_t getDeviceTime(void)
{
    return getDeviceTimeFast();
}

uint32_t getDeviceTimeFast(void)
{
    return getLocalTime() + timeDelta;
}

#endif
