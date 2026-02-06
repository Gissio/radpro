/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdbool.h>
#if defined(SIMULATOR)
#include <time.h>
#endif

#include "../devices/display.h"
#include "../devices/flash.h"
#include "../devices/rtc.h"
#include "../devices/sound.h"
#include "../devices/tube.h"
#include "../extras/game.h"
#include "../extras/rng.h"
#include "../measurements/cumulative.h"
#include "../measurements/datalog.h"
#include "../measurements/measurements.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../system/statistics.h"
#include "../ui/menu.h"

static Menu settingsMenu;

// Settings

Settings settings;

const Settings defaultSettings = {
    .pulseSound = true,
    .pulseLED = true,

    .secondaryDoseUnits = DOSE_UNITS_CPM,

    .rateWarning = RATE_1_USVH,
    .rateAlarm = RATE_10_USVH,
    .alertSound = true,
    .alertVoice = true,
    .alertVibration = true,
    .alertPulseLED = true,
    .alertDisplayFlash = true,

    .tubeType = TUBE_TYPE_DEFAULT,

    .loggingMode = DATALOG_LOGGINGMODE_10_MINUTES,

    .displayContrast = DISPLAY_CONTRAST_DEFAULT,
#if defined(SIMULATOR)
    .displayBrightness = DISPLAY_BRIGHTNESS_VERYHIGH,
#else
    .displayBrightness = DISPLAY_BRIGHTNESS_HIGH,
#endif
    .displaySleep = DISPLAY_SLEEP_30_SECONDS,

#if defined(VOICE)
    .soundAlertStyle = SOUND_ALERTSTYLE_LONG,
#endif
    .soundAlertVolume = SOUND_ALERTVOLUME_VERYHIGH,
    .soundVoiceVolume = SOUND_VOICEVOLUME_VERYHIGH,

    .rtcTimeZone = RTC_TIMEZONE_P0000,
};

// State

typedef struct
{
    Dose tube;
    Dose dose;
    Settings settings;
} State;

static uint32_t stateOffset;

static bool isStateEmpty(const State *state)
{
    const uint8_t *p = (const uint8_t *)state;

    for (uint32_t i = 0; i < sizeof(State); i++)
    {
        if (p[i] != 0xff)
            return false;
    }

    return true;
}

static State *loadState(void)
{
    const uint8_t *statePage = readFlash(STATE_BASE, STATE_SIZE);
    State *lastState = NULL;

    for (uint32_t offset = 0; offset < STATE_SIZE; offset += sizeof(State))
    {
        State *state = (State *)(statePage + offset);

        if (!isStateEmpty(state))
        {
            lastState = state;

            stateOffset = offset + sizeof(State);
        }
    }

    return lastState;
}

static void saveState(State *state)
{
    if (stateOffset >= STATE_SIZE)
    {
        eraseFlash(STATE_BASE);

        stateOffset = 0;
    }

    writeFlash(STATE_BASE + stateOffset, (uint8_t *)state, sizeof(State));

    stateOffset += sizeof(State);
}

static bool validateState(const State *state)
{
    if (!state)
        return false;

    const Settings *s = &state->settings;

    if (s->empty)
        return false;

    return (!s->empty &&
            (s->source < SOURCE_NUM) &&
            (s->instantaneousAveraging < INSTANTANEOUSAVERAGING_NUM) &&
            (s->averaging < AVERAGING_NUM) &&
            (s->tubeType < TUBE_TYPE_NUM) &&
            (s->tubeSensitivity < TUBE_SENSITIVITY_NUM) &&
            (s->tubeDeadTimeCompensation < TUBE_DEADTIMECOMPENSATION_NUM) &&
#if defined(TUBE_HV_PWM)
            (s->tubeHVProfile < TUBE_HVPROFILE_NUM) &&
            (s->tubeHVFrequency < TUBE_HVFREQUENCY_NUM) &&
            (s->tubeHVDutyCycle < TUBE_HVDUTYCYCLE_NUM) &&
#endif
            (s->loggingMode < DATALOG_LOGGINGMODE_NUM) &&
#if defined(DISPLAY_COLOR)
            (s->displayTheme < DISPLAY_THEME_NUM) &&
#endif
            (s->displaySleep < DISPLAY_SLEEP_NUM) &&
            (s->soundPulseType < SOUND_PULSETYPE_NUM) &&
            (s->rtcTimeZone < RTC_TIMEZONE_NUM) &&
#if defined(BATTERY_REMOVABLE)
            (s->powerBatteryType < BATTERYTYPE_NUM) &&
#endif
            true);
}

void initSettings(void)
{
    // Default settings
    settings = defaultSettings;

#if defined(SIMULATOR)
    time_t unixTime = time(NULL);
    struct tm *localTM = gmtime(&unixTime);
    time_t localTime = mktime(localTM);
    settings.rtcTimeZone = 12 + (unixTime - localTime) / 3600;
#endif

    // Load state
    State *state = loadState();

    if (validateState(state))
    {
        setCumulativeDoseTime(state->dose.time);
        setCumulativeDosePulseCount(state->dose.pulseCount);
        setTubeTime(state->tube.time);
        setTubePulseCount(state->tube.pulseCount);

        settings = state->settings;
    }
}

void setupSettings(void)
{
    selectMenuItem(&settingsMenu, 0, 0);
}

void saveSettings(void)
{
    // Save state
    State state;

    state.dose.time = getCumulativeDoseTime();
    state.dose.pulseCount = getCumulativeDosePulseCount();
    state.tube.time = getTubeTime();
    state.tube.pulseCount = getTubePulseCount();

    state.settings = settings;

    saveState(&state);
}

// Settings menu

static MenuOption settingsMenuOptions[] = {
    {STRING_PULSES, &pulsesMenuView},
    {STRING_ALERTS, &alertsMenuView},
    {STRING_MEASUREMENTS, &measurementsMenuView},
    {STRING_GEIGER_TUBE, &tubeMenuView},
    {STRING_DATALOG, &datalogMenuView},
    {STRING_DISPLAY, &displayMenuView},
#if defined(BUZZER) || defined(PULSESOUND_ENABLE) || defined(VOICE)
    {STRING_SOUND, &soundMenuView},
#endif
    {STRING_DATE_AND_TIME, &rtcMenuView},
#if defined(BATTERY_REMOVABLE) || !defined(START_POWERED)
    {STRING_POWER, &powerMenuView},
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

static const char *onSettingsMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
#if !defined(DATA_MODE)
    *menuStyle = MENUSTYLE_SUBMENU;
#else
    if (index < ((sizeof(settingsMenuOptions) / sizeof(MenuOption)) - 2))
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
    if (index < ((sizeof(settingsMenuOptions) / sizeof(MenuOption)) - 2))
        setView(settingsMenuOptions[index].view);
    else
        settings.dataMode = !settings.dataMode;
#endif
}

static MenuState settingsMenuState;

static Menu settingsMenu = {
    STRING_SETTINGS,
    &settingsMenuState,
    onSettingsMenuGetOption,
    onSettingsMenuSelect,
    setMeasurementViewCurrent,
};

View settingsMenuView = {
    onMenuEvent,
    &settingsMenu,
};

void setSettingsMenu(void)
{
    setView(&settingsMenuView);
}
