/*
 * Rad Pro
 * Battery management
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "battery.h"
#include "menu.h"
#include "settings.h"

const float batteryLevelVoltageThresholds[2][9] = {
    {1.159F, 1.198F, 1.223F, 1.244F, 1.257F, 1.268F, 1.277F, 1.297F, 1.333F},
    {1.017F, 1.149F, 1.245F, 1.297F, 1.326F, 1.351F, 1.388F, 1.429F, 1.470F},
};

static const struct Menu batteryTypeMenu;

void initBattery(void)
{
    selectMenuIndex(&batteryTypeMenu, settings.batteryType);

    initBatteryHardware();
}

uint8_t getBatteryLevel(void)
{
    if (isBatteryCharging())
        return BATTERY_LEVEL_CHARGING;

    const float *batteryLevelThreshold = batteryLevelVoltageThresholds[settings.batteryType];
    uint8_t level = BATTERY_LEVEL_MAX;
    float voltage = getBatteryVoltage();
    for (uint8_t i = 0; i < BATTERY_LEVEL_MAX; i++)
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

static void onBatteryTypeMenuSelect(const struct Menu *menu)
{
    settings.batteryType = menu->state->selectedIndex;
}

const char *const batteryTypeMenuOptions[] = {
    "Ni-MH",
    "Alkaline",
    NULL,
};

static struct MenuState batteryTypeMenuState;

static const struct Menu batteryTypeMenu = {
    "Battery type",
    &batteryTypeMenuState,
    onMenuGetOption,
    batteryTypeMenuOptions,
    onBatteryTypeMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View batteryTypeMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &batteryTypeMenu,
};
