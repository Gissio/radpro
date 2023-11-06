/*
 * Rad Pro
 * SDLSim real-time clock
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdio.h>
#include <time.h>

#include "../rtc.h"

#define SDLSIM_RTC_REALTIME

static uint32_t currentTime;

void initRTC(void)
{
    currentTime = RTC_TIME_START;
}

void setRTCDateTime(struct RTCDateTime *dateTime)
{
    currentTime = getTimeFromDateTime(dateTime);
}

void getRTCDateTime(struct RTCDateTime *dateTime)
{
#if defined(SDLSIM_RTC_REALTIME)

    uint32_t time = getRTCTime();
    getDateTimeFromTime(time, dateTime);

#else

    getDateTimeFromTime(currentTime, dateTime);

#endif
}

void setRTCTime(uint32_t time)
{
    currentTime = time;
}

uint32_t getRTCTime(void)
{
#if defined(SDLSIM_RTC_REALTIME)

    return (uint32_t)time(NULL);

#else

    return currentTime;

#endif
}

#endif
