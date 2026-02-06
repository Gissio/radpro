/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(BUZZER)

#include "../devices/buzzer.h"
#include "../devices/sound.h"
#include "../system/settings.h"
#include "../ui/menu.h"

static Menu soundPulsesMenu;

void setupBuzzer(void)
{
    selectMenuItem(&soundPulsesMenu, settings.soundPulseType, SOUND_PULSETYPE_NUM);
}

// Sound pulses menu

static cstring soundPulsesMenuOptions[] = {
    STRING_CLICKS,
    STRING_CHIRPS,
    STRING_BEEPS,
    NULL,
};

static const char *onSoundPulsesMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundPulseType);

    return getString(soundPulsesMenuOptions[index]);
}

static void onSoundPulsesMenuSelect(uint32_t index)
{
    settings.soundPulseType = index;
}

static MenuState soundPulsesMenuState;

static Menu soundPulsesMenu = {
    STRING_PULSES,
    &soundPulsesMenuState,
    onSoundPulsesMenuGetOption,
    onSoundPulsesMenuSelect,
    setSoundMenu,
};

View soundPulsesMenuView = {
    onMenuEvent,
    &soundPulsesMenu,
};

#endif
