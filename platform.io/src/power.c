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

#define BATTERY_LEVEL_MAX 5
#define BATTERY_LEVEL_NUM 6

#if defined(BATTERY_REMOVABLE)
static const float batteryLevelThresholds[2][5] = {
    // Ni-MH
    {1.188F, 1.231F, 1.257F, 1.274F, 1.307F},

    // Alkaline
    {1.186F, 1.260F, 1.296F, 1.338F, 1.395F},
};
#else
static const float batteryLevelThresholds[] =
    // Li-Ion
    {3.527F, 3.653F, 3.780F, 3.907F, 4.033F};
#endif

#if defined(BATTERY_REMOVABLE)
static const Menu batteryTypeMenu;
#endif

bool powerOffRequested;

void initPower()
{
    initPowerController();

#if defined(BATTERY_REMOVABLE)
    selectMenuItem(&batteryTypeMenu,
                   settings.batteryType,
                   BATTERYTYPE_NUM);
#endif
}

void requestPowerOff(void)
{
    powerOffRequested = true;
}

bool isPowerOffRequested(void)
{
    if (powerOffRequested)
    {
        powerOffRequested = false;

        return true;
    }

    return false;
}

int8_t getDeviceBatteryLevel(void)
{
#if defined(BATTERY_REMOVABLE)
    const float *batteryLevelThreshold = batteryLevelThresholds[settings.batteryType];
#else
    const float *batteryLevelThreshold = batteryLevelThresholds;
#endif

    int8_t level = BATTERY_LEVEL_MAX;
    float voltage = getDeviceBatteryVoltage();

    for (uint32_t i = 0; i < BATTERY_LEVEL_MAX; i++)
    {
        if (voltage < batteryLevelThreshold[i])
        {
            level = i;

            break;
        }
    }

    if (isDevicePowered())
    {
#if !defined(FONT_SYMBOLS_LIM)
        if (isBatteryCharging() && 
            (level == BATTERY_LEVEL_MAX))
            level = BATTERY_LEVEL_MAX - 1;
#else
        level = BATTERY_LEVEL_MAX;
#endif

        level += BATTERY_LEVEL_NUM;
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
