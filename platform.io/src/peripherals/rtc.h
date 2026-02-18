/*
 * Rad Pro
 * Real-time clock
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(RTC_H)
#define RTC_H

#include <stdbool.h>
#include <stdint.h>

#define RTC_YEAR_MIN 2025
#define RTC_YEAR_MAX 2099
#define RTC_YEAR_NUM (RTC_YEAR_MAX - RTC_YEAR_MIN + 1)

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} RTCDateTime;

void initRTC(void);
void setupRTC(void);

uint32_t getTimeFromDateTime(const RTCDateTime *dateTime);
void getDateTimeFromTime(uint32_t value, RTCDateTime *dateTime);

void setDeviceDateTime(const RTCDateTime *dateTime);
void getDeviceDateTime(RTCDateTime *dateTime);

bool setDeviceTime(uint32_t value);
uint32_t getDeviceTime(void);
uint32_t getDeviceTimeFast(void);

bool setDeviceTimeZone(float value);
float getDeviceTimeZone(void);

void showRTCMenu(void);

#endif
