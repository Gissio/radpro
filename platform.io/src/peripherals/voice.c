/*
 * Rad Pro
 * Voice
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(VOICE)

#include "../peripherals/sound.h"
#include "../peripherals/voice.h"
#include "../system/cmath.h"
#include "../system/settings.h"

void setupVoice(void)
{
    selectMenuItem(&soundAlertVolumeMenu, settings.soundAlertVolume);
    selectMenuItem(&soundVoiceVolumeMenu, settings.soundVoiceVolume);
}

// Volume options

static cstring volumeMenuOptions[] = {
    STRING_VERY_LOW,
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH,
};

// Alert volume menu

const char *onAlertVolumeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundAlertVolume);

    return getString(volumeMenuOptions[index]);
}

static void onAlertVolumeMenuSelect(menu_size_t index)
{
    settings.soundAlertVolume = index;

    playAlertTest();
}

static MenuState soundAlertVolumeMenuState;

const Menu soundAlertVolumeMenu = {
    STRING_ALERTVOLUME,
    &soundAlertVolumeMenuState,
    ARRAY_SIZE(volumeMenuOptions),
    onAlertVolumeMenuGetOption,
    onAlertVolumeMenuSelect,
    showSoundMenu,
};

// Voice volume menu

const char *onVoiceVolumeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundVoiceVolume);

    return getString(volumeMenuOptions[index]);
}

static void onVoiceVolumeMenuSelect(menu_size_t index)
{
    settings.soundVoiceVolume = index;

    playVoiceTest();
}

static MenuState soundVoiceVolumeMenuState;

const Menu soundVoiceVolumeMenu = {
    STRING_VOICEVOLUME,
    &soundVoiceVolumeMenuState,
    ARRAY_SIZE(volumeMenuOptions),
    onVoiceVolumeMenuGetOption,
    onVoiceVolumeMenuSelect,
    showSoundMenu,
};

#endif
