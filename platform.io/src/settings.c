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
#include "sound.h"
#include "system.h"
#include "tube.h"

static Menu settingsMenu;

typedef struct
{
    Dose tube;
    Dose dose;
    Settings settings;
} PersistentState;

Settings settings;

const Settings defaultSettings = {
#if defined(BUZZER) || defined(SOUND_EN)
    .pulseSound = true,
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    .pulseLED = true,
#endif

    .secondaryUnits = UNITS_CPM,

    .rateWarning = RATE_1_USVH,
    .rateAlarm = RATE_10_USVH,
    .alertSound = true,
#if defined(VOICE)
    .alertVoice = true,
#endif
#if defined(VIBRATION)
    .alertVibration = true,
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    .alertPulseLED = true,
#endif
    .alertDisplayFlash = true,

    .tubeSensitivity = TUBE_SENSITIVITY_DEFAULT,

    .datalogInterval = DATALOG_INTERVAL_10_MINUTES,

#if defined(DISPLAY_MONOCHROME)
    .displayContrast = DISPLAY_CONTRAST_DEFAULT,
#endif
#if defined(SIMULATOR)
    .displayBrightness = DISPLAY_BRIGHTNESS_VERYHIGH,
#else
    .displayBrightness = DISPLAY_BRIGHTNESS_HIGH,
#endif
    .displaySleep = DISPLAY_SLEEP_30_SECONDS,

#if defined(VOICE)
    .soundAlertStyle = SOUND_ALERTSTYLE_LONG,
    .soundAlertVolume = SOUND_ALERTVOLUME_VERYHIGH,
    .soundVoiceVolume = SOUND_VOICEVOLUME_VERYHIGH,
#endif

    .rtcTimeZone = RTC_TIMEZONE_P0000,
};

static PersistentState *getPersistentState(FlashIterator *iterator);

void initSettings(void)
{
    // Default values
    settings = defaultSettings;

#if defined(SIMULATOR)
    time_t unixTime = time(NULL);
    struct tm *localTM = gmtime(&unixTime);
    time_t localTime = mktime(localTM);
    settings.rtcTimeZone = 12 + (unixTime - localTime) / 3600;
#endif

    // Read persistent state
    FlashIterator iterator;
    PersistentState *persistentState = getPersistentState(&iterator);

    if (persistentState)
    {
        setCumulativeDoseTime(persistentState->dose.time);
        setCumulativeDosePulseCount(persistentState->dose.pulseCount);
        setTubeTime(persistentState->tube.time);
        setTubePulseCount(persistentState->tube.pulseCount);
        settings = persistentState->settings;

        // Validate settings
        if (settings.instantaneousAveraging >= INSTANTANEOUSAVERAGING_NUM)
            settings.instantaneousAveraging = INSTANTANEOUSAVERAGING_ADAPTIVEFAST;
        if (settings.averaging >= AVERAGING_NUM)
            settings.averaging = AVERAGING_UNLIMITED;

        if (settings.tubeSensitivity >= TUBE_SENSITIVITY_NUM)
            settings.tubeSensitivity = TUBE_SENSITIVITY_DEFAULT;
        if (settings.tubeDeadTimeCompensation >= TUBE_DEADTIMECOMPENSATION_NUM)
            settings.tubeDeadTimeCompensation = 0;
#if defined(TUBE_HV_PWM)
        if (settings.tubeHVProfile >= TUBE_HVPROFILE_NUM)
            settings.tubeHVProfile = TUBE_HVPROFILE_FACTORYDEFAULT;
        if ((settings.tubeHVFrequency >= TUBE_HVFREQUENCY_NUM) ||
            (settings.tubeHVDutyCycle >= TUBE_HVDUTYCYCLE_NUM))
        {
            settings.tubeHVFrequency = TUBE_HVFREQUENCY_1_25_KHZ;
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
            settings.displaySleep = DISPLAY_SLEEP_30_SECONDS;

        if (settings.rtcTimeZone >= RTC_TIMEZONE_NUM)
            settings.rtcTimeZone = RTC_TIMEZONE_P0000;

#if !defined(BATTERY_REMOVABLE)
        settings.batteryType = BATTERYTYPE_LI_ION;
#endif
    }
}

void resetSettings(void)
{
    selectMenuItem(&settingsMenu,
                   0,
                   0);
}

static PersistentState *getPersistentState(FlashIterator *iterator)
{
    iterator->region = &flashSettingsRegion;
    setFlashPageHead(iterator);

    uint8_t *persistentState = getFlashPage(iterator->pageIndex);
    PersistentState *lastPersistentState = NULL;

    for (iterator->index = 0;
         iterator->index <= (flashPageDataSize - sizeof(PersistentState));
         iterator->index += sizeof(PersistentState))
    {
        if (isFlashEmpty(persistentState, sizeof(PersistentState)))
            break;

        lastPersistentState = (PersistentState *)persistentState;

        persistentState += sizeof(PersistentState);
    }

    return lastPersistentState;
}

void writeSettings(void)
{
    FlashIterator iterator;
    getPersistentState(&iterator);

    PersistentState persistentState;
    persistentState.dose.time = getCumulativeDoseTime();
    persistentState.dose.pulseCount = getCumulativeDosePulseCount();
    persistentState.tube.time = getTubeTime();
    persistentState.tube.pulseCount = getTubePulseCount();
    persistentState.settings = settings;

    writeFlashPage(&iterator,
                   (uint8_t *)&persistentState,
                   sizeof(PersistentState));
}

// Settings menu

static SubMenuOption settingsMenuOptions[] = {
    {STRING_PULSES, &pulsesMenuView},
    {STRING_ALERTS, &alertsMenuView},
    {STRING_MEASUREMENTS, &measurementsMenuView},
    {STRING_GEIGER_TUBE, &tubeMenuView},
    {STRING_DATALOG, &datalogMenuView},
    {STRING_DISPLAY, &displayMenuView},
#if defined(BUZZER) || defined(VOICE)
    {STRING_SOUND, &soundMenuView},
#endif
    {STRING_DATE_AND_TIME, &rtcMenuView},
#if defined(BATTERY_REMOVABLE)
    {STRING_BATTERY_TYPE, &batteryTypeMenuView},
#endif
    {STRING_RANDOM_GENERATOR, &rngMenuView},
#if defined(GAME)
    {STRING_GAME, &gameMenuView},
#endif
    {STRING_STATISTICS, &statisticsView},
#if defined(DATA_MODE)
    {STRING_DATAMODE, NULL},
#endif
    {NULL},
};

static const char *onSettingsMenuGetOption(uint32_t index,
                                           MenuStyle *menuStyle)
{
#if !defined(DATA_MODE)
    *menuStyle = MENUSTYLE_SUBMENU;
#else
    if (index < ((sizeof(settingsMenuOptions) / sizeof(SubMenuOption)) - 2))
        *menuStyle = MENUSTYLE_SUBMENU;
    else
        *menuStyle = settings.dataMode;
#endif

    return getString(settingsMenuOptions[index].title);
}

static void onSettingsMenuSelect(uint32_t index)
{
#if !defined(DATA_MODE)
    setView(settingsMenuOptions[index].view);
#else
    if (index < ((sizeof(settingsMenuOptions) / sizeof(SubMenuOption)) - 2))
        setView(settingsMenuOptions[index].view);
    else
        settings.dataMode = !settings.dataMode;
#endif
}

static void onSettingsMenuBack(void)
{
    setMeasurementView(-1);
}

void onSettingsSubMenuBack(void)
{
    setView(&settingsMenuView);
}

static MenuState settingsMenuState;

static Menu settingsMenu = {
    STRING_SETTINGS,
    &settingsMenuState,
    onSettingsMenuGetOption,
    onSettingsMenuSelect,
    onSettingsMenuBack,
};

View settingsMenuView = {
    onMenuEvent,
    &settingsMenu,
};
