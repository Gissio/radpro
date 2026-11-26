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
#include "../peripherals/rtc.h"
#include "../peripherals/sound.h"
#include "../peripherals/tube.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../system/system.h"
#include "../ui/menu.h"
#include "../ui/system.h"

#define BATTERY_LEVEL_NUM 5

// Battery voltages are compared as integers, in units of 0.1 mV
#define BATTERY_VOLTAGE_SCALE 10000.0F
#define BATTERY_VOLTAGE(value) ((uint16_t)((value) * BATTERY_VOLTAGE_SCALE + 0.5F))

// A level is entered above (nominal + BATTERY_HYSTERESIS) and left below
// (nominal - BATTERY_HYSTERESIS)
#define BATTERY_HYSTERESIS BATTERY_VOLTAGE(0.0025F)

// Seconds the battery indicator is shown while powered off
#define BATTERY_INDICATOR_TIME 5

typedef enum
{
    POWERON_FLASHFAILURE, // Showing the checksum failure screen
    POWERON_SPLASH,       // Showing the splash screen, state not yet loaded
    POWERON_LOADED,       // Showing the splash screen, RTC and history loaded
} PowerOnState;

// While powered off, the MCU only runs while it has power: during the
// battery indicator (PWR_EN latched) or from USB. On battery, it loses power
// when the indicator ends (hideBatteryIndicator() releases PWR_EN).
static struct
{
    bool poweredOn;

    PowerOnState powerOnState;

    // Seconds left showing the battery indicator; 0: hidden
    uint32_t batteryIndicatorTimer;

    float batteryVoltage;
    uint8_t batteryLevel;
    bool previousUSBPowered;
} power;

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

static void loadPowerOnState(void)
{
    // Runs once, even if the splash screen is redrawn
    if (power.powerOnState != POWERON_SPLASH)
        return;

    initRTC();
    loadHistory();

    power.powerOnState = POWERON_LOADED;
}

// Power-on sequence: [checksum failure screen, 1 s] -> splash screen
// (state loaded after it is drawn, 1 s) -> measurement view
static void onPowerOnViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_DRAW:
        if (power.powerOnState == POWERON_FLASHFAILURE)
            drawSplash(getString(STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE));
        else
            drawSplash(getString(STRING_APP_NAME) "\n\n" FIRMWARE_VERSION);

        break;

    case EVENT_POSTDRAW:
        // Loading takes a while: do it with the splash screen visible
        loadPowerOnState();

        break;

    case EVENT_HEARTBEAT:
        if (power.powerOnState == POWERON_FLASHFAILURE)
            power.powerOnState = POWERON_SPLASH;
        else
        {
            // In case the splash post-draw was skipped (display inactive)
            loadPowerOnState();

            setMeasurementsEnabled(true);
            startDatalog();
            setMeasurementView();
        }

        break;

    default:
        break;
    }
}

void boot(void)
{
#if !defined(START_POWERED)
    updatePowerState();

    // The debounced power key state needs samples taken after
    // initKeyboard(): wait for a full keyboard sample window
    sleep(KEY_TICKS);

    // Power on after a watchdog reset (crash recovery), a long power key
    // press, or with USB auto power-on; otherwise show the battery indicator
    if (wasResetByWatchdog() ||
        (isPowerKeyDown() && waitLongKeyPress()) ||
        settings.powerUSBAutoPowerOn)
        powerOn();
    else
        powerOffWithBatteryIndicator(true);
#else
    powerOn();
#endif

    clearResetFlags();

    startKeyboardEvents();
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
        power.powerOnState = POWERON_FLASHFAILURE;
        triggerAlert(true);
    }
    else
        power.powerOnState = POWERON_SPLASH;

    requestBacklightTrigger();
    triggerVibration();

    startHeartbeatEvents();
    showView(onPowerOnViewEvent);

    power.poweredOn = true;
}

// Power off

// While powered off: enables power (needed by the display) and shows the
// battery indicator for BATTERY_INDICATOR_TIME seconds
static void showBatteryIndicator(void)
{
    requestBacklightTrigger();
    setPowerEnabled(true);

    power.batteryIndicatorTimer = BATTERY_INDICATOR_TIME;

    startHeartbeatEvents();
}

// While powered off: hides the battery indicator and disables power
static void hideBatteryIndicator(void)
{
    cancelBacklight();
    setPowerEnabled(false);

    power.batteryIndicatorTimer = 0;
}

static void onPowerOffViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_TOGGLEBACKLIGHT:
        // Any key except power (which powers on, see updateView)
        showBatteryIndicator();

        break;

    case EVENT_DRAW:
        drawPowerOff(power.batteryIndicatorTimer != 0);

        break;

    case EVENT_HEARTBEAT:
        if (power.batteryIndicatorTimer)
        {
            power.batteryIndicatorTimer--;
            if (power.batteryIndicatorTimer == 0)
                hideBatteryIndicator();
        }

        break;

    default:
        break;
    }
}

static void powerOffWithBatteryIndicator(bool batteryIndicator)
{
#if defined(GAME)
    stopGameSearch();
#endif

    if (power.poweredOn)
    {
        setMeasurementsEnabled(false);
        saveSettings();
        stopDatalog();
        setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);
    }

    showView(onPowerOffViewEvent);

    if (batteryIndicator)
        showBatteryIndicator();
    else
        hideBatteryIndicator();

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

static const uint16_t batteryLevels[BATTERYTYPE_NUM][BATTERY_LEVEL_NUM - 1] = {
#if defined(BATTERY_REMOVABLE)
    // Ni-MH
    {
        BATTERY_VOLTAGE(1.198F),
        BATTERY_VOLTAGE(1.243F),
        BATTERY_VOLTAGE(1.268F),
        BATTERY_VOLTAGE(1.297F),
    },

    // Alkaline
    {
        BATTERY_VOLTAGE(1.159F),
        BATTERY_VOLTAGE(1.220F),
        BATTERY_VOLTAGE(1.283F),
        BATTERY_VOLTAGE(1.358F),
    },
#else
    // Li-Ion
    {
        BATTERY_VOLTAGE(3.527F),
        BATTERY_VOLTAGE(3.646F),
        BATTERY_VOLTAGE(3.839F),
        BATTERY_VOLTAGE(3.982F),
    },
#endif
};

static const uint16_t batteryLow[BATTERYTYPE_NUM] = {
#if defined(BATTERY_REMOVABLE)
    BATTERY_VOLTAGE(1.0F), // Ni-MH
    BATTERY_VOLTAGE(0.8F), // Alkaline
#else
    BATTERY_VOLTAGE(3.0F), // Li-Ion
#endif
};

void updatePowerState(void)
{
    // Battery level
    float batteryVoltage = readBatteryVoltage();
    power.batteryVoltage = batteryVoltage;
    uint32_t voltage = (uint32_t)(batteryVoltage * BATTERY_VOLTAGE_SCALE);

    const uint16_t *levelVoltages = batteryLevels[settings.powerBatteryType];
    uint32_t level = 0;
    for (uint32_t i = 0; i < (BATTERY_LEVEL_NUM - 1); i++)
    {
        uint32_t thresholdVoltage = levelVoltages[i] - BATTERY_HYSTERESIS;

        // Rising past the current level needs the upper threshold
        if (i >= power.batteryLevel)
            thresholdVoltage += 2 * BATTERY_HYSTERESIS;

        if (voltage >= thresholdVoltage)
            level = i + 1;
    }
    power.batteryLevel = level;

    // Low battery: power off
    bool usbPowered = isUSBPowered();
    if (power.poweredOn &&
        !usbPowered &&
        (voltage < batteryLow[settings.powerBatteryType]))
        powerOffWithBatteryIndicator(true);

    // USB connected: wake the display (powered off: show the battery
    // indicator, which also enables power for the display)
    if (!power.previousUSBPowered && usbPowered)
    {
        if (power.poweredOn)
            requestBacklightTrigger();
        else
            showBatteryIndicator();
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
