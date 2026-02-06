/*
 * Rad Pro
 * Pulse sound enable
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(PULSESOUND_ENABLE)

#include "../devices/pulsesoundenable.h"
#include "../measurements/instantaneous.h"
#include "../system/cmath.h"
#include "../system/power.h"
#include "../system/settings.h"

#define PULSE_SOUND_LENGTH 0.001F
#define STRETCHERFIX_MIN_RATE (1.0F / PULSE_SOUND_LENGTH)

bool pulseSoundEnableStretcherFix;

void updatePulseSoundEnable(void)
{
    bool enabled = isPoweredOn() && settings.pulseSound && isPulseThresholdExceeded();

    pulseSoundEnableStretcherFix = enabled && (getInstantaneousRate() >= STRETCHERFIX_MIN_RATE);

    if (!pulseSoundEnableStretcherFix)
        setPulseSoundEnable(enabled);
}

void onPulseSoundEnableTick(void)
{
    if (pulseSoundEnableStretcherFix)
        setPulseSoundEnable(getRandomBit());
}

#endif
