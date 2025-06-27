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

Power power;

// Power on

static void resetBattery();

static void onPowerOnViewEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_DRAW:
        if (power.onViewState == POWERON_VIEW_FLASHFAILURE)
            drawNotification(getString(STRING_NOTIFICATION_WARNING),
                             getString(STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE));
        else
            drawNotification(getString(STRING_APP_NAME),
                             FIRMWARE_VERSION);

        break;

    case EVENT_POST_DRAW:
        if (power.onViewState == POWERON_VIEW_SPLASH)
            initRTC();

        break;

    case EVENT_PERIOD:
        if (power.onViewState == POWERON_VIEW_FLASHFAILURE)
            power.onViewState = POWERON_VIEW_SPLASH;
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
            startDatalog();

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
    setPower(true);

    if (!verifyFlash())
    {
        power.onViewState = POWERON_VIEW_FLASHFAILURE;

        triggerAlarm();
    }
    else
        power.onViewState = POWERON_VIEW_SPLASH;

    requestBacklightTrigger();
    triggerVibration();
#if defined(PULSE_CONTROL)
    updatePulseControl();
#endif

    resetEvents();
    resetBattery();
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

    setView(&powerOnView);
}

// Power off

#define POWEROFF_DISPLAY_TIME 5

static void onPowerOffViewEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_TOGGLEBACKLIGHT:
        power.offDisplayTimer = POWEROFF_DISPLAY_TIME;
        resetEvents();
        requestBacklightTrigger();

        break;

    case EVENT_DRAW:
        drawPowerOff(power.offDisplayTimer != 0);

        break;

    case EVENT_PERIOD:
        if (power.offDisplayTimer)
        {
            power.offDisplayTimer--;
            if (!power.offDisplayTimer)
                cancelBacklight();
        }

        break;

    default:
        break;
    }
}

const View powerOffView = {
    onPowerOffViewEvent,
    NULL,
};

void powerOff(bool displayEnabled)
{
    resetEvents();
    if (isPowered())
    {
        writeSettings();
        setTubeHV(false);
        stopDatalog();
        closeComm();
        setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);
    }
    else
        updateBattery();
    if (displayEnabled)
    {
        power.offDisplayTimer = POWEROFF_DISPLAY_TIME;
        requestBacklightTrigger();
    }
    else
    {
        power.offDisplayTimer = 0;
        cancelBacklight();
    }
    disableMeasurements();
#if defined(PULSE_CONTROL)
    updatePulseControl();
#endif

    setView(&powerOffView);

    setPower(false);
}

bool isPowered(void)
{
    return power.enabled;
}

// Battery

// First order filter (n: time constant in taps): k = exp(-1 / n)
// For n = 100 (seconds):
#define BATTERY_VOLTAGE_FILTER_CONSTANT 0.99F

#define BATTERY_LEVEL_HYSTERESIS 0.01F

#if defined(BATTERY_REMOVABLE)
static const float batteryLevelThresholds[2][BATTERY_LEVEL_NUM - 1] = {
    // Ni-MH
    {1.198F, 1.243F, 1.268F, 1.297F},

    // Alkaline
    {1.159F, 1.220F, 1.283F, 1.358F},
};

static const float batteryLowThreshold[2] = {
    1.0F,
    0.8F,
};

static const Menu batteryTypeMenu;
#else
static const float batteryLevelThresholds[BATTERY_LEVEL_NUM - 1] =
    // Li-Ion
    {3.527F, 3.646F, 3.839F, 3.982F};

static const float batteryLowThreshold =
    3.0F;
#endif

struct
{
    float voltage;
    float filteredVoltage;

    uint8_t level;

    bool lastUSBPowered;
} battery;

static void resetBattery()
{
#if defined(BATTERY_REMOVABLE)
    selectMenuItem(&batteryTypeMenu,
                   settings.batteryType,
                   BATTERYTYPE_NUM);
#endif
}

void updateBattery(void)
{
    // Battery voltage
    battery.voltage = readBatteryVoltage();

    // Battery level
    if (battery.filteredVoltage == 0.0F)
        battery.filteredVoltage = battery.voltage;
    else
        battery.filteredVoltage = battery.voltage + BATTERY_VOLTAGE_FILTER_CONSTANT * (battery.filteredVoltage - battery.voltage);

#if defined(BATTERY_REMOVABLE)
    const float *selectedBatteryLevelThresholds = batteryLevelThresholds[settings.batteryType];
#else
    const float *selectedBatteryLevelThresholds = batteryLevelThresholds;
#endif

    uint8_t level = 0;

    for (uint32_t i = 0; i < (BATTERY_LEVEL_NUM - 1); i++)
    {
        float thresholdVoltage = selectedBatteryLevelThresholds[i];

        if (i >= battery.level)
            thresholdVoltage += BATTERY_LEVEL_HYSTERESIS;

        if (battery.filteredVoltage >= thresholdVoltage)
            level = i + 1;
    }

    battery.level = level;

    // Low battery
#if defined(BATTERY_REMOVABLE)
    bool lowBattery = battery.filteredVoltage < batteryLowThreshold[settings.batteryType];
#else
    bool lowBattery = battery.filteredVoltage < batteryLowThreshold;
#endif

    if (lowBattery && isPowered())
        powerOff(true);

    // USB power state
    bool usbPowered = isUSBPowered();
    if (!battery.lastUSBPowered && usbPowered)
    {
        requestBacklightTrigger();
        triggerVibration();
    }
    battery.lastUSBPowered = usbPowered;
}

uint8_t getBatteryLevel(void)
{
    return battery.level;
}

float getBatteryVoltage(void)
{
    return getBatteryNum() * battery.voltage;
}

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
