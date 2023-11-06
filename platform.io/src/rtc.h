/*
 * Rad Pro
 * Real-time clock
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(RTC_H)

#define RTC_H

#include <stdint.h>

#include "view.h"

#define RTC_YEAR_MIN 2020
#define RTC_YEAR_MAX 2099
#define RTC_YEAR_NUM (RTC_YEAR_MAX - RTC_YEAR_MIN + 1)
#define RTC_TIME_START 946684800 // 2000-01-01 00:00:00

struct RTCDateTime
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

extern const struct View dateAndTimeMenuView;

void initRTC(void);

uint32_t getTimeFromDateTime(struct RTCDateTime *dateTime);
void getDateTimeFromTime(uint32_t time, struct RTCDateTime *dateTime);

void setRTCDateTime(struct RTCDateTime *dateTime);
void getRTCDateTime(struct RTCDateTime *dateTime);

void setRTCTime(uint32_t time);
uint32_t getRTCTime(void);

#endif
