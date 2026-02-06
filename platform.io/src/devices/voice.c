/*
 * Rad Pro
 * Voice
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(VOICE)

#include "../devices/sound.h"
#include "../devices/voice.h"
#include "../system/settings.h"
#include "../ui/menu.h"

static Menu soundAlertVolumeMenu;
static Menu soundVoiceVolumeMenu;

void setupVoice(void)
{
    selectMenuItem(&soundAlertVolumeMenu, settings.soundAlertVolume, SOUND_ALERTVOLUME_NUM);
    selectMenuItem(&soundVoiceVolumeMenu, settings.soundVoiceVolume, SOUND_VOICEVOLUME_NUM);
}

// Alert volume menu

static cstring volumeMenuOptions[] = {
    STRING_VERY_LOW,
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH,
    NULL,
};

const char *onAlertVolumeMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundAlertVolume);

    return getString(volumeMenuOptions[index]);
}

static void onAlertVolumeMenuSelect(uint32_t index)
{
    settings.soundAlertVolume = index;

    playAlertTest();
}

static MenuState soundAlertVolumeMenuState;

static Menu soundAlertVolumeMenu = {
    STRING_ALERTVOLUME,
    &soundAlertVolumeMenuState,
    onAlertVolumeMenuGetOption,
    onAlertVolumeMenuSelect,
    setSoundMenu,
};

View soundAlertVolumeMenuView = {
    onMenuEvent,
    &soundAlertVolumeMenu,
};

// Voice volume menu

const char *onVoiceVolumeMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundVoiceVolume);

    return getString(volumeMenuOptions[index]);
}

static void onVoiceVolumeMenuSelect(uint32_t index)
{
    settings.soundVoiceVolume = index;

    playVoiceTest();
}

static MenuState soundVoiceVolumeMenuState;

static Menu soundVoiceVolumeMenu = {
    STRING_VOICEVOLUME,
    &soundVoiceVolumeMenuState,
    onVoiceVolumeMenuGetOption,
    onVoiceVolumeMenuSelect,
    setSoundMenu,
};

View soundVoiceVolumeMenuView = {
    onMenuEvent,
    &soundVoiceVolumeMenu,
};

#endif
