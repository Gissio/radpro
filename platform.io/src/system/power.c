/*
 * Rad Pro
 * Power management
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../extras/game.h"
#include "../extras/rng.h"
#include "../measurements/datalog.h"
#include "../measurements/history.h"
#include "../measurements/measurements.h"
#include "../peripherals/adc.h"
#include "../peripherals/display.h"
#include "../peripherals/flash.h"
#include "../peripherals/keyboard.h"
#include "../peripherals/led.h"
#include "../peripherals/pulsesoundenable.h"
#include "../peripherals/rtc.h"
#include "../peripherals/tube.h"
#include "../peripherals/sound.h"
#include "../peripherals/voice.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../system/system.h"
#include "../ui/menu.h"
#include "../ui/system.h"

#define BATTERY_LEVEL_NUM 5

#define BATTERY_HYSTERESIS_RANGE 0.005F
#define BATTERY_HYSTERESIS (BATTERY_HYSTERESIS_RANGE / 2)

#define POWEROFF_DISPLAY_TIME 5

typedef enum
{
    POWERON_VIEW_FLASHFAILURE,
    POWERON_VIEW_SPLASH,
} PowerOnViewState;

static struct
{
    bool poweredOn;

    PowerOnViewState onViewState;

    uint32_t displayTimer;

    float batteryVoltage;
    uint8_t batteryLevel;
    bool usbPoweredLast;
} power;

#if defined(BATTERY_REMOVABLE) || !defined(START_POWERED)
static Menu powerMenu;
#endif

#if defined(BATTERY_REMOVABLE)
static Menu powerBatteryTypeMenu;

static void setPowerMenu(void);
#endif

static void setupPower(void)
{
#if defined(BATTERY_REMOVABLE)
    selectMenuItem(&powerMenu, 0, 0);
    selectMenuItem(&powerBatteryTypeMenu, settings.powerBatteryType, BATTERYTYPE_NUM);
#endif
}

// Power on

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
            power.poweredOn = true;

#if defined(PULSESOUND_ENABLE)
            updatePulseSoundEnable();
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
            updateLED();
#endif
            setMeasurementsEnabled(true);
            if (settings.loggingMode != DATALOG_LOGGINGMODE_OFF)
                startDatalog();

            setMeasurementViewCurrent();
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
#if !defined(START_POWERED)
    if (isBoot)
    {
        updatePowerState();

        bool powerKeyDown = isPowerKeyDown();

        if (!powerKeyDown && !settings.powerUSBAutoPowerOn)
        {
            setView(&powerOffView);
            powerOff(true);

            return;
        }

        if (powerKeyDown)
            waitLongKeyPress();
    }
#endif

    setPowerEnabled(true);

    setupEvents();
    setupPower();
    setupSettings();
    setupMeasurements();
    setupTube();
    setupDatalog();
    setupDisplay();
#if defined(BUZZER) || defined(PULSESOUND_ENABLE) || defined(VOICE)
    setupSound();
#endif
    setupRTC();
    setupRNG();
#if defined(GAME)
    setupGame();
#endif

    if (!verifyFlash())
    {
        power.onViewState = POWERON_VIEW_FLASHFAILURE;

        triggerAlert(true);
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
        setupEvents();
        requestBacklightTrigger();

        break;

    case EVENT_DRAW:
        drawPowerOff(power.displayTimer != 0);

        break;

    case EVENT_HEARTBEAT:
        if (!isBacklightActive())
            setPowerEnabled(false);

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
    setupEvents();

    bool poweredOnLast = power.poweredOn;
    power.poweredOn = false;

    setMeasurementsEnabled(false);

    if (poweredOnLast)
    {
        saveSettings();
#if defined(PULSESOUND_ENABLE)
        updatePulseSoundEnable();
#endif
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
        updateLED();
#endif
        stopDatalog();
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
        setPowerEnabled(false);
    }

    setView(&powerOffView);
}

// Powered on

bool isPoweredOn(void)
{
    return power.poweredOn;
}

// Battery

#if defined(BATTERY_REMOVABLE)
static const float batteryLevels[2][BATTERY_LEVEL_NUM - 1] = {
    // Ni-MH
    {
        1.198F - BATTERY_HYSTERESIS,
        1.243F - BATTERY_HYSTERESIS,
        1.268F - BATTERY_HYSTERESIS,
        1.297F - BATTERY_HYSTERESIS,
    },

    // Alkaline
    {
        1.159F - BATTERY_HYSTERESIS,
        1.220F - BATTERY_HYSTERESIS,
        1.283F - BATTERY_HYSTERESIS,
        1.358F - BATTERY_HYSTERESIS,
    },
};

static const float batteryLow[2] = {
    1.0F,
    0.8F,
};
#else
static const float batteryLevels[1][BATTERY_LEVEL_NUM - 1] = {
    // Li-Ion
    {
        3.527F - BATTERY_HYSTERESIS,
        3.646F - BATTERY_HYSTERESIS,
        3.839F - BATTERY_HYSTERESIS,
        3.982F - BATTERY_HYSTERESIS,
    },
};

static const float batteryLow[1] = {
    3.0F,
};
#endif

void updatePowerState(void)
{
    // Battery level
    power.batteryVoltage = readBatteryVoltage();

    const float *batteryLevel = batteryLevels[settings.powerBatteryType];
    uint8_t level = 0;
    for (uint32_t i = 0; i < (BATTERY_LEVEL_NUM - 1); i++)
    {
        float thresholdVoltage = batteryLevel[i];

        if (i >= level)
            thresholdVoltage += BATTERY_HYSTERESIS_RANGE;

        if (power.batteryVoltage >= thresholdVoltage)
            level = i + 1;
    }
    power.batteryLevel = level;

    // Power state
    bool usbPowered = isUSBPowered();
    if (!usbPowered)
    {
        bool lowBattery = (power.batteryVoltage < batteryLow[settings.powerBatteryType]);

        if (power.poweredOn && lowBattery)
            powerOff(true);
    }

    if (!power.usbPoweredLast && usbPowered)
    {
        requestBacklightTrigger();
        triggerVibration();
    }

    power.usbPoweredLast = usbPowered;
}

float getBatteryVoltage(void)
{
    return power.batteryVoltage;
}

uint8_t getBatteryLevel(void)
{
    return power.batteryLevel;
}

// Battery type menu

#if defined(BATTERY_REMOVABLE)

static cstring powerBatteryTypeMenuOptions[] = {
    STRING_NI_MH,
    STRING_ALKALINE,
    NULL,
};

static const char *onPowerBatteryTypeMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.powerBatteryType);

    return getString(powerBatteryTypeMenuOptions[index]);
}

static void onPowerBatteryTypeMenuSelect(uint32_t index)
{
    settings.powerBatteryType = index;
}

static MenuState powerBatteryTypeMenuState;

static Menu powerBatteryTypeMenu = {
    STRING_BATTERY,
    &powerBatteryTypeMenuState,
    onPowerBatteryTypeMenuGetOption,
    onPowerBatteryTypeMenuSelect,
    setPowerMenu,
};

static View powerBatteryTypeMenuView = {
    onMenuEvent,
    &powerBatteryTypeMenu,
};

#endif

// Power menu

#if defined(BATTERY_REMOVABLE) || !defined(START_POWERED)

static cstring powerMenuOptions[] = {
#if defined(BATTERY_REMOVABLE)
    STRING_BATTERY,
#endif
#if !defined(START_POWERED)
    STRING_USB_AUTO_POWER_ON,
#endif
    NULL,
};

static const char *onPowerMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
#if defined(BATTERY_REMOVABLE) && !defined(START_POWERED)
    switch (index)
    {
    case 0:
        *menuStyle = MENUSTYLE_SUBMENU;

        break;

    case 1:
        *menuStyle = settings.powerUSBAutoPowerOn;

        break;
    }
#elif defined(BATTERY_REMOVABLE)
    *menuStyle = MENUSTYLE_SUBMENU;
#elif !defined(START_POWERED)
    *menuStyle = settings.powerUSBAutoPowerOn;
#endif

    return getString(powerMenuOptions[index]);
}

static void onPowerMenuSelect(uint32_t index)
{
#if defined(BATTERY_REMOVABLE) && !defined(START_POWERED)
    switch (index)
    {
    case 0:
        setView(&powerBatteryTypeMenuView);

        break;

    case 1:
        settings.powerUSBAutoPowerOn = !settings.powerUSBAutoPowerOn;

        break;
    }
#elif defined(BATTERY_REMOVABLE)
    setView(&powerBatteryTypeMenuView);
#elif !defined(START_POWERED)
    settings.powerUSBAutoPowerOn = !settings.powerUSBAutoPowerOn;
#endif
}

static MenuState powerMenuState;

static Menu powerMenu = {
    STRING_POWER,
    &powerMenuState,
    onPowerMenuGetOption,
    onPowerMenuSelect,
    setSettingsMenu,
};

View powerMenuView = {
    onMenuEvent,
    &powerMenu,
};

#if defined(BATTERY_REMOVABLE)
static void setPowerMenu(void)
{
    setView(&powerMenuView);
}
#endif

#endif
