/*
 * Rad Pro
 * Real-time clock
 *
 * (C) 2022-2025 Gissio
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
#define RTC_TIME_INIT 946684800 // 2000-01-01T00:00:00+0000

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} RTCDateTime;

extern const View rtcMenuView;

void initRTC(void);
void initRTCMenus(void);

uint32_t getTimeFromDateTime(const RTCDateTime *dateTime);
void getDateTimeFromTime(uint32_t value, RTCDateTime *dateTime);
uint32_t getTimeZoneOffset(void);

void setDeviceDateTime(const RTCDateTime *dateTime);
void getDeviceDateTime(RTCDateTime *dateTime);

void setDeviceTime(uint32_t value);
uint32_t getDeviceTime(void);

uint32_t getDeviceTimeFast(void);

#endif
