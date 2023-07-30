/*
 * Rad Pro
 * SDLSim RTC interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include <stdio.h>
#include <time.h>

#include "../../rtc.h"

#define RTC_REALTIME

static uint32_t currentTimestamp;

void initRTC(void)
{
    currentTimestamp = RTC_TIMESTAMP_MIN;
}

void setRTCDateTime(struct RTCDateTime *dateTime)
{
    printf("Set RTC date time: %d, %d, %d, %d, %d\n",
           dateTime->year,
           dateTime->month,
           dateTime->day,
           dateTime->hour,
           dateTime->minute);

    currentTimestamp = getTimestampFromDateTime(dateTime);
}

void getRTCDateTime(struct RTCDateTime *dateTime)
{
#ifdef RTC_REALTIME
    uint32_t timestamp = getRTCTimestamp();
    getDateTimeFromTimestamp(timestamp, dateTime);
#else
    getDateTimeFromTimestamp(currentTimestamp, dateTime);
#endif
}

uint32_t getRTCTimestamp(void)
{
#ifdef RTC_REALTIME
    return (uint32_t)time(NULL);
#else
    return currentTimestamp;
#endif
}

#endif
