/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(BUZZER)

#include "../peripherals/buzzer.h"
#include "../peripherals/sound.h"
#include "../system/cmath.h"
#include "../system/settings.h"

void setupBuzzer(void)
{
    selectMenuItem(&soundPulsesMenu, settings.soundPulseType);
}

// Sound pulses menu

static cstring soundPulsesMenuOptions[] = {
    STRING_CLICKS,
    STRING_CHIRPS,
    STRING_BEEPS,
};

static const char *onSoundPulsesMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundPulseType);

    return getString(soundPulsesMenuOptions[index]);
}

static void onSoundPulsesMenuSelect(menu_size_t index)
{
    settings.soundPulseType = index;
}

static MenuState soundPulsesMenuState;

const Menu soundPulsesMenu = {
    STRING_PULSES,
    &soundPulsesMenuState,
    ARRAY_SIZE(soundPulsesMenuOptions),
    onSoundPulsesMenuGetOption,
    onSoundPulsesMenuSelect,
    showSoundMenu,
};

#endif
