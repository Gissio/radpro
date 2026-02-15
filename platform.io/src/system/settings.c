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

#include "../extras/game.h"
#include "../extras/rng.h"
#include "../measurements/cumulative.h"
#include "../measurements/datalog.h"
#include "../measurements/measurements.h"
#include "../peripherals/display.h"
#include "../peripherals/flash.h"
#include "../peripherals/rtc.h"
#include "../peripherals/sound.h"
#include "../peripherals/tube.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../system/statistics.h"
#include "../ui/menu.h"

static Menu settingsMenu;

// Settings

Settings settings;

static const Settings defaultSettings = {
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

typedef
#if defined(__GNUC__)
    __attribute__((aligned(8)))
#elif defined(_MSC_VER)
    __declspec(align(8))
#endif
    struct
{
    Dose tube;
    Dose dose;
    Settings settings;
} State;

static uint32_t stateOffset;
static bool settingsLoaded;

#define STATES_PAGE_SIZE FLASH_PAGE_SIZE
#define STATES_PAGE_LASTSTATE_OFFSET ((STATES_PAGE_ID_OFFSET / sizeof(State)) * sizeof(State))
#define STATES_PAGE_ID_OFFSET (STATES_PAGE_SIZE - STATES_PAGE_ID_SIZE)
#define STATES_PAGE_ID_SIZE 8

static const uint8_t statesPageId[STATES_PAGE_ID_SIZE] = {
    'R',
    'a',
    'd',
    'P',
    'r',
    'o',
    SETTINGS_VERSION,
    0,
};

static bool validateStatesPage(const uint8_t *statesPage)
{
    return memcmp(statesPage + STATES_PAGE_ID_OFFSET, statesPageId, STATES_PAGE_ID_SIZE) == 0;
}

static void resetStatesPage(void)
{
    eraseFlash(STATES_BASE);
    writeFlash(STATES_BASE + STATES_PAGE_ID_OFFSET, statesPageId, STATES_PAGE_ID_SIZE);

    stateOffset = 0;
}

static bool validateState(const State *state)
{
    const Settings *s = &state->settings;

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

static const State *loadLatestState(void)
{
    const uint8_t *statesPage = readFlash(STATES_BASE, STATES_SIZE);
    const State *lastState = NULL;

    if (validateStatesPage(statesPage))
    {
        // Find last state
        for (uint32_t offset = 0; offset < STATES_PAGE_LASTSTATE_OFFSET; offset += sizeof(State))
        {
            const State *state = (const State *)(statesPage + offset);
            if (validateState(state))
            {
                lastState = state;

                stateOffset = offset + sizeof(State);
            }
        }
    }
    else
        stateOffset = STATES_PAGE_LASTSTATE_OFFSET;

    return lastState;
}

static void appendState(State *state)
{
    // Try twice
    for (uint32_t i = 0; i < 2; i++)
    {
        if ((stateOffset < STATES_PAGE_LASTSTATE_OFFSET) &&
            writeFlash(STATES_BASE + stateOffset, (uint8_t *)state, sizeof(State)))
            break;
        else
            resetStatesPage();
    }

    stateOffset += sizeof(State);
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
    const State *state = loadLatestState();
    if (state)
    {
        setCumulativeDoseTime(state->dose.time);
        setCumulativeDosePulseCount(state->dose.pulseCount);
        setTubeTime(state->tube.time);
        setTubePulseCount(state->tube.pulseCount);

        settings = state->settings;

        settingsLoaded = true;
    }
}

void setupSettings(void)
{
    selectMenuItem(&settingsMenu, 0, 0);
}

bool isSettingsLoaded(void)
{
    return settingsLoaded;
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

    appendState(&state);
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

#define SETTINGS_MENU_COUNT (sizeof(settingsMenuOptions)/sizeof(MenuOption))
#define DATA_MODE_INDEX (SETTINGS_MENU_COUNT - 2)

static const char *onSettingsMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
#if !defined(DATA_MODE)
    *menuStyle = MENUSTYLE_SUBMENU;
#else
    if (index < DATA_MODE_INDEX)
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
    if (index < DATA_MODE_INDEX)
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
