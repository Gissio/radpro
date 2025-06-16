/*
 * Rad Pro
 * Battery
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "adc.h"
#include "cstring.h"
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
