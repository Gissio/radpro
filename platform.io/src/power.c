/*
 * Rad Pro
 * Power management
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "adc.h"
#include "comm.h"
#include "cstring.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "flash.h"
#include "game.h"
#include "keyboard.h"
#include "measurements.h"
#include "power.h"
#include "pulsecontrol.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

// Power on

typedef enum
{
    POWERON_VIEW_FLASHFAILURE,
    POWERON_VIEW_SPLASH,
} PowerOnViewState;

static PowerOnViewState powerOnViewState;

static void onPowerOnViewEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_DRAW:
        if (powerOnViewState == POWERON_VIEW_FLASHFAILURE)
            drawNotification(getString(STRING_NOTIFICATION_WARNING),
                             getString(STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE));
        else
            drawNotification(getString(STRING_APP_NAME),
                             FIRMWARE_VERSION);

        break;

    case EVENT_POST_DRAW:
        if (powerOnViewState == POWERON_VIEW_SPLASH)
            initRTC();

        break;

    case EVENT_PERIOD:
        if (powerOnViewState == POWERON_VIEW_FLASHFAILURE)
            powerOnViewState = POWERON_VIEW_SPLASH;
        else
        {
            // Start measurements
            setTubeHV(true);
            enableMeasurements();
#if defined(DATA_MODE)
            if (settings.dataMode)
                openComm();
#elif !defined(PWR_USB)
            openComm();
#endif
            openDatalog();

            setMeasurementView(0);
        }

    default:
        break;
    }
}

const View powerOnView = {
    onPowerOnViewEvent,
    NULL,
};

void powerOn(void)
{
    // Power on
    setPower(true);

    if (!verifyFlash())
    {
        powerOnViewState = POWERON_VIEW_FLASHFAILURE;

        triggerAlarm();
    }
    else
        powerOnViewState = POWERON_VIEW_SPLASH;

    requestBacklightTrigger();
    triggerVibration();
#if defined(PULSE_CONTROL)
    updatePulseControl();
#endif

    // Reset
    resetEvents();
    resetPower();
    resetSettings();
    resetMeasurements();
    resetTube();
    resetDisplay();
    resetDatalog();
    resetRTC();
    resetRNG();
#if defined(GAME)
    resetGame();
#endif

    // Set view
    setView(&powerOnView);
}

// Power off

static void onPowerOffViewEvent(const View *view, Event event)
{
}

const View powerOffView = {
    onPowerOffViewEvent,
    NULL,
};

void powerOff(void)
{
    // Set view
    setView(&powerOffView);

    // Power off
    writeSettings();
    closeDatalog();
    closeComm();
    disableMeasurements();
    setTubeHV(false);
#if defined(PULSE_CONTROL)
    updatePulseControl();
#endif
    cancelBacklight();

    setPower(false);
}

bool isPoweredOff(void)
{
    return (getView() == &powerOffView);
}

// Battery

#if defined(BATTERY_REMOVABLE)
static const float batteryLevelThresholds[2][BATTERY_LEVEL_NUM - 1] = {
    // Ni-MH
    {1.198F, 1.243F, 1.268F, 1.297F},

    // Alkaline
    {1.159F, 1.220F, 1.283F, 1.358F},
};
#else
static const float batteryLevelThresholds[BATTERY_LEVEL_NUM - 1] =
    // Li-Ion
    {3.527F, 3.646F, 3.839F, 3.982F};
#endif

#if defined(BATTERY_REMOVABLE)
static const Menu batteryTypeMenu;
#endif

void resetPower()
{
#if defined(BATTERY_REMOVABLE)
    selectMenuItem(&batteryTypeMenu,
                   settings.batteryType,
                   BATTERYTYPE_NUM);
#endif
}

uint8_t getBatteryLevel(void)
{
#if defined(BATTERY_REMOVABLE)
    const float *selectedBatteryLevelThresholds = batteryLevelThresholds[settings.batteryType];
#else
    const float *selectedBatteryLevelThresholds = batteryLevelThresholds;
#endif

    float voltage = getFilteredBatteryVoltage();

    for (uint32_t i = 0; i < (BATTERY_LEVEL_NUM - 1); i++)
    {
        if (voltage < selectedBatteryLevelThresholds[i])
            return i;
    }

    return (BATTERY_LEVEL_NUM - 1);
}

// Battery type menu

#if defined(BATTERY_REMOVABLE)

static const char *const batteryTypeMenuOptions[] = {
    getString(STRING_NI_MH),
    getString(STRING_ALKALINE),
    NULL,
};

static const char *onBatteryTypeMenuGetOption(const Menu *menu,
                                              uint32_t index,
                                              MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.batteryType);

    return batteryTypeMenuOptions[index];
}

static void onBatteryTypeMenuSelect(const Menu *menu)
{
    settings.batteryType = menu->state->selectedIndex;
}

static MenuState batteryTypeMenuState;

static const Menu batteryTypeMenu = {
    getString(STRING_BATTERY_TYPE),
    &batteryTypeMenuState,
    onBatteryTypeMenuGetOption,
    onBatteryTypeMenuSelect,
    onSettingsSubMenuBack,
};

const View batteryTypeMenuView = {
    onMenuEvent,
    &batteryTypeMenu,
};

#endif
