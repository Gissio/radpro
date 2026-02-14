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
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/menu.h"

static Menu soundMenu;
static Menu soundAlertStyleMenu;

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
    selectMenuItem(&soundMenu, 0, 0);
#if defined(BUZZER)
    setupBuzzer();
#endif
    selectMenuItem(&soundAlertStyleMenu, settings.soundAlertStyle, SOUND_ALERTSTYLE_NUM);
#if defined(VOICE)
    setupVoice();
#endif
}

// Sound alert style menu

#if defined(BUZZER) || defined(VOICE)

static cstring soundAlertStyleMenuOptions[] = {
    STRING_SHORT,
    STRING_LONG,
    NULL,
};

static const char *onSoundAlertStyleMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundAlertStyle);

    return getString(soundAlertStyleMenuOptions[index]);
}

static void onSoundAlertStyleMenuSelect(uint32_t index)
{
    settings.soundAlertStyle = index;

    triggerAlert(true);
}

static MenuState soundAlertStyleMenuState;

static Menu soundAlertStyleMenu = {
    STRING_ALERTSTYLE,
    &soundAlertStyleMenuState,
    onSoundAlertStyleMenuGetOption,
    onSoundAlertStyleMenuSelect,
    setSoundMenu,
};

static View soundAlertStyleMenuView = {
    onMenuEvent,
    &soundAlertStyleMenu,
};

#endif

// Sound menu

static MenuOption soundMenuOptions[] = {
#if defined(BUZZER)
    {STRING_PULSES, &soundPulsesMenuView},
#endif
    {STRING_ALERTSTYLE, &soundAlertStyleMenuView},
#if defined(VOICE)
    {STRING_ALERTVOLUME, &soundAlertVolumeMenuView},
    {STRING_VOICEVOLUME, &soundVoiceVolumeMenuView},
#endif
    {NULL},
};

static const char *onSoundMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(soundMenuOptions[index].title);
}

static void onSoundMenuSelect(uint32_t index)
{
    setView(soundMenuOptions[index].view);
}

static MenuState soundMenuState;

static Menu soundMenu = {
    STRING_SOUND,
    &soundMenuState,
    onSoundMenuGetOption,
    onSoundMenuSelect,
    setSettingsMenu,
};

View soundMenuView = {
    onMenuEvent,
    &soundMenu,
};

void setSoundMenu(void)
{
    setView(&soundMenuView);
}

#endif
