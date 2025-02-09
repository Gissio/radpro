/*
 * Rad Pro
 * Pulse LED
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(VIBRATION)

#include "menu.h"
#include "vibration.h"
#include "settings.h"
#include "tube.h"

static const Menu pulseVibrationMenu;

void initVibration(void)
{
    initVibrationController();

    selectMenuItem(&pulseVibrationMenu,
                   settings.pulseVibration,
                   PULSE_VIBRATION_NUM);
}

// Pulse LED menu

static const char *const pulseVibrationsMenuOptions[] = {
    "Off",
    "Weak",
    "Strong",
    NULL,
};

static const char *onPulseVibrationMenuGetOption(const Menu *menu,
                                           uint32_t index,
                                           MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseVibration);

    return pulseVibrationsMenuOptions[index];
}

static void onPulseVibrationMenuSelect(const Menu *menu)
{
    settings.pulseVibration = menu->state->selectedIndex;
}

static MenuState pulseVibrationsMenuState;

static const Menu pulseVibrationMenu = {
    "Haptic",
    &pulseVibrationsMenuState,
    onPulseVibrationMenuGetOption,
    onPulseVibrationMenuSelect,
    onPulsesSubMenuBack,
};

const View pulseVibrationMenuView = {
    onMenuEvent,
    &pulseVibrationMenu,
};

#endif
