/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdbool.h>
#if defined(SIMULATOR)
#include <time.h>
#endif

#include "comm.h"
#include "cstring.h"
#include "datalog.h"
#include "display.h"
#include "flash.h"
#include "game.h"
#include "measurements.h"
#include "power.h"
#include "pulseled.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

typedef struct
{
    Settings settings;

    Dose dose;
    Dose tube;
} FlashSettings;

Settings settings;

static bool getFlashSettings(FlashIterator *iterator,
                             FlashSettings *flashSettings);

void initSettings(void)
{
    // Default values
    settings.tubeConversionFactor = TUBE_CONVERSIONFACTOR_DEFAULT;

#if defined(PULSE_LED)
    settings.pulseLED = PULSE_LED_ON;
#endif
    settings.pulseClicks = PULSE_CLICKS_CLICKS;
#if defined(DISPLAY_MONOCHROME)
    settings.displayContrast = DISPLAY_CONTRAST_DEFAULT;
#endif
#if defined(SIMULATOR)
    settings.displayBrightness = DISPLAY_BRIGHTNESS_VERYHIGH;
#else
    settings.displayBrightness = DISPLAY_BRIGHTNESS_HIGH;
#endif
    settings.displaySleep = DISPLAY_SLEEP_30S;
#if defined(SIMULATOR)
    time_t unixTime = time(NULL);
    struct tm *localTM = gmtime(&unixTime);
    time_t localTime = mktime(localTM);
    settings.rtcTimeZone = 12 + (unixTime - localTime) / 3600;
#else
    settings.rtcTimeZone = RTC_TIMEZONE_P0000;
#endif

    // Read settings
    FlashIterator iterator;
    iterator.region = &flashSettingsRegion;

    FlashSettings flashSettings;

    if (getFlashSettings(&iterator, &flashSettings))
    {
        settings = flashSettings.settings;

        if (settings.tubeHVFrequency >= TUBE_HVFREQUENCY_NUM)
            settings.tubeHVFrequency = TUBE_HVFREQUENCY_NUM - 1;
        if (settings.tubeHVDutyCycle >= TUBE_HVDUTYCYCLE_NUM)
            settings.tubeHVDutyCycle = TUBE_HVDUTYCYCLE_NUM - 1;

        setDoseTime(flashSettings.dose.time);
        setDosePulseCount(flashSettings.dose.pulseCount);
        setTubeTime(flashSettings.tube.time);
        setTubePulseCount(flashSettings.tube.pulseCount);
    }
}

static bool isFlashSettingsEmpty(FlashSettings *entry)
{
    uint8_t *data = (uint8_t *)entry;

    for (uint32_t i = 0;
         i < sizeof(FlashSettings);
         i++)
    {
        if (data[i] != 0xff)
            return false;
    }

    return true;
}

static bool getFlashSettings(FlashIterator *iterator,
                             FlashSettings *flashSettings)
{
    setFlashPageHead(iterator);

    bool entryValid = false;

    while (iterator->index <= (flashPageDataSize - sizeof(FlashSettings)))
    {
        FlashSettings entry;

        readFlash(iterator,
                  (uint8_t *)&entry,
                  sizeof(FlashSettings));

        if (isFlashSettingsEmpty(&entry))
        {
            iterator->index -= sizeof(FlashSettings);

            break;
        }

        memcpy(flashSettings,
               &entry,
               sizeof(FlashSettings));

        entryValid = true;
    }

    return entryValid;
}

void writeSettings(void)
{
    FlashIterator iterator;
    iterator.region = &flashSettingsRegion;

    FlashSettings flashSettings;

    getFlashSettings(&iterator, &flashSettings);

    flashSettings.settings = settings;
    flashSettings.dose.time = getDoseTime();
    flashSettings.dose.pulseCount = getDosePulseCount();
    flashSettings.tube.time = getTubeTime();
    flashSettings.tube.pulseCount = getTubePulseCount();

    writeFlashPage(&iterator,
                   (uint8_t *)&flashSettings,
                   sizeof(FlashSettings));
}

// Settings Menu

static const OptionView settingsMenuOptions[] = {
    {"Units", &unitsMenuView},
    {"Averaging", &averagingMenuView},
    {"Rate alarm", &rateAlarmMenuView},
    {"Dose alarm", &doseAlarmMenuView},
    {"Geiger tube", &tubeMenuView},
    {"Data logging", &datalogMenuView},
    {"Pulses", &pulsesMenuView},
    {"Display", &displayMenuView},
    {"Date and time", &dateAndTimeMenuView},
#if defined(BATTERY_REMOVABLE)
    {"Battery type", &batteryTypeMenuView},
#endif
#if !defined(DISPLAY_240X320)
    {"Random generator", &rngMenuView},
#else
    {"Random gen.", &rngMenuView},
#endif
#if defined(GAME)
    {"Game", &gameMenuView},
#endif
    {"Statistics", &statisticsView},
#if defined(DATA_MODE)
    {"Data mode", &dataModeMenuView},
#endif
    {NULL},
};

static const char *onSettingsMenuGetOption(const Menu *menu,
                                           uint32_t index,
                                           MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return settingsMenuOptions[index].option;
}

static void onSettingsMenuSelect(const Menu *menu)
{
    setView(settingsMenuOptions[menu->state->selectedIndex].view);
}

static void onSettingsMenuBack(const Menu *menu)
{
    setMeasurementView(-1);
}

void onSettingsSubMenuBack(const Menu *menu)
{
    setView(&settingsMenuView);
}

static MenuState settingsMenuState;

static const Menu settingsMenu = {
    "Settings",
    &settingsMenuState,
    onSettingsMenuGetOption,
    onSettingsMenuSelect,
    onSettingsMenuBack,
};

const View settingsMenuView = {
    onMenuEvent,
    &settingsMenu,
};
