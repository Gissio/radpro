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

#include <stdbool.h>
#include <stdint.h>

void initVoice(void);

void onVoiceTick(void);

void playVoiceInstantaneousRate(void);
void playVoiceAverageRate(void);
void playVoiceCumulativeDose(void);
void playVoiceTest(void);
void playNumber(uint32_t value);

// measurementAlert: voice alerts may read out the active rate or dose alert
void triggerVoiceAlert(bool measurementAlert);
void clearVoiceAlert(void);

void stopVoice(void);

#endif
