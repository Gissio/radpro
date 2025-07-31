/*
 * Rad Pro
 * Sound
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "cmath.h"
#include "events.h"
#include "measurements.h"
#include "power.h"
#include "settings.h"
#include "sound.h"

#define PULSE_LENGTH 0.001F
#define PULSE_STRETCHERFIX_MINRATE (1.0F / PULSE_LENGTH)

#if defined(SOUND_EN)
bool soundEnabledPulseStretcherFix;
#endif
#if defined(BUZZER)
static Menu soundPulseTypeMenu;
#endif
#if defined(BUZZER) || defined(VOICE)
static Menu soundAlertStyleMenu;
#endif
#if defined(VOICE)
static Menu soundAlertVolumeMenu;
static Menu soundVoiceVolumeMenu;
#endif

void onSoundSubMenuBack(void);

void initSound(void)
{
#if defined(BUZZER)
    initBuzzer();
#endif
#if defined(SOUND_EN)
    initSoundEnabled();

    updateSoundEnabled();
#endif
#if defined(VOICE)
    initVoice();
#endif
}

void resetSound(void)
{
#if defined(BUZZER)
    selectMenuItem(&soundPulseTypeMenu,
                   settings.soundPulseType,
                   SOUND_PULSETYPE_NUM);
#endif
#if defined(BUZZER) || defined(VOICE)
    selectMenuItem(&soundAlertStyleMenu,
                   settings.soundAlertStyle,
                   SOUND_ALERTSTYLE_NUM);
#endif
#if defined(VOICE)
    selectMenuItem(&soundAlertVolumeMenu,
                   settings.soundAlertVolume,
                   SOUND_ALERTVOLUME_NUM);
    selectMenuItem(&soundVoiceVolumeMenu,
                   settings.soundVoiceVolume,
                   SOUND_VOICEVOLUME_NUM);
#endif
}

#if defined(BUZZER)

// Sound pulse type menu

static cstring soundPulseTypeMenuOptions[] = {
    STRING_CLICKS,
    STRING_CHIRPS,
    STRING_BEEPS,
    NULL,
};

static const char *onSoundPulseTypeMenuGetOption(uint32_t index,
                                                 MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundPulseType);

    return getString(soundPulseTypeMenuOptions[index]);
}

static void onSoundPulseTypeMenuSelect(uint32_t index)
{
    settings.soundPulseType = index;
}

static MenuState soundPulseTypeMenuState;

static Menu soundPulseTypeMenu = {
    STRING_PULSES,
    &soundPulseTypeMenuState,
    onSoundPulseTypeMenuGetOption,
    onSoundPulseTypeMenuSelect,
    onSoundSubMenuBack,
};

View soundPulseTypeMenuView = {
    onMenuEvent,
    &soundPulseTypeMenu,
};

#endif

#if defined(SOUND_EN)

// Sound control

void onSoundEnabledTick(void)
{
    if (soundEnabledPulseStretcherFix)
        setSoundEnabled(getRandomBit());
}

void updateSoundEnabled(void)
{
    bool soundEnabled = !isDeviceOff() &&
                        isPulseThresholdExceeded() &&
                        settings.pulseSound;

    soundEnabledPulseStretcherFix = soundEnabled &&
                                   (getInstantaneousRate() >= PULSE_STRETCHERFIX_MINRATE);

    if (!soundEnabledPulseStretcherFix)
        setSoundEnabled(soundEnabled);
}

#endif

#if defined(BUZZER) || defined(VOICE)

// Sound alert style menu

static cstring soundAlertStyleMenuOptions[] = {
    STRING_SHORT,
    STRING_LONG,
    NULL,
};

static const char *onSoundAlertStyleMenuGetOption(uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.soundAlertStyle);

    return getString(soundAlertStyleMenuOptions[index]);
}

static void onSoundAlertStyleMenuSelect(uint32_t index)
{
    settings.soundAlertStyle = index;

    triggerAlert();
}

static MenuState soundAlertStyleMenuState;

static Menu soundAlertStyleMenu = {
    STRING_ALERTSTYLE,
    &soundAlertStyleMenuState,
    onSoundAlertStyleMenuGetOption,
    onSoundAlertStyleMenuSelect,
    onSoundSubMenuBack,
};

View soundAlertStyleMenuView = {
    onMenuEvent,
    &soundAlertStyleMenu,
};

#endif

#if defined(VOICE)

// Volume menu common

static cstring volumeMenuOptions[] = {
    STRING_VERY_LOW,
    STRING_LOW,
    STRING_MEDIUM,
    STRING_HIGH,
    NULL,
};

// Alert volume menu

const char *onAlertVolumeMenuGetOption(uint32_t index,
                                       MenuStyle *menuStyle)
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
    onSoundSubMenuBack,
};

View soundAlertVolumeMenuView = {
    onMenuEvent,
    &soundAlertVolumeMenu,
};

// Voice volume menu

const char *onVoiceVolumeMenuGetOption(uint32_t index,
                                       MenuStyle *menuStyle)
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
    onSoundSubMenuBack,
};

View soundVoiceVolumeMenuView = {
    onMenuEvent,
    &soundVoiceVolumeMenu,
};

#endif

#if defined(BUZZER) || defined(VOICE)

// Sound menu

static SubMenuOption soundMenuOptions[] = {
#if defined(BUZZER)
    {STRING_PULSES, &soundPulseTypeMenuView},
#endif
    {STRING_ALERTSTYLE, &soundAlertStyleMenuView},
#if defined(VOICE)
    {STRING_ALERTVOLUME, &soundAlertVolumeMenuView},
    {STRING_VOICEVOLUME, &soundVoiceVolumeMenuView},
#endif
    {NULL},
};

void onSoundSubMenuBack(void)
{
    setView(&soundMenuView);
}

static const char *onSoundMenuGetOption(uint32_t index,
                                        MenuStyle *menuStyle)
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
    onSettingsSubMenuBack,
};

View soundMenuView = {
    onMenuEvent,
    &soundMenu,
};

#endif
