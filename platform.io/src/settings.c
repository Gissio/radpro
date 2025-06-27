/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2025 Gissio
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
#include "events.h"
#include "flash.h"
#include "game.h"
#include "led.h"
#include "measurements.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

static const Menu settingsMenu;

typedef struct
{
    Dose tube;
    Dose dose;
    Settings settings;
} FlashState;

Settings settings;

static FlashState *getFlashState(FlashIterator *iterator);

void initSettings(void)
{
    // Default values
#if defined(PULSE_CONTROL)
    settings.pulseSound = true;
#endif
#if defined(BUZZER)
    settings.pulseSound = PULSE_SOUND_ON_CLICKS;
#endif
#if defined(PULSE_LED)
    settings.pulseLED = true;
#endif

    settings.secondaryUnits = UNITS_CPM;

    settings.alarmIndication =
#if defined(BUZZER) || defined(VOICE)
        (1 << ALARMINDICATION_SOUND) |
#endif
#if defined(VOICE)
        (1 << ALARMINDICATION_VOICE) |
#endif
#if defined(VIBRATION)
        (1 << ALARMINDICATION_VIBRATION) |
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_CONTROL)
        (1 << ALARMINDICATION_PULSE_LED) |
#endif
        (1 << ALARMINDICATION_DISPLAY_FLASH);
#if defined(VOICE)
    settings.alarmVolume = ALARM_VOLUME_VERYHIGH;
#endif

    settings.tubeSensitivity = TUBE_SENSITIVITY_DEFAULT;

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

    // Read flash state
    FlashIterator iterator;
    FlashState *flashState = getFlashState(&iterator);

    if (flashState)
    {
        setCumulativeDoseTime(flashState->dose.time);
        setCumulativeDosePulseCount(flashState->dose.pulseCount);
        setTubeTime(flashState->tube.time);
        setTubePulseCount(flashState->tube.pulseCount);
        settings = flashState->settings;

        // Validate settings
        if (settings.averaging >= AVERAGING_NUM)
            settings.averaging = AVERAGING_UNLIMITED;
        if (settings.instantaneousAveraging >= INSTANTANEOUSAVERAGING_NUM)
            settings.instantaneousAveraging = INSTANTANEOUSAVERAGING_ADAPTIVEFAST;
        if (settings.tubeSensitivity >= TUBE_SENSITIVITY_NUM)
            settings.tubeSensitivity = TUBE_SENSITIVITY_DEFAULT;
#if defined(TUBE_HV_PWM)
        if ((settings.tubeHVFrequency >= TUBE_HVFREQUENCY_NUM) ||
            (settings.tubeHVDutyCycle >= TUBE_HVDUTYCYCLE_NUM))
        {
            settings.tubeHVFrequency = TUBE_HVFREQUENCY_1_25;
            settings.tubeHVDutyCycle = 0;
        }
#endif
        if (settings.datalogInterval >= DATALOG_INTERVAL_NUM)
            settings.datalogInterval = DATALOG_INTERVAL_OFF;
#if defined(DISPLAY_COLOR)
        if (settings.displayTheme >= DISPLAY_THEME_NUM)
            settings.displayTheme = DISPLAY_THEME_DAY;
#endif
        if (settings.displaySleep >= DISPLAY_SLEEP_NUM)
            settings.displaySleep = DISPLAY_SLEEP_30S;
        if (settings.rtcTimeZone >= RTC_TIMEZONE_NUM)
            settings.rtcTimeZone = RTC_TIMEZONE_P0000;
    }
}

void resetSettings(void)
{
    selectMenuItem(&settingsMenu,
                   0,
                   0);
}

static FlashState *getFlashState(FlashIterator *iterator)
{
    iterator->region = &flashSettingsRegion;
    setFlashPageHead(iterator);

    uint8_t *entry = getFlashPage(iterator->pageIndex);
    FlashState *flashState = NULL;

    for (iterator->index = 0;
         iterator->index <= (flashPageDataSize - sizeof(FlashState));
         iterator->index += sizeof(FlashState))
    {
        if (isFlashEmpty(entry, sizeof(FlashState)))
            break;

        flashState = (FlashState *)entry;

        entry += sizeof(FlashState);
    }

    return flashState;
}

void writeSettings(void)
{
    FlashIterator iterator;
    getFlashState(&iterator);

    FlashState flashState;
    flashState.dose.time = getCumulativeDoseTime();
    flashState.dose.pulseCount = getCumulativeDosePulseCount();
    flashState.tube.time = getTubeTime();
    flashState.tube.pulseCount = getTubePulseCount();
    flashState.settings = settings;

    writeFlashPage(&iterator,
                   (uint8_t *)&flashState,
                   sizeof(FlashState));
}

// Settings menu

static const OptionView settingsMenuOptions[] = {
    {getString(STRING_PULSES), &pulsesMenuView},
    {getString(STRING_ALARMS), &alarmsMenuView},
    {getString(STRING_MEASUREMENTS), &measurementsMenuView},
    {getString(STRING_GEIGER_TUBE), &tubeMenuView},
    {getString(STRING_DATA_LOG), &datalogMenuView},
    {getString(STRING_DISPLAY), &displayMenuView},
    {getString(STRING_DATE_AND_TIME), &rtcMenuView},
#if defined(BATTERY_REMOVABLE)
    {getString(STRING_BATTERY_TYPE), &batteryTypeMenuView},
#endif
    {getString(STRING_RANDOM_GENERATOR), &rngMenuView},
#if defined(GAME)
    {getString(STRING_GAME), &gameMenuView},
#endif
    {getString(STRING_STATISTICS), &statisticsView},
#if defined(DATA_MODE)
    {getString(STRING_DATA_MODE), NULL},
#endif
    {NULL},
};

static const char *onSettingsMenuGetOption(const Menu *menu,
                                           uint32_t index,
                                           MenuStyle *menuStyle)
{
#if !defined(DATA_MODE)
    *menuStyle = MENUSTYLE_SUBMENU;
#else
    if (index < ((sizeof(settingsMenuOptions) / sizeof(OptionView)) - 2))
        *menuStyle = MENUSTYLE_SUBMENU;
    else
        *menuStyle = settings.dataMode;
#endif

    return settingsMenuOptions[index].option;
}

static void onSettingsMenuSelect(const Menu *menu)
{
#if !defined(DATA_MODE)
    setView(settingsMenuOptions[menu->state->selectedIndex].view);
#else
    uint32_t selectedIndex = menu->state->selectedIndex;

    if (selectedIndex < ((sizeof(settingsMenuOptions) / sizeof(OptionView)) - 2))
        setView(settingsMenuOptions[selectedIndex].view);
    else
    {
        settings.dataMode = !settings.dataMode;
        if (settings.dataMode)
            openComm();
        else
            closeComm();
    }
#endif
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
    getString(STRING_SETTINGS),
    &settingsMenuState,
    onSettingsMenuGetOption,
    onSettingsMenuSelect,
    onSettingsMenuBack,
};

const View settingsMenuView = {
    onMenuEvent,
    &settingsMenu,
};
