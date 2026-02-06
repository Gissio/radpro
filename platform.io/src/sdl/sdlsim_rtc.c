/*
 * Rad Pro
 * Simulator real-time clock
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>
#include <time.h>

#include "../devices/rtc.h"

static int32_t timeDelta;

void initRTC(void)
{
}

static uint32_t getLocalTime(void)
{
    return time(NULL);
}

bool setDeviceTime(uint32_t value)
{
    timeDelta = value - getLocalTime();

    return true;
}

uint32_t getDeviceTime(void)
{
    return getLocalTime() + timeDelta;
}

uint32_t getDeviceTimeFast(void)
{
    return getDeviceTime();
}

#endif
