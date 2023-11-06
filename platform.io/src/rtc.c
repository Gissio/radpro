/*
 * Rad Pro
 * Real-time clock
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "cstring.h"
#include "menu.h"
#include "rtc.h"
#include "settings.h"

// Based on https://howardhinnant.github.io/date_algorithms.html

uint32_t getTimeFromDateTime(struct RTCDateTime *dateTime)
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

void getDateTimeFromTime(uint32_t time, struct RTCDateTime *dateTime)
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

// RTC menu common

static struct MenuState rtcMenuState;

static struct RTCDateTime rtcCurrentDateTime;

static void normalizeYear(struct RTCDateTime *dateTime)
{
    if (dateTime->year < RTC_YEAR_MIN)
        dateTime->year = RTC_YEAR_MIN;
    if (dateTime->year > RTC_YEAR_MAX)
        dateTime->year = RTC_YEAR_MAX;
}

struct RTCMenuOptionSetting
{
    uint32_t offset;
    uint32_t maxIndex;
};

static const struct RTCMenuOptionSetting rtcMenuOptionSettings[] = {
    {RTC_YEAR_MIN, 80},
    {1, 12},
    {1, 31},
    {0, 24},
    {0, 60},
};

static const char *onRTCMenuGetOption(const struct Menu *menu, uint32_t index)
{
    uint32_t dateAndTimeIndex = rtcMenuState.selectedIndex;
    const struct RTCMenuOptionSetting *rtcMenuOptionSetting =
        &rtcMenuOptionSettings[dateAndTimeIndex];

    uint32_t maxIndex = rtcMenuOptionSetting->maxIndex;
    if (dateAndTimeIndex == 2)
        maxIndex = getDaysInMonth(rtcCurrentDateTime.year, rtcCurrentDateTime.month);

    if (index < maxIndex)
    {
        strcpy(menuOption, "");
        strcatUInt32(menuOption, rtcMenuOptionSetting->offset + index, 0);

        return menuOption;
    }
    else
        return NULL;
}

static void onRTCMenuSelect(const struct Menu *menu)
{
    struct RTCDateTime dateTime;
    getRTCDateTime(&dateTime);
    normalizeYear(&dateTime);

    switch (rtcMenuState.selectedIndex)
    {
    case 0:
        dateTime.year = RTC_YEAR_MIN + menu->state->selectedIndex;

        break;

    case 1:
        dateTime.month = 1 + menu->state->selectedIndex;

        break;

    case 2:
        dateTime.day = 1 + menu->state->selectedIndex;

        break;

    case 3:
        dateTime.hour = (uint8_t)menu->state->selectedIndex;

        break;

    case 4:
        dateTime.minute = (uint8_t)menu->state->selectedIndex;
        dateTime.second = 0;

        break;
    }

    setRTCDateTime(&dateTime);
}

static void onRTCSubMenuBack(const struct Menu *menu)
{
    setView(&dateAndTimeMenuView);
}

static struct MenuState rtcItemMenuState;

// Year menu

static const struct Menu rtcYearMenu = {
    "Year",
    &rtcItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

static const struct View rtcYearMenuView = {
    onMenuEvent,
    &rtcYearMenu,
};

// Month menu

static const struct Menu rtcMonthMenu = {
    "Month",
    &rtcItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

const struct View rtcMonthMenuView = {
    onMenuEvent,
    &rtcMonthMenu,
};

// Day menu

static const struct Menu rtcDayMenu = {
    "Day",
    &rtcItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

static const struct View rtcDayMenuView = {
    onMenuEvent,
    &rtcDayMenu,
};

// Hour menu

static const struct Menu rtcHourMenu = {
    "Hour",
    &rtcItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

static const struct View rtcHourMenuView = {
    onMenuEvent,
    &rtcHourMenu,
};

// Minute menu

static const struct Menu rtcMinuteMenu = {
    "Minute",
    &rtcItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

static const struct View rtcMinuteMenuView = {
    onMenuEvent,
    &rtcMinuteMenu,
};

// Date and type menu

static void onRTCMenuEnter(const struct Menu *menu)
{
    getRTCDateTime(&rtcCurrentDateTime);
    normalizeYear(&rtcCurrentDateTime);

    const struct View *view = NULL;
    uint32_t menuIndex = 0;
    uint32_t optionsNum;

    switch (rtcMenuState.selectedIndex)
    {
    case 0:
        view = &rtcYearMenuView;
        menuIndex = rtcCurrentDateTime.year - RTC_YEAR_MIN;
        optionsNum = RTC_YEAR_NUM;

        break;

    case 1:
        view = &rtcMonthMenuView;
        menuIndex = rtcCurrentDateTime.month - 1;
        optionsNum = 12;

        break;

    case 2:
        view = &rtcDayMenuView;
        menuIndex = rtcCurrentDateTime.day - 1;
        optionsNum = getDaysInMonth(rtcCurrentDateTime.year, rtcCurrentDateTime.month);

        break;

    case 3:
        view = &rtcHourMenuView;
        menuIndex = rtcCurrentDateTime.hour;
        optionsNum = 24;

        break;

    case 4:
        view = &rtcMinuteMenuView;
        menuIndex = rtcCurrentDateTime.minute;
        optionsNum = 60;

        break;
    }

    selectMenuIndex((const struct Menu *)view->userdata, menuIndex, optionsNum);
    setView(view);
}

static const char *const rtcMenuOptions[] = {
    "Year",
    "Month",
    "Day",
    "Hour",
    "Minute",
    NULL,
};

static const struct Menu rtcMenu = {
    "Date and time",
    &rtcMenuState,
    onMenuGetOption,
    rtcMenuOptions,
    NULL,
    onRTCMenuEnter,
    onSettingsSubMenuBack,
};

const struct View dateAndTimeMenuView = {
    onMenuEvent,
    &rtcMenu,
};
