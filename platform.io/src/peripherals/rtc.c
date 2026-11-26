/*
 * Rad Pro
 * Real-time clock
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stddef.h>

#include "../measurements/datalog.h"
#include "../peripherals/rtc.h"
#include "../system/cmath.h"
#include "../system/settings.h"
#include "../ui/menu.h"

static const Menu rtcMenu;

void resetRTC(void)
{
    selectMenuItem(&rtcMenu, 0);
}

static int32_t getTimeZoneOffset(void)
{
    return 3600 * ((int32_t)settings.rtcTimeZone - RTC_TIMEZONE_P0000);
}

// Based on https://howardhinnant.github.io/date_algorithms.html

uint32_t getTimeFromDateTime(const RTCDateTime *dateTime)
{
    uint32_t year = dateTime->year - (dateTime->month <= 2);
    uint32_t era = year / 400;
    uint32_t yearOfEra = year - era * 400;
    uint32_t month = dateTime->month;
    uint32_t day = dateTime->day;
    uint32_t dayOfYear = (153 * (month > 2 ? (month - 3) : month + 9) + 2) / 5 + day - 1;
    uint32_t dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;
    uint32_t dayOfEpoch = era * 146097 + dayOfEra - 719468;

    return 86400 * dayOfEpoch + 3600 * dateTime->hour + 60 * dateTime->minute + dateTime->second;
}

void getDateTimeFromTime(uint32_t time, RTCDateTime *dateTime)
{
    uint32_t dayOfEpoch = time / 86400 + 719468;
    uint32_t secondsOfDay = time % 86400;

    uint32_t era = dayOfEpoch / 146097;
    uint32_t dayOfEra = dayOfEpoch - era * 146097;
    uint32_t yearOfEra = (dayOfEra - dayOfEra / 1460 + dayOfEra / 36524 - dayOfEra / 146096) / 365;
    uint32_t year = yearOfEra + era * 400;
    uint32_t dayOfYear = dayOfEra - (365 * yearOfEra + yearOfEra / 4 - yearOfEra / 100);
    uint32_t mp = (5 * dayOfYear + 2) / 153;
    uint32_t day = dayOfYear - (153 * mp + 2) / 5 + 1;
    uint32_t month = mp < 10 ? mp + 3 : mp - 9;

    dateTime->year = year + (month <= 2);
    dateTime->month = month;
    dateTime->day = day;
    dateTime->hour = secondsOfDay / 3600;
    dateTime->minute = (secondsOfDay / 60) % 60;
    dateTime->second = secondsOfDay % 60;
}

void setDeviceDateTime(const RTCDateTime *dateTime)
{
    setDeviceTime(getTimeFromDateTime(dateTime) - getTimeZoneOffset());
}

void getDeviceDateTime(RTCDateTime *dateTime)
{
    getDateTimeFromTime(getDeviceTime() + getTimeZoneOffset(), dateTime);
}

bool setDeviceTimeZone(float value)
{
    // Round to the nearest whole-hour time zone; also rejects NaN
    float index = value + (RTC_TIMEZONE_P0000 + 0.5F);

    if (!((index >= 0.0F) && (index < RTC_TIMEZONE_NUM)))
        return false;

    settings.rtcTimeZone = (uint32_t)index;

    return true;
}

float getDeviceTimeZone(void)
{
    float value = settings.rtcTimeZone;

    return value - RTC_TIMEZONE_P0000;
}

// RTC menu common

enum
{
    DATETIME_ZONE,
    DATETIME_YEAR,
    DATETIME_MONTH,
    DATETIME_DAY,
    DATETIME_HOUR,
    DATETIME_MINUTE,
    DATETIME_FORMAT,
};

static MenuState rtcMenuState;
static MenuState rtcItemMenuState;

static RTCDateTime rtcCurrentDateTime;

// Only valid for RTC_YEAR_MIN...RTC_YEAR_MAX (no century leap-year exceptions)
static uint32_t getDaysInMonth(uint32_t year, uint32_t month)
{
    if (month == 2)
        return (year % 4) ? 28 : 29;

    return 30 + ((month + (month >> 3)) & 1);
}

NOINLINE static void getMenuDeviceDateTime(RTCDateTime *dateTime)
{
    getDeviceDateTime(dateTime);

    if (dateTime->year < RTC_YEAR_MIN)
        dateTime->year = RTC_YEAR_MIN;
    if (dateTime->year > RTC_YEAR_MAX)
        dateTime->year = RTC_YEAR_MAX;
}

// Date/time field menus (year...minute)

// Value of the first item of each field menu
static const uint16_t rtcFieldOffsets[] = {
    RTC_YEAR_MIN,
    1,
    1,
    0,
    0,
};

// Month, day, hour and minute are consecutive bytes, indexed by field
_Static_assert((offsetof(RTCDateTime, minute) - offsetof(RTCDateTime, month)) ==
                   (DATETIME_MINUTE - DATETIME_MONTH),
               "RTCDateTime byte fields must match field order");

static uint8_t *getFieldByte(RTCDateTime *dateTime, uint32_t field)
{
    return (uint8_t *)dateTime + offsetof(RTCDateTime, month) + (field - DATETIME_MONTH);
}

static uint32_t getFieldValue(uint32_t field)
{
    if (field == DATETIME_YEAR)
        return rtcCurrentDateTime.year;

    return *getFieldByte(&rtcCurrentDateTime, field);
}

static const char *onRTCFieldMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    uint32_t field = rtcMenuState.selectedIndex;
    uint32_t value = rtcFieldOffsets[field - DATETIME_YEAR] + index;

    *menuStyle = (value == getFieldValue(field));

    strclr(menuOption);

    if ((field == DATETIME_HOUR) &&
        (settings.rtcTimeFormat == RTC_TIMEFORMAT_12_HOUR))
    {
        uint32_t hour = index % 12;
        strcatUInt32(menuOption, hour ? hour : 12, 1);
        strcatChar(menuOption, ' ');
        strcat(menuOption, getString((index < 12) ? STRING_AM : STRING_PM));
    }
    else
        strcatUInt32(menuOption, value, 0);

    return menuOption;
}

static void onRTCFieldMenuSelect(menu_size_t index)
{
    uint32_t field = rtcMenuState.selectedIndex;
    uint32_t value = rtcFieldOffsets[field - DATETIME_YEAR] + index;

    RTCDateTime dateTime;
    getMenuDeviceDateTime(&dateTime);

    if (field == DATETIME_YEAR)
        dateTime.year = value;
    else
        *getFieldByte(&dateTime, field) = value;

    if (field == DATETIME_MINUTE)
        dateTime.second = 0;

    // Clamp day so changing month or year never rolls over into the next month
    uint32_t maxDay = getDaysInMonth(dateTime.year, dateTime.month);
    if (dateTime.day > maxDay)
        dateTime.day = maxDay;

    setDeviceDateTime(&dateTime);
    rtcCurrentDateTime = dateTime;

    writeDatalogTimeChange();
}

static const Menu rtcYearMenu = {
    STRING_YEAR,
    &rtcItemMenuState,
    RTC_YEAR_NUM,
    onRTCFieldMenuGetOption,
    onRTCFieldMenuSelect,
    showRTCMenu,
};

static const Menu rtcMonthMenu = {
    STRING_MONTH,
    &rtcItemMenuState,
    12,
    onRTCFieldMenuGetOption,
    onRTCFieldMenuSelect,
    showRTCMenu,
};

static Menu rtcDayMenu = {
    STRING_DAY,
    &rtcItemMenuState,
    0,
    onRTCFieldMenuGetOption,
    onRTCFieldMenuSelect,
    showRTCMenu,
};

static const Menu rtcHourMenu = {
    STRING_HOUR,
    &rtcItemMenuState,
    24,
    onRTCFieldMenuGetOption,
    onRTCFieldMenuSelect,
    showRTCMenu,
};

static const Menu rtcMinuteMenu = {
    STRING_MINUTE,
    &rtcItemMenuState,
    60,
    onRTCFieldMenuGetOption,
    onRTCFieldMenuSelect,
    showRTCMenu,
};

// Time zone menu

static const char *onRTCTimeZoneMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rtcTimeZone);

    int32_t deltaTimeMinutes = 60 * (index - RTC_TIMEZONE_P0000);

    if (deltaTimeMinutes < 0)
    {
        strcpy(menuOption, getString(STRING_UTCMINUS));

        deltaTimeMinutes = -deltaTimeMinutes;
    }
    else
        strcpy(menuOption, getString(STRING_UTCPLUS));

    // Minutes passed as seconds yields "H:MM"
    strcatTime(menuOption, deltaTimeMinutes);

    return menuOption;
}

static void onRTCTimeZoneMenuSelect(menu_size_t index)
{
    settings.rtcTimeZone = index;
}

static const Menu rtcTimeZoneMenu = {
    STRING_TIME_ZONE,
    &rtcItemMenuState,
    RTC_TIMEZONE_NUM,
    onRTCTimeZoneMenuGetOption,
    onRTCTimeZoneMenuSelect,
    showRTCMenu,
};

// Time format menu

static cstring rtcTimeFormatMenuOptions[] = {
    STRING_24_HOUR,
    STRING_12_HOUR,
};

static const char *onRTCTimeFormatMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rtcTimeFormat);

    return getString(rtcTimeFormatMenuOptions[index]);
}

static void onRTCTimeFormatMenuSelect(menu_size_t index)
{
    settings.rtcTimeFormat = index;
}

static const Menu rtcTimeFormatMenu = {
    STRING_TIME_FORMAT,
    &rtcItemMenuState,
    ARRAY_SIZE(rtcTimeFormatMenuOptions),
    onRTCTimeFormatMenuGetOption,
    onRTCTimeFormatMenuSelect,
    showRTCMenu,
};

// RTC menu

static cstring rtcMenuOptions[] = {
    STRING_TIME_ZONE,
    STRING_YEAR,
    STRING_MONTH,
    STRING_DAY,
    STRING_HOUR,
    STRING_MINUTE,
    STRING_TIME_FORMAT,
};

static const Menu *rtcMenus[] = {
    &rtcTimeZoneMenu,
    &rtcYearMenu,
    &rtcMonthMenu,
    &rtcDayMenu,
    &rtcHourMenu,
    &rtcMinuteMenu,
    &rtcTimeFormatMenu,
};

static const char *onRTCMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(rtcMenuOptions[index]);
}

static void onRTCMenuSelect(menu_size_t index)
{
    uint32_t menuIndex;

    if (index == DATETIME_ZONE)
        menuIndex = settings.rtcTimeZone;
    else if (index == DATETIME_FORMAT)
        menuIndex = settings.rtcTimeFormat;
    else
    {
        getMenuDeviceDateTime(&rtcCurrentDateTime);

        rtcDayMenu.itemCount = getDaysInMonth(rtcCurrentDateTime.year,
                                              rtcCurrentDateTime.month);

        menuIndex = getFieldValue(index) - rtcFieldOffsets[index - DATETIME_YEAR];
    }

    const Menu *menu = rtcMenus[index];
    selectMenuItem(menu, menuIndex);
    showMenu(menu);
}

static const Menu rtcMenu = {
    STRING_DATE_AND_TIME,
    &rtcMenuState,
    ARRAY_SIZE(rtcMenuOptions),
    onRTCMenuGetOption,
    onRTCMenuSelect,
    showSettingsMenu,
};

void showRTCMenu(void)
{
    showMenu(&rtcMenu);
}
