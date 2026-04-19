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
#include "../system/cmath.h"
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
    bool previousUSBPowered;
} power;

static void onPowerOffViewEvent(ViewEvent event);
static void powerOffWithBatteryIndicator(bool batteryIndicator);

#if defined(POWER_MENU)
static const Menu powerMenu;
#endif
#if defined(BATTERY_REMOVABLE)
static const Menu powerBatteryTypeMenu;
#endif

static void resetPower(void)
{
#if defined(POWER_MENU)
    selectMenuItem(&powerMenu, 0);
#endif
#if defined(BATTERY_REMOVABLE)
    selectMenuItem(&powerBatteryTypeMenu, settings.powerBatteryType);
#endif
}

// Power on

static void onPowerOnViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_DRAW:
        if (power.onViewState == POWERON_VIEW_FLASHFAILURE)
            drawSplash(getString(STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE));
        else
            drawSplash(getString(STRING_APP_NAME) "\n\n" FIRMWARE_VERSION);

        break;

    case EVENT_POSTDRAW:
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
            setMeasurementsEnabled(true);
            startDatalog();
            setMeasurementView();
        }

    default:
        break;
    }
}

void boot(void)
{
#if !defined(START_POWERED)
    updatePowerState();

    bool powerKeyDown = isPowerKeyDown();

    if (!powerKeyDown && !settings.powerUSBAutoPowerOn)
    {
        powerOffWithBatteryIndicator(true);

        return;
    }

    if (powerKeyDown)
        waitLongKeyPress();
#endif

    powerOn();
}

void powerOn(void)
{
    if (isPoweredOn())
        return;

    setPowerEnabled(true);

    resetPower();
    resetSettings();
    resetMeasurements();
    resetTube();
    resetDatalog();
    resetDisplay();
#if defined(SOUND)
    resetSound();
#endif
    resetRTC();
    resetRNG();
#if defined(GAME)
    resetGame();
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

    startHeartbeatEvents();
    showView(onPowerOnViewEvent);

    power.poweredOn = true;
}

// Power off

static void triggerPowerOffBacklight(void)
{
    requestBacklightTrigger();
    setPowerEnabled(true);

    power.displayTimer = POWEROFF_DISPLAY_TIME;

    startHeartbeatEvents();
}

static void cancelPowerOffBacklight(void)
{
    cancelBacklight();
    setPowerEnabled(false);

    power.displayTimer = 0;
}

static void onPowerOffViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_TOGGLEBACKLIGHT:
        triggerPowerOffBacklight();

        break;

    case EVENT_DRAW:
        drawPowerOff(power.displayTimer != 0);

        break;

    case EVENT_HEARTBEAT:
        if (power.displayTimer)
        {
            power.displayTimer--;
            if (power.displayTimer == 0)
                cancelPowerOffBacklight();
        }

        break;

    default:
        break;
    }
}

static void powerOffWithBatteryIndicator(bool batteryIndicator)
{
    if (isPoweredOn())
    {
        setMeasurementsEnabled(false);
        saveSettings();
        stopDatalog();
        setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);
    }

    showView(onPowerOffViewEvent);

    if (batteryIndicator)
        triggerPowerOffBacklight();
    else
        cancelPowerOffBacklight();

    power.poweredOn = false;
}

void powerOff(void)
{
    powerOffWithBatteryIndicator(false);
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

        if (isPoweredOn() && lowBattery)
            powerOffWithBatteryIndicator(true);
    }

    if (!power.previousUSBPowered && usbPowered)
    {
        requestBacklightTrigger();
        triggerVibration();
    }

    power.previousUSBPowered = usbPowered;
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
};

static const char *onPowerBatteryTypeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.powerBatteryType);

    return getString(powerBatteryTypeMenuOptions[index]);
}

static void onPowerBatteryTypeMenuSelect(menu_size_t index)
{
    settings.powerBatteryType = index;
}

static MenuState powerBatteryTypeMenuState;

static const Menu powerBatteryTypeMenu = {
    STRING_BATTERY,
    &powerBatteryTypeMenuState,
    ARRAY_SIZE(powerBatteryTypeMenuOptions),
    onPowerBatteryTypeMenuGetOption,
    onPowerBatteryTypeMenuSelect,
    showPowerMenu,
};

#endif

// Power menu

#if defined(POWER_MENU)

enum
{
#if defined(BATTERY_REMOVABLE)
    POWER_BATTERY,
#endif
#if defined(USB_AUTOPOWER_ON)
    POWER_USB_AUTOPOWER_ON,
#endif
#if defined(DATA_MODE)
    POWER_DATA_MODE,
#endif
};

static cstring powerMenuOptions[] = {
#if defined(BATTERY_REMOVABLE)
    STRING_BATTERY,
#endif
#if defined(USB_AUTOPOWER_ON)
    STRING_USB_AUTOPOWER_ON,
#endif
#if defined(DATA_MODE)
    STRING_DATAMODE,
#endif
};

static const char *onPowerMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    switch (index)
    {
#if defined(BATTERY_REMOVABLE)
    case POWER_BATTERY:
        *menuStyle = MENUSTYLE_SUBMENU;

        break;
#endif

#if defined(USB_AUTOPOWER_ON)
    case POWER_USB_AUTOPOWER_ON:
        *menuStyle = settings.powerUSBAutoPowerOn;

        break;
#endif

#if defined(DATA_MODE)
    case POWER_DATA_MODE:
        *menuStyle = settings.dataMode;

        break;
#endif
    }

    return getString(powerMenuOptions[index]);
}

static void onPowerMenuSelect(menu_size_t index)
{
    switch (index)
    {
#if defined(BATTERY_REMOVABLE)
    case POWER_BATTERY:
        showMenu(&powerBatteryTypeMenu);

        break;
#endif

#if defined(USB_AUTOPOWER_ON)
    case POWER_USB_AUTOPOWER_ON:
        settings.powerUSBAutoPowerOn = !settings.powerUSBAutoPowerOn;

        break;
#endif

#if defined(DATA_MODE)
    case POWER_DATA_MODE:
        settings.dataMode = !settings.dataMode;

        break;
#endif
    }
}

static MenuState powerMenuState;

static const Menu powerMenu = {
    STRING_POWER,
    &powerMenuState,
    ARRAY_SIZE(powerMenuOptions),
    onPowerMenuGetOption,
    onPowerMenuSelect,
    showSettingsMenu,
};

void showPowerMenu(void)
{
    showMenu(&powerMenu);
}

#endif
