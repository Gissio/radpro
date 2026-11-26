/*
 * Rad Pro
 * Pulse sound enable
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(PULSESOUND_ENABLE_H)
#define PULSESOUND_ENABLE_H

#include <stdbool.h>

// The platform sets the pin's disabled level before initEvents(): the tick
// drives it from then on (state starts disabled)
void setPulseSoundEnable(bool value);

void updatePulseSoundEnable(void);
void onPulseSoundEnableTick(void);

#endif
