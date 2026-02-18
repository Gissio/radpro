/*
 * Rad Pro
 * Voice
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(VOICE_H)
#define VOICE_H

#include <stdint.h>

#include "../ui/menu.h"

extern const Menu soundAlertVolumeMenu;
extern const Menu soundVoiceVolumeMenu;

void initVoice(void);

void setupVoice(void);

void onVoiceTick(void);

void playVoiceInstantaneousRate(void);
void playVoiceAverageRate(void);
void playVoiceCumulativeDose(void);
void playAlertTest(void);
void playVoiceTest(void);
void playNumber(uint32_t value);

void triggerVoiceAlert(void);
void clearVoiceAlert(void);

void stopVoice(void);

#endif
