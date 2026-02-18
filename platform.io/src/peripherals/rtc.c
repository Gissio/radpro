/*
 * Rad Pro
 * Real-time clock
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/datalog.h"
#include "../peripherals/rtc.h"
#include "../system/cmath.h"
#include "../system/settings.h"
#include "../ui/menu.h"

static const Menu rtcMenu;

void setupRTC(void)
{
    selectMenuItem(&rtcMenu, 0);
}

static void normalizeDateTime(RTCDateTime *dateTime)
{
    if (dateTime->year < RTC_YEAR_MIN)
        dateTime->year = RTC_YEAR_MIN;
    if (dateTime->year > RTC_YEAR_MAX)
        dateTime->year = RTC_YEAR_MAX;
}

static int32_t getTimeZoneOffset(void)
{
    return 3600 * (settings.rtcTimeZone - 12);
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

static const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static uint32_t getDaysInMonth(uint32_t year, uint32_t month)
{
    if (month == 2)
    {
        bool isLeap = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);

        return isLeap ? 29 : 28;
    }

    return daysInMonth[month - 1];
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
    int32_t index = (int32_t)(value + RTC_TIMEZONE_P0000);

    if ((index < 0) || (index >= RTC_TIMEZONE_NUM))
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

static RTCDateTime rtcCurrentDateTime;

typedef const struct
{
    uint32_t offset;
    uint32_t itemCount;
} RTCMenuOptionSetting;

static RTCMenuOptionSetting rtcMenuOptionSettings[] = {
    {RTC_YEAR_MIN, RTC_YEAR_NUM},
    {1, 12},
    {1, 31},
    {0, 24},
    {0, 60},
};

static const char *onRTCOptionMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    switch (rtcMenuState.selectedIndex)
    {
    case DATETIME_YEAR:
        *menuStyle = (index == (uint32_t)(rtcCurrentDateTime.year - RTC_YEAR_MIN));

        break;

    case DATETIME_MONTH:
        *menuStyle = (index == (uint32_t)(rtcCurrentDateTime.month - 1));

        break;

    case DATETIME_DAY:
        *menuStyle = (index == (uint32_t)(rtcCurrentDateTime.day - 1));

        break;

    case DATETIME_HOUR:
        *menuStyle = (index == rtcCurrentDateTime.hour);

        break;

    case DATETIME_MINUTE:
        *menuStyle = (index == rtcCurrentDateTime.minute);

        break;
    }

    RTCMenuOptionSetting *rtcMenuOptionSetting = &rtcMenuOptionSettings[rtcMenuState.selectedIndex - 1];

    uint32_t itemCount = rtcMenuOptionSetting->itemCount;
    if (rtcMenuState.selectedIndex == DATETIME_DAY)
        itemCount = getDaysInMonth(rtcCurrentDateTime.year, rtcCurrentDateTime.month);

    if (index < itemCount)
    {
        strclr(menuOption);

        if ((rtcMenuState.selectedIndex == DATETIME_HOUR) &&
            (settings.rtcTimeFormat == RTC_TIMEFORMAT_12_HOUR))
        {
            uint32_t hour = index % 12;
            strcatUInt32(menuOption, (hour == 0) ? 12 : hour, 1);
            strcatChar(menuOption, ' ');
            strcat(menuOption, index < 12 ? getString(STRING_AM) : getString(STRING_PM));
        }
        else
            strcatUInt32(menuOption, rtcMenuOptionSetting->offset + index, 0);

        return menuOption;
    }
    else
        return NULL;
}

static void onRTCOptionMenuSelect(menu_size_t index)
{
    RTCDateTime dateTime;
    getDeviceDateTime(&dateTime);
    normalizeDateTime(&dateTime);

    switch (rtcMenuState.selectedIndex)
    {
    case DATETIME_YEAR:
        dateTime.year = RTC_YEAR_MIN + index;

        break;

    case DATETIME_MONTH:
        dateTime.month = index + 1;

        break;

    case DATETIME_DAY:
        dateTime.day = index + 1;

        break;

    case DATETIME_HOUR:
        dateTime.hour = index;

        break;

    case DATETIME_MINUTE:
        dateTime.minute = index;
        dateTime.second = 0;

        break;
    }

    setDeviceDateTime(&dateTime);
    rtcCurrentDateTime = dateTime;

    writeDatalogTimeChange();
}

static MenuState rtcItemMenuState;

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

// Year menu

static const Menu rtcYearMenu = {
    STRING_YEAR,
    &rtcItemMenuState,
    RTC_YEAR_NUM,
    onRTCOptionMenuGetOption,
    onRTCOptionMenuSelect,
    showRTCMenu,
};

// Month menu

static const Menu rtcMonthMenu = {
    STRING_MONTH,
    &rtcItemMenuState,
    12,
    onRTCOptionMenuGetOption,
    onRTCOptionMenuSelect,
    showRTCMenu,
};

// Day menu

static Menu rtcDayMenu = {
    STRING_DAY,
    &rtcItemMenuState,
    0,
    onRTCOptionMenuGetOption,
    onRTCOptionMenuSelect,
    showRTCMenu,
};

// Hour menu

static const Menu rtcHourMenu = {
    STRING_HOUR,
    &rtcItemMenuState,
    24,
    onRTCOptionMenuGetOption,
    onRTCOptionMenuSelect,
    showRTCMenu,
};

// Minute menu

static const Menu rtcMinuteMenu = {
    STRING_MINUTE,
    &rtcItemMenuState,
    60,
    onRTCOptionMenuGetOption,
    onRTCOptionMenuSelect,
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
    getDeviceDateTime(&rtcCurrentDateTime);
    normalizeDateTime(&rtcCurrentDateTime);

    const Menu *menu = rtcMenus[index];
    uint32_t menuIndex = 0;

    switch (rtcMenuState.selectedIndex)
    {
    case DATETIME_ZONE:
        menuIndex = settings.rtcTimeZone;

        break;

    case DATETIME_YEAR:
        menuIndex = rtcCurrentDateTime.year - RTC_YEAR_MIN;

        break;

    case DATETIME_MONTH:
        menuIndex = rtcCurrentDateTime.month - 1;

        break;

    case DATETIME_DAY:
        menuIndex = rtcCurrentDateTime.day - 1;
        rtcDayMenu.itemCount = getDaysInMonth(rtcCurrentDateTime.year, rtcCurrentDateTime.month);

        break;

    case DATETIME_HOUR:
        menuIndex = rtcCurrentDateTime.hour;

        break;

    case DATETIME_MINUTE:
        menuIndex = rtcCurrentDateTime.minute;

        break;

    case DATETIME_FORMAT:
        menuIndex = settings.rtcTimeFormat;

        break;
    }

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
