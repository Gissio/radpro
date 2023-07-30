/*
 * Rad Pro
 * Real time clock
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "events.h"
#include "format.h"
#include "rtc.h"
#include "settings.h"

// Based on https://howardhinnant.github.io/date_algorithms.html
uint32_t getTimestampFromDateTime(struct RTCDateTime *dateTime)
{
    uint32_t year = dateTime->year - (dateTime->month <= 2);
    uint32_t era = year / 400;
    uint32_t yearOfEra = year - era * 400;
    uint32_t dayOfYear = (153 * (dateTime->month > 2 ? dateTime->month - 3 : dateTime->month + 9) + 2) / 5 + dateTime->day - 1;
    uint32_t dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;
    uint32_t dayOfEpoch = era * 146097 + dayOfEra - 719468;

    return 86400 * dayOfEpoch + 3600 * dateTime->hour + 60 * dateTime->minute + dateTime->second;
}

void getDateTimeFromTimestamp(uint32_t timestamp, struct RTCDateTime *dateTime)
{
    uint32_t dayOfEpoch = timestamp / 86400 + 719468;
    uint32_t secondsOfDay = timestamp % 86400;

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

static struct MenuState dateAndTimeMenuState;

static struct RTCDateTime currentDateTime;

static void normalizeYear(struct RTCDateTime *dateTime)
{
    if (dateTime->year < RTC_YEAR_MIN)
        dateTime->year = RTC_YEAR_MIN;
    if (dateTime->year > RTC_YEAR_MAX)
        dateTime->year = RTC_YEAR_MAX;
}

static void onRTCMenuSelect(const struct Menu *menu)
{
    struct RTCDateTime dateTime;
    getRTCDateTime(&dateTime);
    normalizeYear(&dateTime);

    switch (dateAndTimeMenuState.selectedIndex)
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
    resetDataLogging();
}

struct RTCMenuOption
{
    uint32_t offset;
    uint32_t maxIndex;
};

struct RTCMenuOption rtcMenuOptions[] = {
    {RTC_YEAR_MIN, 80},
    {1, 12},
    {1, 31},
    {0, 24},
    {0, 60},
};

static const char *onRTCMenuGetOption(const struct Menu *menu, uint32_t index)
{
    uint32_t dateAndTimeIndex = dateAndTimeMenuState.selectedIndex;
    struct RTCMenuOption *rtcMenuOption = &rtcMenuOptions[dateAndTimeIndex];

    uint32_t maxIndex = rtcMenuOption->maxIndex;
    if (dateAndTimeIndex == 2)
        maxIndex = getDaysInMonth(currentDateTime.year, currentDateTime.month);

    if (index >= maxIndex)
        return NULL;

    strcpy(menuOption, "");
    strcatNumber(menuOption, rtcMenuOption->offset + index, 0);

    return menuOption;
}

static void onRTCSubMenuBack(const struct Menu *menu)
{
    setView(&dateAndTimeMenuView);
}

static struct MenuState dateAndTimeItemMenuState;

// Year menu

static const struct Menu rtcYearMenu = {
    "Year",
    &dateAndTimeItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

const struct View rtcYearMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &rtcYearMenu,
};

// Month menu

static const struct Menu rtcMonthMenu = {
    "Month",
    &dateAndTimeItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

const struct View rtcMonthMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &rtcMonthMenu,
};

// Day menu

static const struct Menu rtcDayMenu = {
    "Day",
    &dateAndTimeItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

const struct View rtcDayMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &rtcDayMenu,
};

// Hour menu

static const struct Menu rtcHourMenu = {
    "Hour",
    &dateAndTimeItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

const struct View rtcHourMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &rtcHourMenu,
};

// Minute menu

static const struct Menu rtcMinuteMenu = {
    "Minute",
    &dateAndTimeItemMenuState,
    onRTCMenuGetOption,
    NULL,
    onRTCMenuSelect,
    NULL,
    onRTCSubMenuBack,
};

const struct View rtcMinuteMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &rtcMinuteMenu,
};

// Date and type menu

static void onDateAndTimeMenuEnter(const struct Menu *menu)
{
    getRTCDateTime(&currentDateTime);
    normalizeYear(&currentDateTime);

    switch (dateAndTimeMenuState.selectedIndex)
    {
    case 0:
        selectMenuIndex(&rtcYearMenu, currentDateTime.year - RTC_YEAR_MIN);
        setView(&rtcYearMenuView);

        break;

    case 1:
        selectMenuIndex(&rtcMonthMenu, currentDateTime.month - 1);
        setView(&rtcMonthMenuView);

        break;

    case 2:
        selectMenuIndex(&rtcDayMenu, currentDateTime.day - 1);
        setView(&rtcDayMenuView);

        break;

    case 3:
        selectMenuIndex(&rtcHourMenu, currentDateTime.hour);
        setView(&rtcHourMenuView);

        break;

    case 4:
        selectMenuIndex(&rtcMinuteMenu, currentDateTime.minute);
        setView(&rtcMinuteMenuView);

        break;
    }
}

const char *const dateAndTimeMenuOptions[] = {
    "Year",
    "Month",
    "Day",
    "Hour",
    "Minute",
    NULL,
};

static const struct Menu dateAndTimeMenu = {
    "Date and time",
    &dateAndTimeMenuState,
    onMenuGetOption,
    dateAndTimeMenuOptions,
    NULL,
    onDateAndTimeMenuEnter,
    onSettingsSubMenuBack,
};

const struct View dateAndTimeMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &dateAndTimeMenu,
};
