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

static const Menu rtcTimeZoneMenu;

void initRTC(void)
{
    initRTCController();

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

static uint8_t daysInMonth[] = {
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

static const char *const rtcMenuOptions[] = {
    "Time zone",
    "Year",
    "Month",
    "Day",
    "Hour",
    "Minute",
    "Time format",
    NULL,
};

static MenuState rtcMenuState;

static RTCDateTime rtcCurrentDateTime;

typedef struct
{
    uint32_t offset;
    uint32_t maxIndex;
} RTCMenuOptionSetting;

static const RTCMenuOptionSetting rtcMenuOptionSettings[] = {
    {RTC_YEAR_MIN, 80},
    {1, 12},
    {1, 31},
    {0, 24},
    {0, 60},
};

static const char *onRTCSubMenuGetOption(const Menu *menu,
                                         uint32_t index,
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

    const RTCMenuOptionSetting *rtcMenuOptionSetting =
        &rtcMenuOptionSettings[rtcMenuState.selectedIndex - 1];

    uint32_t maxIndex = rtcMenuOptionSetting->maxIndex;
    if (rtcMenuState.selectedIndex == 3)
        maxIndex = getDaysInMonth(rtcCurrentDateTime.year, rtcCurrentDateTime.month);

    if (index < maxIndex)
    {
        strclr(menuOption);
        strcatUInt32(menuOption, rtcMenuOptionSetting->offset + index, 0);

        return menuOption;
    }
    else
        return NULL;
}

static void onRTCSubMenuSelect(const Menu *menu)
{
    RTCDateTime dateTime;
    getDeviceDateTime(&dateTime);
    normalizeDateTime(&dateTime);

    switch (rtcMenuState.selectedIndex)
    {
    case DATETIME_YEAR:
        dateTime.year = RTC_YEAR_MIN + menu->state->selectedIndex;

        break;

    case DATETIME_MONTH:
        dateTime.month = 1 + menu->state->selectedIndex;

        break;

    case DATETIME_DAY:
        dateTime.day = 1 + menu->state->selectedIndex;

        break;

    case DATETIME_HOUR:
        dateTime.hour = (uint8_t)menu->state->selectedIndex;

        break;

    case DATETIME_MINUTE:
        dateTime.minute = (uint8_t)menu->state->selectedIndex;
        dateTime.second = 0;

        break;
    }

    setDeviceDateTime(&dateTime);
    rtcCurrentDateTime = dateTime;
}

static void onRTCSubMenuBack(const Menu *menu)
{
    setView(&dateAndTimeMenuView);
}

static MenuState rtcItemMenuState;

// Time zone menu

static const char *onRTCTimeZoneMenuGetOption(const Menu *menu,
                                              uint32_t index,
                                              MenuStyle *menuStyle)
{
    if (index >= RTC_TIMEZONE_NUM)
        return NULL;

    *menuStyle = (index == settings.rtcTimeZone);

    int32_t deltaTimeMinutes = 60 * (index - 12);

    if (deltaTimeMinutes < 0)
    {
        strcpy(menuOption, "UTC-");

        deltaTimeMinutes = -deltaTimeMinutes;
    }
    else
        strcpy(menuOption, "UTC+");

    strcatTime(menuOption, deltaTimeMinutes);

    return menuOption;
}

static void onRTCTimeZoneMenuSelect(const Menu *menu)
{
    settings.rtcTimeZone = menu->state->selectedIndex;
}

static const Menu rtcTimeZoneMenu = {
    "Time zone",
    &rtcItemMenuState,
    onRTCTimeZoneMenuGetOption,
    onRTCTimeZoneMenuSelect,
    onRTCSubMenuBack,
};

static const View rtcTimeZoneMenuView = {
    onMenuEvent,
    &rtcTimeZoneMenu,
};

// Year menu

static const Menu rtcYearMenu = {
    "Year",
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static const View rtcYearMenuView = {
    onMenuEvent,
    &rtcYearMenu,
};

// Month menu

static const Menu rtcMonthMenu = {
    "Month",
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

const View rtcMonthMenuView = {
    onMenuEvent,
    &rtcMonthMenu,
};

// Day menu

static const Menu rtcDayMenu = {
    "Day",
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static const View rtcDayMenuView = {
    onMenuEvent,
    &rtcDayMenu,
};

// Hour menu

static const Menu rtcHourMenu = {
    "Hour",
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static const View rtcHourMenuView = {
    onMenuEvent,
    &rtcHourMenu,
};

// Minute menu

static const Menu rtcMinuteMenu = {
    "Minute",
    &rtcItemMenuState,
    onRTCSubMenuGetOption,
    onRTCSubMenuSelect,
    onRTCSubMenuBack,
};

static const View rtcMinuteMenuView = {
    onMenuEvent,
    &rtcMinuteMenu,
};

// Time format menu

static const char *const rtcTimeFormatMenuOptions[] = {
    "24-hour",
    "12-hour",
    NULL,
};

static const char *onRTCTimeFormatMenuGetOption(const Menu *menu,
                                                uint32_t index,
                                                MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.rtcTimeFormat);

    return rtcTimeFormatMenuOptions[index];
}

static void onRTCTimeFormatMenuSelect(const Menu *menu)
{
    settings.rtcTimeFormat = menu->state->selectedIndex;
}

static const Menu rtcTimeFormatMenu = {
    "Time format",
    &rtcItemMenuState,
    onRTCTimeFormatMenuGetOption,
    onRTCTimeFormatMenuSelect,
    onRTCSubMenuBack,
};

static const View rtcTimeFormatMenuView = {
    onMenuEvent,
    &rtcTimeFormatMenu,
};

// Date and time menu

static const char *onRTCMenuGetOption(const Menu *menu,
                                      uint32_t index,
                                      MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return rtcMenuOptions[index];
}

static void onRTCMenuSelect(const Menu *menu)
{
    getDeviceDateTime(&rtcCurrentDateTime);
    normalizeDateTime(&rtcCurrentDateTime);

    const View *view = NULL;
    uint32_t menuIndex = 0;
    uint32_t optionsNum;

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

    selectMenuItem((const Menu *)view->userdata,
                   menuIndex,
                   optionsNum);
    setView(view);
}

static const Menu rtcMenu = {
    "Date and time",
    &rtcMenuState,
    onRTCMenuGetOption,
    onRTCMenuSelect,
    onSettingsSubMenuBack,
};

const View dateAndTimeMenuView = {
    onMenuEvent,
    &rtcMenu,
};
