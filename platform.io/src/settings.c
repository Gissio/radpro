/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdbool.h>

#include "buzzer.h"
#include "datalog.h"
#include "display.h"
#include "flash.h"
#include "game.h"
#include "measurements.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

struct FlashSettings
{
    struct Settings settings;

    struct Dose dose;
    struct Dose tube;
};

struct SettingsState
{
    uint8_t pageIndex;
    uint32_t index;
};

struct Settings settings;

static struct FlashSettings *getSettingsState(struct SettingsState *settingsState);

void initSettings(void)
{
    settings.pulseClicks = PULSE_CLICKS_QUIET;

#if defined(PULSE_LED)

    settings.pulseLED = PULSE_LED_ON;

#endif

#if defined(DISPLAY_MONO)

    settings.displayBacklight = DISPLAY_BACKLIGHT_30S;

#elif defined(DISPLAY_COLOR)

    settings.displayBrightnessLevel = DISPLAY_BRIGHTNESS_HIGH;
    settings.displaySleep = DISPLAY_SLEEP_30S;

#endif

    struct FlashSettings *settingsEntry = getSettingsState(NULL);
    if (settingsEntry)
    {
        settings = settingsEntry->settings;

        setDoseTime(settingsEntry->dose.time);
        setDosePulseCount(settingsEntry->dose.pulseCount);

        setTubeTime(settingsEntry->tube.time);
        setTubePulseCount(settingsEntry->tube.pulseCount);
    }
}

static struct FlashSettings *getSettingsState(struct SettingsState *settingsState)
{
    uint8_t pageIndex = getFlashHeadPage(&flashSettingsRegion);
    uint8_t *page = getFlashData(pageIndex);

    if (settingsState)
        settingsState->pageIndex = pageIndex;

    struct FlashSettings *settingsEntry = NULL;
    for (uint32_t index = 0;
         index < flashDataSize;
         index += sizeof(struct FlashSettings))
    {
        if (settingsState)
            settingsState->index = index;

        if (page[index] == 0xff)
            break;

        settingsEntry = (struct FlashSettings *)&page[index];
    }

    return settingsEntry;
}

void writeSettings(void)
{
    struct SettingsState settingsState;
    getSettingsState(&settingsState);

    struct FlashSettings settingsEntry;
    settingsEntry.settings = settings;
    settingsEntry.dose.time = getDoseTime();
    settingsEntry.dose.pulseCount = getDosePulseCount();
    settingsEntry.tube.time = getTubeTime();
    settingsEntry.tube.pulseCount = getTubePulseCount();

    flashEntry(&flashSettingsRegion,
               settingsState.pageIndex, settingsState.index,
               (uint8_t *)&settingsEntry, sizeof(struct FlashSettings));
}

// Settings Menu

static const char *const settingsMenuOptions[] = {
    "Units",
    "Average timer",
    "Rate alarm",
    "Dose alarm",
    "Pulse clicks",

#if defined(PULSE_LED)

    "Pulse LED",

#endif

#if defined(DISPLAY_MONO)

    "Backlight",

#elif defined(DISPLAY_COLOR)

    "Display",

#endif

    "Date and time",
    "Data logging",
    "Geiger tube",

#if defined(BATTERY_REMOVABLE)

    "Battery type",

#endif

    "Random number generator",
    "Statistics",
    "Game",
    NULL,
};

static const struct View *settingsMenuOptionViews[] = {
    &unitsMenuView,
    &averageTimerMenuView,
    &rateAlarmMenuView,
    &doseAlarmMenuView,
    &pulseClicksMenuView,

#if defined(PULSE_LED)

    &pulseLEDMenuView,

#endif

#if defined(DISPLAY_MONO)

    &backlightMenuView,

#elif defined(DISPLAY_COLOR)

    &displayMenuView,

#endif

    &dateAndTimeMenuView,
    &datalogMenuView,
    &tubeMenuView,

#if defined(BATTERY_REMOVABLE)

    &batteryTypeMenuView,

#endif

    &rngMenuView,
    &statisticsView,
    &gameMenuView,
};

static void onSettingsMenuEnter(const struct Menu *menu)
{
    setView(settingsMenuOptionViews[menu->state->selectedIndex]);
}

static void onSettingsMenuBack(const struct Menu *menu)
{
    setMeasurementView(-1);
}

void onSettingsSubMenuBack(const struct Menu *menu)
{
    setView(&settingsMenuView);
}

static struct MenuState settingsMenuState;

static const struct Menu settingsMenu = {
    "Settings",
    &settingsMenuState,
    onMenuGetOption,
    settingsMenuOptions,
    NULL,
    onSettingsMenuEnter,
    onSettingsMenuBack,
};

const struct View settingsMenuView = {
    onMenuEvent,
    &settingsMenu,
};
