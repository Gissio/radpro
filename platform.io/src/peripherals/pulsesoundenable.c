/*
 * Rad Pro
 * Pulse sound enable
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(PULSESOUND_ENABLE)

#include "../measurements/instantaneous.h"
#include "../measurements/measurements.h"
#include "../peripherals/pulsesoundenable.h"
#include "../system/cmath.h"
#include "../system/settings.h"

#define PULSE_SOUND_LENGTH 0.001F
#define STRETCHERFIX_MIN_RATE (1.0F / PULSE_SOUND_LENGTH)

bool pulseSoundEnableStretcherFix;

void updatePulseSoundEnable(void)
{
    bool enabled = isMeasurementsEnabled() && settings.pulseSound && isPulseThresholdExceeded();

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
