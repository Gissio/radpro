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
    setupBuzzer();
#endif
    selectMenuItem(&soundAlertStyleMenu, settings.soundAlertStyle);
#if defined(VOICE)
    setupVoice();
#endif
}

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

// Sound menu

static const MenuOption soundMenuOptions[] = {
#if defined(BUZZER)
    {STRING_PULSES, &soundPulsesMenu},
#endif
    {STRING_ALERTSTYLE, &soundAlertStyleMenu},
#if defined(VOICE)
    {STRING_ALERTVOLUME, &soundAlertVolumeMenu},
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
