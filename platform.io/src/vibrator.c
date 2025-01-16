/*
 * Rad Pro
 * Pulse LED
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(VIBRATOR)

#include "menu.h"
#include "vibrator.h"
#include "settings.h"
#include "tube.h"

static const Menu pulseVibrationsMenu;

void initVibrator(void)
{
    initVibratorController();

    selectMenuItem(&pulseVibrationsMenu,
                   settings.pulseVibrations,
                   PULSE_VIBRATIONS_NUM);
}

// Pulse LED menu

static const char *const pulseVibrationsMenuOptions[] = {
    "Off",
    "Weak",
    "Strong",
    NULL,
};

static const char *onPulseVibrationsMenuGetOption(const Menu *menu,
                                           uint32_t index,
                                           MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseVibrations);

    return pulseVibrationsMenuOptions[index];
}

static void onPulseVibrationsMenuSelect(const Menu *menu)
{
    settings.pulseVibrations = menu->state->selectedIndex;
}

static MenuState pulseVibrationsMenuState;

static const Menu pulseVibrationsMenu = {
    "Haptic",
    &pulseVibrationsMenuState,
    onPulseVibrationsMenuGetOption,
    onPulseVibrationsMenuSelect,
    onPulsesSubMenuBack,
};

const View pulseVibrationsMenuView = {
    onMenuEvent,
    &pulseVibrationsMenu,
};

#endif
