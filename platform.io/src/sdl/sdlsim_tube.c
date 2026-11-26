/*
 * Rad Pro
 * Simulator Geiger-Müller tube
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "../peripherals/tube.h"
#include "../system/events.h"
#include "sdlsim.h"

#define SIM_SENSITIVITY 120.0F
#define SIM_USVH 0.15F
#define SIM_CPS (SIM_USVH * SIM_SENSITIVITY / 60.0F)

#define SIM_DEAD_TIME 0.000075F

float tubeCPS;

void initTubeHardware(void)
{
    srand(time(NULL));

    tubeCPS = SIM_CPS;

    // Fixed simulated dead time (pulse intervals are not simulated)
    tubeDeadTime = (uint32_t)(SIM_DEAD_TIME * PULSE_MEASUREMENT_FREQUENCY);
}

void setTubeHVEnabled(bool value)
{
}

void updateTubeHV(void)
{
}

// Knuth's algorithm: https://en.wikipedia.org/wiki/Poisson_distribution
static uint32_t getPoisson(double lambda)
{
    double limit = exp(-lambda);
    double p = getUniformRandomValue();
    uint32_t n = 0;

    while (p > limit)
    {
        n++;
        p *= getUniformRandomValue();
    }

    return n;
}

void onTubeTick(void)
{
#if defined(SIMULATE_PULSES)

#if defined(__EMSCRIPTEN__)
    uint32_t tubeTick = currentTick % 120000;
    if (tubeTick == 30000)
        tubeCPS = 100 * SIM_CPS;
    else if (tubeTick == 31000)
        tubeCPS = SIM_CPS;
#endif

    uint32_t pulseCount = getPoisson(tubeCPS / SYSTICK_FREQUENCY);

    tubePulseCount += pulseCount;

    for (uint32_t i = 0; i < pulseCount; i++)
        tubeRandomBits = (tubeRandomBits << TUBE_BITS_PER_PULSE) | (rand() & TUBE_BITS_PER_PULSE_MASK);

#endif
}

bool readTubeDet(void)
{
    return false;
}

#endif
