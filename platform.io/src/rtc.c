/*
 * Rad Pro
 * Real-time clock
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "cstring.h"
#include "menu.h"
#include "rtc.h"
#include "settings.h"

static Menu rtcMenu;
static Menu rtcTimeZoneMenu;

void resetRTC(void)
{
    selectMenuItem(&rtcMenu,
                   0,
                   0);

    selectMenuItem(&rtcTimeZoneMenu,
                   settings.rtcTimeZone,
                   RTC_TIMEZONE_NUM);
}

static void normalizeDateTime(RTCDateTime *dateTime)
{
    if (dateTime->year < RTC_YEAR_MIN)
        dateTime->year = RTC_YEAR_MIN;
    if (dateTime->year > RTC_YEAR_MAX)
        dateTime->year = RTC_YEAR_MAX;
}

uint32_t getTimeZoneOffset(void)
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

static const uint8_t daysInMonth[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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
    value += RTC_TIMEZONE_P0000;

    if ((value < 0) || (value >= RTC_TIMEZONE_NUM))
        return false;

    settings.rtcTimeZone = value;

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

static cstring rtcMenuOptions[] = {
    STRING_TIME_ZONE,
    STRING_YEAR,
    STRING_MONTH,
    STRING_DAY,
    STRING_HOUR,
    STRING_MINUTE,
    STRING_TIME_FORMAT,
    NULL,
};

static MenuState rtcMenuState;

static RTCDateTime rtcCurrentDateTime;

typedef const struct
{
    uint32_t offset;
    uint32_t maxIndex;
} RTCMenuOptionSetting;

static RTCMenuOptionSetting rtcMenuOptionSettings[] = {
    {RTC_YEAR_MIN, 80},
    {1, 12},
    {1, 31},
    {0, 24},
    {0, 60},
};

static const char *onRTCSubMenuGetOption(uint32_t index,
                                         MenuStyle *menuStyle)
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

    uint32_t maxIndex = rtcMenuOptionSetting->maxIndex;
    if (rtcMenuState.selectedIndex == DATETIME_DAY)
        maxIndex = getDaysInMonth(rtcCurrentDateTime.year, rtcCurrentDateTime.month);

    if (index < maxIndex)
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

static void onRTCSubMenuSelect(uint32_t index)
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
}

static void onRTCSubMenuBack(void)
{
    setView(&rtcMenuView);
}

static MenuState rtcItemMenuState;

// Time zone menu

static const char *onRTCTimeZoneMenuGetOption(uint32_t index,
                                              MenuStyle *menuStyle)
{
    if (index >= RTC_TIMEZONE_NUM)
        return NULL;

    *menuStyle = (index == settings.rtcTimeZone);

    int32_t deltaTimeMinutes = 60 * (index - 12);

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

static void onRTCTimeZoneMenuSelect(uint32_t index)
{
    settings.rtcTimeZone = index;
}

static Menu rtcTimeZoneMenu = {
    STRING_TIME_ZONE,
    &rtcItemMenuState,
    onRTCTimeZoneMenuGetOption,
    onRTCTimeZoneMenuSelect,
    onRTCSubMenuBack,
};

static View rtcTimeZoneMenuView = {
    onMenuEvent,
    &rtcTimeZoneMenu,
};

// Year menu

static Menu rtcYearMenu = {
    STRING_YEAR,
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static View rtcYearMenuView = {
    onMenuEvent,
    &rtcYearMenu,
};

// Month menu

static Menu rtcMonthMenu = {
    STRING_MONTH,
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static View rtcMonthMenuView = {
    onMenuEvent,
    &rtcMonthMenu,
};

// Day menu

static Menu rtcDayMenu = {
    STRING_DAY,
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static View rtcDayMenuView = {
    onMenuEvent,
    &rtcDayMenu,
};

// Hour menu

static Menu rtcHourMenu = {
    STRING_HOUR,
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static View rtcHourMenuView = {
    onMenuEvent,
    &rtcHourMenu,
};

// Minute menu

static Menu rtcMinuteMenu = {
    STRING_MINUTE,
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static View rtcMinuteMenuView = {
    onMenuEvent,
    &rtcMinuteMenu,
};

// Time format menu

static cstring rtcTimeFormatMenuOptions[] = {
    STRING_24_HOUR,
    STRING_12_HOUR,
    NULL,
};

static const char *onRTCTimeFormatMenuGetOption(uint32_t index,
                                                MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rtcTimeFormat);

    return getString(rtcTimeFormatMenuOptions[index]);
}

static void onRTCTimeFormatMenuSelect(uint32_t index)
{
    settings.rtcTimeFormat = index;
}

static Menu rtcTimeFormatMenu = {
    STRING_TIME_FORMAT,
    &rtcItemMenuState,
    onRTCTimeFormatMenuGetOption,
    onRTCTimeFormatMenuSelect,
    onRTCSubMenuBack,
};

static View rtcTimeFormatMenuView = {
    onMenuEvent,
    &rtcTimeFormatMenu,
};

// RTC menu

static const char *onRTCMenuGetOption(uint32_t index,
                                      MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(rtcMenuOptions[index]);
}

static void onRTCMenuSelect(uint32_t index)
{
    getDeviceDateTime(&rtcCurrentDateTime);
    normalizeDateTime(&rtcCurrentDateTime);

    View *view = NULL;
    uint32_t menuIndex = 0;
    uint32_t optionsNum = 0;

    switch (rtcMenuState.selectedIndex)
    {
    case DATETIME_ZONE:
        view = &rtcTimeZoneMenuView;
        menuIndex = settings.rtcTimeZone;
        optionsNum = RTC_TIMEZONE_NUM;

        break;

    case DATETIME_YEAR:
        view = &rtcYearMenuView;
        menuIndex = rtcCurrentDateTime.year - RTC_YEAR_MIN;
        optionsNum = RTC_YEAR_NUM;

        break;

    case DATETIME_MONTH:
        view = &rtcMonthMenuView;
        menuIndex = rtcCurrentDateTime.month - 1;
        optionsNum = 12;

        break;

    case DATETIME_DAY:
        view = &rtcDayMenuView;
        menuIndex = rtcCurrentDateTime.day - 1;
        optionsNum = getDaysInMonth(rtcCurrentDateTime.year, rtcCurrentDateTime.month);

        break;

    case DATETIME_HOUR:
        view = &rtcHourMenuView;
        menuIndex = rtcCurrentDateTime.hour;
        optionsNum = 24;

        break;

    case DATETIME_MINUTE:
        view = &rtcMinuteMenuView;
        menuIndex = rtcCurrentDateTime.minute;
        optionsNum = 60;

        break;

    case DATETIME_FORMAT:
        view = &rtcTimeFormatMenuView;
        menuIndex = settings.rtcTimeFormat;
        optionsNum = RTC_TIMEFORMAT_NUM;

        break;
    }

    selectMenuItem((Menu *)view->userdata,
                   menuIndex,
                   optionsNum);
    setView(view);
}

static Menu rtcMenu = {
    STRING_DATE_AND_TIME,
    &rtcMenuState,
    onRTCMenuGetOption,
    onRTCMenuSelect,
    onSettingsSubMenuBack,
};

View rtcMenuView = {
    onMenuEvent,
    &rtcMenu,
};
