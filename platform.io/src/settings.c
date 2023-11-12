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

struct SettingsDoseTube
{
    struct Settings settings;

    struct Dose dose;
    struct Dose tube;
};

struct Settings settings;

static struct SettingsDoseTube *getSettingsDoseTube(struct FlashIterator *iterator);

void initSettings(void)
{
    // Default values

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

    settings.tubeHVFrequency = 5;

    // Read settings

    struct FlashIterator iterator;

    iterator.region = &flashSettingsRegion;

    struct SettingsDoseTube *settingsDoseTube = getSettingsDoseTube(&iterator);

    if (settingsDoseTube)
    {
        settings = settingsDoseTube->settings;
        setDoseTime(settingsDoseTube->dose.time);
        setDosePulseCount(settingsDoseTube->dose.pulseCount);
        setTubeTime(settingsDoseTube->tube.time);
        setTubePulseCount(settingsDoseTube->tube.pulseCount);
    }
}

static struct SettingsDoseTube *getSettingsDoseTube(struct FlashIterator *iterator)
{
    setFlashPageHead(iterator);

    uint8_t *page = getFlash(iterator);

    struct SettingsDoseTube *settingsDoseTube = NULL;

    for (iterator->index = 0;
         iterator->index <= flashPageDataSize - sizeof(struct SettingsDoseTube);
         iterator->index += sizeof(struct SettingsDoseTube))
    {
        if (!((struct SettingsDoseTube *)&page[iterator->index])->settings.empty)
            settingsDoseTube = (struct SettingsDoseTube *)&page[iterator->index];
        else
            break;
    }

    return settingsDoseTube;
}

void writeSettings(void)
{
    struct FlashIterator iterator;

    iterator.region = &flashSettingsRegion;

    getSettingsDoseTube(&iterator);

    struct SettingsDoseTube settingsDoseTube;

    settingsDoseTube.settings = settings;
    settingsDoseTube.dose.time = getDoseTime();
    settingsDoseTube.dose.pulseCount = getDosePulseCount();
    settingsDoseTube.tube.time = getTubeTime();
    settingsDoseTube.tube.pulseCount = getTubePulseCount();

    programFlashPage(&iterator,
                     (uint8_t *)&settingsDoseTube,
                     sizeof(struct SettingsDoseTube));
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
