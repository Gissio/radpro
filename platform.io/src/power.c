/*
 * Rad Pro
 * Battery
 *
 * (C) 2022-2025 Gissio
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

#define BATTERY_LEVEL_NUM 5

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

void initPower()
{
    initPowerController();

#if defined(BATTERY_REMOVABLE)
    selectMenuItem(&batteryTypeMenu,
                   settings.batteryType,
                   BATTERYTYPE_NUM);
#endif
}

int8_t getDeviceBatteryLevel(void)
{
#if defined(BATTERY_REMOVABLE)
    const float *selectedBatteryLevelThresholds = batteryLevelThresholds[settings.batteryType];
#else
    const float *selectedBatteryLevelThresholds = batteryLevelThresholds;
#endif

    float voltage = getDeviceBatteryVoltage();
    int8_t level = (BATTERY_LEVEL_NUM - 1);

    for (uint32_t i = 0; i < (BATTERY_LEVEL_NUM - 1); i++)
    {
        if (voltage < selectedBatteryLevelThresholds[i])
        {
            level = i;

            break;
        }
    }

#if !defined(FONT_SYMBOLS_NOCHARGING)
    if (isDevicePowered() || isBatteryCharging()) 
        level += BATTERY_LEVEL_NUM;
#endif

    return level;
}

// Battery type menu

#if defined(BATTERY_REMOVABLE)

static const char *const batteryTypeMenuOptions[] = {
    "Ni-MH",
    "Alkaline",
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
    "Battery type",
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
