/*
 * Rad Pro
 * Real time clock
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#include "menu.h"

#define RTC_YEAR_MIN 2020
#define RTC_YEAR_MAX 2099
#define RTC_TIMESTAMP_MIN 946684800 // 2000-01-01 00:00:00

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

uint32_t getTimestampFromDateTime(struct RTCDateTime *dateTime);
void getDateTimeFromTimestamp(uint32_t timestamp, struct RTCDateTime *dateTime);

void initRTC(void);
void setRTCDateTime(struct RTCDateTime *dateTime);
void getRTCDateTime(struct RTCDateTime *dateTime);
uint32_t getRTCTimestamp(void);

#endif
