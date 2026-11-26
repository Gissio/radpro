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

enum
{
    PULSESOUNDENABLE_OFF,
    PULSESOUNDENABLE_ON,
    PULSESOUNDENABLE_STRETCHERFIX,
};

// Applied by the tick, the pin's only writer
static volatile uint8_t pulseSoundEnableState;

void updatePulseSoundEnable(void)
{
    uint8_t state = PULSESOUNDENABLE_OFF;

    if (isMeasurementsEnabled() && settings.pulseSound && isPulseThresholdExceeded())
        state = (getInstantaneousRate() >= STRETCHERFIX_MIN_RATE)
                    ? PULSESOUNDENABLE_STRETCHERFIX
                    : PULSESOUNDENABLE_ON;

    pulseSoundEnableState = state;
}

void onPulseSoundEnableTick(void)
{
    uint8_t state = pulseSoundEnableState;

    setPulseSoundEnable((state == PULSESOUNDENABLE_STRETCHERFIX)
                            ? getRandomBit()
                            : state);
}

#endif
