/*
 * Rad Pro
 * SDLSim real-time clock
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdio.h>
#include <time.h>

#include "../rtc.h"

static int32_t timeDelta;

void initRTCHardware(void)
{
    timeDelta = 0;
}

static uint32_t getLocalTime(void)
{
    return time(NULL);
}

void setRTCTime(uint32_t value)
{
    timeDelta = value - getLocalTime();
}

uint32_t getRTCTime(void)
{
    return getLocalTime() + timeDelta;
}

#endif
