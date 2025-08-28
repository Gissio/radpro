/*
 * Rad Pro
 * Power management
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "adc.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "flash.h"
#include "game.h"
#include "keyboard.h"
#include "led.h"
#include "measurements.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "sound.h"
#include "system.h"
#include "tube.h"

#define BATTERY_LEVEL_NUM 5

// First order filter (n: time constant in taps): k = exp(-1 / n)
// For n = 100 (seconds):
#define BATTERY_VOLTAGE_FILTER_CONSTANT 0.99F

#define BATTERY_LEVEL_HYSTERESIS 0.01F

#define POWEROFF_DISPLAY_TIME 5

typedef enum
{
    POWERON_VIEW_FLASHFAILURE,
    POWERON_VIEW_SPLASH,
} PowerOnViewState;

static struct
{
    PowerOnViewState onViewState;

    uint32_t displayTimer;
} power;

// Power on

static void resetBattery(void);

static void onPowerOnViewEvent(Event event)
{
    switch (event)
    {
    case EVENT_DRAW:
        if (power.onViewState == POWERON_VIEW_FLASHFAILURE)
            drawSplash(getString(STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE));
        else
            drawSplash(getString(STRING_APP_NAME) "\n\n" FIRMWARE_VERSION);

        break;

    case EVENT_POST_DRAW:
        if (power.onViewState == POWERON_VIEW_SPLASH)
        {
            initRTC();
            loadHistory();
        }

        break;

    case EVENT_HEARTBEAT:
        if (power.onViewState == POWERON_VIEW_FLASHFAILURE)
            power.onViewState = POWERON_VIEW_SPLASH;
        else
        {
            // Start measurements
#if defined(SOUND_EN)
            updateSound(true);
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
            updateLED(true);
#endif
            setTubeHV(true);
            setMeasurements(true);
            openDatalogWrite();

            setMeasurementView(0);
        }

    default:
        break;
    }
}

View powerOnView = {
    onPowerOnViewEvent,
    NULL,
};

void powerOn(bool isBoot)
{
#if !defined(START_POWERON)
    if (isBoot)
    {
        updateBattery();

        bool powerKeyDown = isPowerKeyDown();

        if (isPowerOnReset() &&
            !powerKeyDown)
        {
            setView(&powerOffView);
            powerOff(true);

            return;
        }

        if (powerKeyDown)
            waitLongKeyPress();
    }
#endif

    setPower(true);

    resetEvents();
    resetBattery();
    resetSettings();
    resetMeasurements();
    resetTube();
    resetDisplay();
    resetSound();
    resetDatalog();
    resetRTC();
    resetRNG();
#if defined(GAME)
    resetGame();
#endif

    if (!verifyFlash())
    {
        power.onViewState = POWERON_VIEW_FLASHFAILURE;

        triggerAlert();
    }
    else
        power.onViewState = POWERON_VIEW_SPLASH;

    requestBacklightTrigger();
    triggerVibration();

    setView(&powerOnView);
}

// Power off

static void onPowerOffViewEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_TOGGLEBACKLIGHT:
        power.displayTimer = POWEROFF_DISPLAY_TIME;
        resetEvents();
        requestBacklightTrigger();

        break;

    case EVENT_DRAW:
        drawPowerOff(power.displayTimer != 0);

        break;

    case EVENT_HEARTBEAT:
        if (!isBacklightActive())
            setPower(false);

        if (power.displayTimer)
        {
            power.displayTimer--;
            if (!power.displayTimer)
                cancelBacklight();
        }

        break;

    default:
        break;
    }
}

View powerOffView = {
    onPowerOffViewEvent,
    NULL,
};

void powerOff(bool showBatteryIndicator)
{
    resetEvents();

    setMeasurements(false);

    if (!isInPowerOffView())
    {
        writeSettings();
#if defined(SOUND_EN)
        updateSound(false);
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
        updateLED(false);
#endif
        setTubeHV(false);
        closeDatalogWrite();
        setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);
#if defined(VOICE)
        stopVoice();
#endif
    }

    if (showBatteryIndicator)
    {
        power.displayTimer = POWEROFF_DISPLAY_TIME;
        requestBacklightTrigger();
    }
    else
    {
        power.displayTimer = 0;
        cancelBacklight();
        setPower(false);
    }

    setView(&powerOffView);
}

bool isInPowerOffView(void)
{
    return (getView() == &powerOffView);
}

// Battery

#if defined(BATTERY_REMOVABLE)
static const float batteryLevels[2][BATTERY_LEVEL_NUM - 1] = {
    // Ni-MH
    {1.198F, 1.243F, 1.268F, 1.297F},

    // Alkaline
    {1.159F, 1.220F, 1.283F, 1.358F},
};

static const float batteryLow[2] = {
    1.0F,
    0.8F,
};

static Menu batteryTypeMenu;
#else
static const float batteryLevels[1][BATTERY_LEVEL_NUM - 1] = {
    // Li-Ion
    {3.527F, 3.646F, 3.839F, 3.982F},
};

static const float batteryLow[1] = {
    3.0F,
};
#endif

struct
{
    float voltage;
    float filteredVoltage;

    uint8_t level;

    bool lastUSBPowered;
} battery;

static void resetBattery(void)
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

    const float *batteryLevel = batteryLevels[settings.batteryType];

    uint8_t level = 0;

    for (uint32_t i = 0; i < (BATTERY_LEVEL_NUM - 1); i++)
    {
        float thresholdVoltage = batteryLevel[i];

        if (i >= battery.level)
            thresholdVoltage += BATTERY_LEVEL_HYSTERESIS;

        if (battery.filteredVoltage >= thresholdVoltage)
            level = i + 1;
    }

    battery.level = level;

    // Power state
    bool usbPowered = isUSBPowered();

    if (!usbPowered)
    {
        bool lowBattery = (battery.filteredVoltage < batteryLow[settings.batteryType]);

        if (!isInPowerOffView() && lowBattery)
            powerOff(true);
    }

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

// Battery type menu

#if defined(BATTERY_REMOVABLE)

static cstring batteryTypeMenuOptions[] = {
    STRING_NI_MH,
    STRING_ALKALINE,
    NULL,
};

static const char *onBatteryTypeMenuGetOption(uint32_t index,
                                              MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.batteryType);

    return getString(batteryTypeMenuOptions[index]);
}

static void onBatteryTypeMenuSelect(uint32_t index)
{
    settings.batteryType = index;
}

static MenuState batteryTypeMenuState;

static Menu batteryTypeMenu = {
    STRING_BATTERY_TYPE,
    &batteryTypeMenuState,
    onBatteryTypeMenuGetOption,
    onBatteryTypeMenuSelect,
    onSettingsSubMenuBack,
};

View batteryTypeMenuView = {
    onMenuEvent,
    &batteryTypeMenu,
};

#endif
