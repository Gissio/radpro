/*
 * Rad Pro
 * Battery
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include "adc.h"
#include "datalog.h"
#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "power.h"
#include "settings.h"

#if defined(BATTERY_REMOVABLE)

static const float batteryLevelThresholds[2][5] = {
    // Alkaline

    {1.204F, 1.240F, 1.261F, 1.276F, 1.310F},

    // Ni-MH

    {1.186F, 1.260F, 1.296F, 1.338F, 1.395F},
};

#else

static const float batteryLevelThresholds[] =
    // Li-Ion

    {3.377F, 3.554F, 3.660F, 3.824F, 3.889F};

#endif

#if defined(BATTERY_REMOVABLE)
static const Menu batteryTypeMenu;
#endif

void initPower()
{
    initPowerHardware();

#if defined(BATTERY_REMOVABLE)
    selectMenuItem(&batteryTypeMenu,
                   settings.batteryType,
                   BATTERYTYPE_NUM);
#endif
}

void powerOff(void)
{
    writeDatalogEntry(true);
    writeSettings();

    // Disable devices

    setEventHandling(false);
    setDisplay(false);
    setPower(false);

    while (getKeyboardEvent() != EVENT_KEY_POWER)
        sleep(1);

    // Enable devices (runs when using external power supply)

    setPower(true);
    setEventHandling(true);
    triggerBacklight();

    // Resume

    writeDatalogEntry(false);

    updateView();
}

int8_t getBatteryLevel(void)
{
    if (isBatteryCharging())
        return BATTERY_LEVEL_CHARGING;

#if defined(BATTERY_REMOVABLE)
    const float *batteryLevelThreshold = batteryLevelThresholds[settings.batteryType];
#else
    const float *batteryLevelThreshold = batteryLevelThresholds;
#endif

    int8_t level = BATTERY_LEVEL_MAX;
    float voltage = getBatteryVoltage();

    for (uint32_t i = 0; i < BATTERY_LEVEL_MAX; i++)
    {
        if (batteryLevelThreshold[i] > voltage)
        {
            level = i;

            break;
        }
    }

    return level;
}

// Battery type menu

#if defined(BATTERY_REMOVABLE)

static const char *const batteryTypeMenuOptions[] = {
    "Ni-MH",
    "Alkaline",
    NULL,
};

static const char *onPulseClicksMenuGetOption(const Menu *menu,
                                              uint32_t index,
                                              MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.batteryType);

    return batteryTypeMenuOptions[index];
}

static void onBatteryTypeMenuSelect(const Menu *menu)
{
    settings.batteryType = menu->state->selectedIndex;

    resetADCFilters();
}

static MenuState batteryTypeMenuState;

static const Menu batteryTypeMenu = {
    "Battery type",
    &batteryTypeMenuState,
    onPulseClicksMenuGetOption,
    onBatteryTypeMenuSelect,
    onSettingsSubMenuBack,
};

const View batteryTypeMenuView = {
    onMenuEvent,
    &batteryTypeMenu,
};

#endif
