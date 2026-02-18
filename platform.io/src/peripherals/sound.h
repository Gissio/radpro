/*
 * Rad Pro
 * Sound
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(SOUND_H)
#define SOUND_H

#if defined(BUZZER) || defined(PULSESOUND_ENABLE) || defined(VOICE)
#define SOUND
#endif

void initSound(void);

void setupSound(void);

void showSoundMenu(void);

#endif
