/*
 * Rad Pro
 * Event handler
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "battery.h"
#include "buzzer.h"
#include "display.h"
#include "events.h"
#include "flash.h"
#include "game.h"
#include "gm.h"
#include "keyboard.h"
#include "measurements.h"
#include "menu.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"

#define LIFE_STATE_UPDATE_TIME (60 * 60)

static const uint16_t dataLoggingUpdateTime[] = {
    60 * 60,
    30 * 60,
    10 * 60,
    5 * 60,
    1 * 60,
};

static const char *const dataLoggingMenuOptions[] = {
    "Every 60 minutes",
    "Every 30 minutes",
    "Every 10 minutes",
    "Every 5 minutes",
    "Every minute",
    NULL,
};

static struct
{
    volatile uint32_t tick;

    volatile int32_t backlightTimer;
    volatile int32_t buzzerTimer;
    int32_t keyboardTimer;

    int32_t oneSecondTimer;
    volatile uint8_t oneSecondUpdate;
    uint8_t lastOneSecondUpdate;

    uint32_t dataLoggingLastTimestamp;

    bool measurementsEnabled;
} events;

static const struct Menu pulseClicksMenu;
static const struct Menu backlightMenu;
static const struct Menu dataLoggingMenu;

static void writeDataLogEntryWithTimestamp(uint32_t timestamp);

void initEvents(void)
{
    initEventsHardware();

    events.oneSecondTimer = SYS_TICK_FREQUENCY;
}

void updateEventsMenus(void)
{
    selectMenuIndex(&pulseClicksMenu, settings.pulseClicks);
    selectMenuIndex(&backlightMenu, settings.backlight);
    selectMenuIndex(&dataLoggingMenu, settings.dataLogging);
}

void enableMeasurements(bool value)
{
    events.measurementsEnabled = value;
}

static bool isTimerElapsed(volatile int32_t *timer)
{
    int32_t timerValue = *timer;

    if (timerValue > 0)
    {
        timerValue--;
        *timer = timerValue;

        return (timerValue == 0);
    }

    return false;
}

void onTick(void)
{
    // Backlight
    if (isTimerElapsed(&events.backlightTimer))
        setBacklight(false);
    else if (events.backlightTimer != 0)
        setBacklight(!getBacklight());

    // Buzzer
    if (isTimerElapsed(&events.buzzerTimer))
        setBuzzer(false);

    // Keyboard
    if (isTimerElapsed(&events.keyboardTimer))
    {
        events.keyboardTimer = KEY_TICKS;

        onKeyboardTick();
    }

    // Measurement
    uint32_t pulseNum = 0;
    uint32_t pulseTime;
    while (getGMPulse(&pulseTime))
    {
        onRNGPulse(pulseTime);

        pulseNum++;
    }

    if (events.measurementsEnabled)
    {
        onMeasurementTick(pulseNum);

        // One second timer
        if (isTimerElapsed(&events.oneSecondTimer))
        {
            events.oneSecondTimer = SYS_TICK_FREQUENCY;
            events.oneSecondUpdate++;

            onMeasurementsOneSecond();
        }
    }
}

void syncTimerThread(void)
{
    sleep(1);
}

void updateEvents(void)
{
    uint8_t oneSecondUpdate = events.oneSecondUpdate;
    if (events.lastOneSecondUpdate != oneSecondUpdate)
    {
        events.lastOneSecondUpdate = oneSecondUpdate;

        updateBattery();
        updateMeasurements();
        updateGameTimer();
        refreshView();

        uint32_t timestamp = getRTCTimestamp();
        if ((timestamp - events.dataLoggingLastTimestamp) >= dataLoggingUpdateTime[settings.dataLogging])
            writeDataLogEntryWithTimestamp(timestamp);
    }
}

// Backlight timer

void setBacklightTimer(int32_t value)
{
    if (settings.backlight == BACKLIGHT_ON)
        return;

    if ((value < 0) || (value > events.backlightTimer))
        events.backlightTimer = value;
}

void triggerBacklight(void)
{
    syncTimerThread();

    switch (settings.backlight)
    {
    case BACKLIGHT_OFF:
    case BACKLIGHT_PULSE_FLASHES:
        events.backlightTimer = 1;

        break;

    case BACKLIGHT_10S:
        events.backlightTimer = 10 * SYS_TICK_FREQUENCY;

        break;

    case BACKLIGHT_60S:
        events.backlightTimer = 60 * SYS_TICK_FREQUENCY;

        break;

    case BACKLIGHT_ON:
        events.backlightTimer = -1;

        break;
    }
}

void stopBacklightTimer(void)
{
    syncTimerThread();

    events.backlightTimer = 1;
}

// Buzzer timer

void setBuzzerTimer(int32_t value)
{
    if ((value < 0) || (value > events.buzzerTimer))
    {
        events.buzzerTimer = value;

        setBuzzer(true);
    }
}

void stopBuzzerTimer(void)
{
    syncTimerThread();

    events.buzzerTimer = 1;
}

// Keyboard timer

void startKeyboardTimer(void)
{
    syncTimerThread();

    events.keyboardTimer = KEY_TICKS;
}

// Data logging

void resetDataLogging(void)
{
    events.dataLoggingLastTimestamp = getRTCTimestamp();
}

static void writeDataLogEntryWithTimestamp(uint32_t timestamp)
{
    events.dataLoggingLastTimestamp = timestamp;

    struct DataLogEntry dataLogEntry = {
        timestamp,
        settings.lifePulseCount,
    };

    flashEntry(flashDataLogStart, flashDataLogEnd, sizeof(struct DataLogEntry), (uint32_t *)&dataLogEntry);
}

void writeDataLogEntry(void)
{
    writeDataLogEntryWithTimestamp(getRTCTimestamp());
}

// Pulse clicks menu

static void onPulseClicksMenuSelect(const struct Menu *menu)
{
    settings.pulseClicks = menu->state->selectedIndex;
}

static const char *const pulseClicksMenuOptions[] = {
    "Off",
    "Quiet",
    "Loud",
    NULL,
};

static struct MenuState pulseClicksMenuState;

static const struct Menu pulseClicksMenu = {
    "Pulse clicks",
    &pulseClicksMenuState,
    onMenuGetOption,
    pulseClicksMenuOptions,
    onPulseClicksMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View pulseClicksMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &pulseClicksMenu,
};

// Backlight menu

static void onBacklightMenuSelect(const struct Menu *menu)
{
    settings.backlight = menu->state->selectedIndex;

    triggerBacklight();
}

static const char *const backlightMenuOptions[] = {
    "Off",
    "On for 10 seconds",
    "On for 60 seconds",
    "Pulse flashes",
    "Always on",
    NULL,
};

static struct MenuState backlightMenuState;

static const struct Menu backlightMenu = {
    "Backlight",
    &backlightMenuState,
    onMenuGetOption,
    backlightMenuOptions,
    onBacklightMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View backlightMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &backlightMenu,
};

// Data logging menu

static void onDataLoggingMenuSelect(const struct Menu *menu)
{
    settings.dataLogging = menu->state->selectedIndex;

    events.dataLoggingLastTimestamp = getRTCTimestamp(); // Reset data logging
}

static struct MenuState dataLoggingMenuState;

static const struct Menu dataLoggingMenu = {
    "Data logging",
    &dataLoggingMenuState,
    onMenuGetOption,
    dataLoggingMenuOptions,
    onDataLoggingMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View dataLoggingMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &dataLoggingMenu,
};
