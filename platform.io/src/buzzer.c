/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "events.h"
#include "menu.h"
#include "settings.h"
#include "tube.h"

static const Menu pulseSoundMenu;

void initBuzzer(void)
{
    initBuzzerController();

    selectMenuItem(&pulseSoundMenu,
                   settings.pulseSound,
                   PULSE_SOUND_NUM);
}

// Pulse clicks menu

static const char *const pulseSoundMenuOptions[] = {
    "Off",
    "Clicks",
    "Beeps",
    NULL,
};

static const char *onPulseSoundMenuGetOption(const Menu *menu,
                                             uint32_t index,
                                             MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.pulseSound);

    return pulseSoundMenuOptions[index];
}

static void onPulseSoundMenuSelect(const Menu *menu)
{
    settings.pulseSound = menu->state->selectedIndex;
}

static MenuState pulseSoundMenuState;

static const Menu pulseSoundMenu = {
    "Sound",
    &pulseSoundMenuState,
    onPulseSoundMenuGetOption,
    onPulseSoundMenuSelect,
    onPulsesSubMenuBack,
};

const View pulseSoundMenuView = {
    onMenuEvent,
    &pulseSoundMenu,
};
