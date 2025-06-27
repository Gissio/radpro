/*
 * Rad Pro
 * Voice
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(VOICE_H)
#define VOICE_H

void initVoice(void);
void onVoiceTick(void);

void updateVoiceVolume(void);

void playVoiceInstantaneousRate(void);
void playVoiceAverageRate(void);
void playVoiceCumulativeDose(void);

void playVoiceAlarm(void);

#endif
