/*
 * Rad Pro
 * Sound
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(SOUND_H)
#define SOUND_H

#include <stdbool.h>

#include "menu.h"

extern View soundMenuView;

void initSound(void);

void resetSound(void);

#if defined(BUZZER)
void initBuzzer(void);

void setBuzzer(bool value);

#if defined(SIMULATOR)
void updateBuzzer(void);
#endif
#endif

#if defined(SOUND_EN)
void initSoundEnabled(void);

void setSoundEnabled(bool value);

void onSoundEnabledTick(void);

void updateSoundEnabled(void);
#endif

#if defined(VOICE)
void initVoice(void);

void onVoiceTick(void);

void playVoiceInstantaneousRate(void);
void playVoiceAverageRate(void);
void playVoiceCumulativeDose(void);
void playAlertTest(void);
void playVoiceTest(void);

void playVoiceAlert(void);
void clearVoiceAlert(void);

void stopVoice(void);
#endif

#endif
