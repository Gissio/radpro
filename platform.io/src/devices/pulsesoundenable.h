/*
 * Rad Pro
 * Pulse sound enable
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(PULSESOUND_ENABLEABLE_H)
#define PULSESOUND_ENABLEABLE_H

#include <stdbool.h>

void initPulseSoundEnable(void);
void setPulseSoundEnable(bool value);

void updatePulseSoundEnable(void);
void onPulseSoundEnableTick(void);

#endif
