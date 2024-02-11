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
#if defined(SDLSIM)
#include <time.h>
#endif

#include "buzzer.h"
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
} SettingsDoseTube;

Settings settings;

static SettingsDoseTube *getSettingsDoseTube(FlashIterator *iterator);

void initSettings(void)
{
    // Default values

    settings.tubeConversionFactor = TUBE_CONVERSIONFACTOR_DEFAULT;
    settings.tubeHVFrequency = TUBE_FACTORYDEFAULT_HVFREQUENCY;
    settings.tubeHVDutyCycle = TUBE_FACTORYDEFAULT_HVDUTYCYCLE;

#if defined(PULSE_LED)
    settings.pulseLED = PULSE_LED_ON;
#endif
    settings.pulseClicks = PULSE_CLICKS_QUIET;
#if defined(DISPLAY_MONOCHROME)
    settings.displayContrast = DISPLAY_CONTRAST_DEFAULT;
#endif
    settings.displayTimer = DISPLAY_TIMER_30S;
#if defined(SDLSIM)
    settings.displayBrightness = DISPLAY_BRIGHTNESS_VERYHIGH;
    time_t unixTime = time(NULL);
    struct tm *localTM = gmtime(&unixTime);
    time_t localTime = mktime(localTM);
    settings.rtcTimeZone = 12 + (unixTime - localTime) / 3600;
#else
    settings.displayBrightness = DISPLAY_BRIGHTNESS_HIGH;
    settings.rtcTimeZone = RTC_TIMEZONE_P0000;
#endif

    // Read settings

    FlashIterator iterator;
    iterator.region = &flashSettingsRegion;
    SettingsDoseTube *settingsDoseTube = getSettingsDoseTube(&iterator);

    if (settingsDoseTube)
    {
        settings = settingsDoseTube->settings;
        setDoseTime(settingsDoseTube->dose.time);
        setDosePulseCount(settingsDoseTube->dose.pulseCount);
        setTubeTime(settingsDoseTube->tube.time);
        setTubePulseCount(settingsDoseTube->tube.pulseCount);
    }
}

static SettingsDoseTube *getSettingsDoseTube(FlashIterator *iterator)
{
    setFlashPageHead(iterator);

    uint8_t *page = getFlash(iterator);

    SettingsDoseTube *settingsDoseTube = NULL;

    for (iterator->index = 0;
         iterator->index <= flashPageDataSize - sizeof(SettingsDoseTube);
         iterator->index += sizeof(SettingsDoseTube))
    {
        if (!((SettingsDoseTube *)&page[iterator->index])->settings.entryEmpty)
            settingsDoseTube = (SettingsDoseTube *)&page[iterator->index];
        else
            break;
    }

    return settingsDoseTube;
}

void writeSettings(void)
{
    FlashIterator iterator;

    iterator.region = &flashSettingsRegion;

    getSettingsDoseTube(&iterator);

    SettingsDoseTube settingsDoseTube;

    settingsDoseTube.settings = settings;
    settingsDoseTube.dose.time = getDoseTime();
    settingsDoseTube.dose.pulseCount = getDosePulseCount();
    settingsDoseTube.tube.time = getTubeTime();
    settingsDoseTube.tube.pulseCount = getTubePulseCount();

    programFlashPage(&iterator,
                     (uint8_t *)&settingsDoseTube,
                     sizeof(SettingsDoseTube));
}

// Settings Menu

static const OptionView settingsMenuOptions[] = {
    {"Units", &unitsMenuView},
    {"Average timer", &averageTimerMenuView},
    {"Rate alarm", &rateAlarmMenuView},
    {"Dose alarm", &doseAlarmMenuView},
    {"Data logging", &datalogMenuView},
    {"Geiger tube", &tubeMenuView},
#if defined(PULSE_LED)
    {"Pulse LED", &pulseLEDMenuView},
#endif
    {"Pulse clicks", &pulseClicksMenuView},
    {"Display", &displayMenuView},
    {"Date and time", &dateAndTimeMenuView},
#if defined(BATTERY_REMOVABLE)
    {"Battery type", &batteryTypeMenuView},
#endif
    {"Random generator", &rngMenuView},
    {"Game", &gameMenuView},
    {"Statistics", &statisticsView},
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
