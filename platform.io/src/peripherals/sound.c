/*
 * Rad Pro
 * Sound
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(BUZZER) || defined(PULSESOUND_ENABLE) || defined(VOICE)

#include "../peripherals/buzzer.h"
#include "../peripherals/pulsesoundenable.h"
#include "../peripherals/sound.h"
#include "../peripherals/voice.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/menu.h"

static const Menu soundMenu;
static const Menu soundAlertStyleMenu;

// Volume options

#if defined(BUZZER_VOLUME) || defined(VOICE)
static cstring volumeMenuOptions[] = {
    STRING_VERY_LOW,
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH,
};
#endif

void initSound(void)
{
#if defined(BUZZER)
    initBuzzer();
#endif

#if defined(PULSESOUND_ENABLE)
    initPulseSoundEnable();
    updatePulseSoundEnable();
#endif

#if defined(VOICE)
    initVoice();
#endif
}

void setupSound(void)
{
    selectMenuItem(&soundMenu, 0);

#if defined(BUZZER)
    selectMenuItem(&soundPulseStyleMenu, settings.soundPulseStyle);
#endif
#if defined(BUZZER_VOLUME)
    selectMenuItem(&soundPulseVolumeMenu, settings.soundPulseVolume);
#endif

    selectMenuItem(&soundAlertStyleMenu, settings.soundAlertStyle);
#if defined(BUZZER_VOLUME)
    selectMenuItem(&soundAlertVolumeMenu, settings.soundAlertVolume);
#endif

#if defined(VOICE)
    selectMenuItem(&soundAlertVolumeMenu, settings.soundAlertVolume);
    selectMenuItem(&soundVoiceVolumeMenu, settings.soundVoiceVolume);
#endif
}

// Sound pulse style menu

static cstring soundPulseStyleMenuOptions[] = {
    STRING_CLICKS,
    STRING_CHIRPS,
    STRING_BEEPS,
};

static const char *onSoundPulseStyleMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundPulseStyle);

    return getString(soundPulseStyleMenuOptions[index]);
}

static void onSoundPulseStyleMenuSelect(menu_size_t index)
{
    settings.soundPulseStyle = index;
}

static MenuState soundPulseStyleMenuState;

const Menu soundPulseStyleMenu = {
    STRING_PULSESTYLE,
    &soundPulseStyleMenuState,
    ARRAY_SIZE(soundPulseStyleMenuOptions),
    onSoundPulseStyleMenuGetOption,
    onSoundPulseStyleMenuSelect,
    showSoundMenu,
};

// Sound pulse volume menu

#if defined(BUZZER_VOLUME)

static const char *onSoundPulseVolumeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundPulseVolume);

    return getString(volumeMenuOptions[index]);
}

static void onSoundPulseVolumeMenuSelect(menu_size_t index)
{
    settings.soundPulseVolume = index;
}

static MenuState soundPulseVolumeMenuState;

const Menu soundPulseVolumeMenu = {
    STRING_PULSEVOLUME,
    &soundPulseVolumeMenuState,
    ARRAY_SIZE(volumeMenuOptions),
    onSoundPulseVolumeMenuGetOption,
    onSoundPulseVolumeMenuSelect,
    showSoundMenu,
};

#endif

// Sound alert style menu

#if defined(BUZZER) || defined(VOICE)

static cstring soundAlertStyleMenuOptions[] = {
    STRING_SHORT,
    STRING_LONG,
};

static const char *onSoundAlertStyleMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundAlertStyle);

    return getString(soundAlertStyleMenuOptions[index]);
}

static void onSoundAlertStyleMenuSelect(menu_size_t index)
{
    settings.soundAlertStyle = index;

    triggerAlert(true);
}

static MenuState soundAlertStyleMenuState;

static const Menu soundAlertStyleMenu = {
    STRING_ALERTSTYLE,
    &soundAlertStyleMenuState,
    ARRAY_SIZE(soundAlertStyleMenuOptions),
    onSoundAlertStyleMenuGetOption,
    onSoundAlertStyleMenuSelect,
    showSoundMenu,
};

#endif

// Sound alert volume menu

#if defined(BUZZER_VOLUME) || defined(VOICE)

const char *onAlertVolumeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundAlertVolume);

    return getString(volumeMenuOptions[index]);
}

static void onAlertVolumeMenuSelect(menu_size_t index)
{
    settings.soundAlertVolume = index;

    triggerAlert(true);
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

#endif

// Sound voice volume menu

#if defined(VOICE)

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

// Sound menu

static const MenuOption soundMenuOptions[] = {
#if defined(BUZZER)
    {STRING_PULSESTYLE, &soundPulseStyleMenu},
#endif
#if defined(BUZZER_VOLUME)
    {STRING_PULSEVOLUME, &soundPulseVolumeMenu},
#endif

    {STRING_ALERTSTYLE, &soundAlertStyleMenu},
#if defined(BUZZER_VOLUME) || defined(VOICE)
    {STRING_ALERTVOLUME, &soundAlertVolumeMenu},
#endif

#if defined(VOICE)
    {STRING_VOICEVOLUME, &soundVoiceVolumeMenu},
#endif
};

static const char *onSoundMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(soundMenuOptions[index].title);
}

static void onSoundMenuSelect(menu_size_t index)
{
    showMenu(soundMenuOptions[index].menu);
}

static MenuState soundMenuState;

static const Menu soundMenu = {
    STRING_SOUND,
    &soundMenuState,
    ARRAY_SIZE(soundMenuOptions),
    onSoundMenuGetOption,
    onSoundMenuSelect,
    showSettingsMenu,
};

void showSoundMenu(void)
{
    showMenu(&soundMenu);
}

#endif
