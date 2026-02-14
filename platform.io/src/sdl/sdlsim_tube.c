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
#include "../system/settings.h"

#if !defined(M_PI)
#define M_PI 3.14159265F
#endif

#define SIM_SENSITIVITIY 120.0F
#define SIM_USVH 0.15F
#define SIM_CPS (SIM_USVH * SIM_SENSITIVITIY / 60.0F)

float tubeCPS;
uint32_t tubeLastTick;

void initTubeHardware(void)
{
    srand(time(NULL));

    tubeCPS = SIM_CPS;

    tubeDeadTime = (uint32_t)(0.000075F * PULSE_MEASUREMENT_FREQUENCY);
}

void setTubeHVEnabled(bool value)
{
}

void updateTubeHV(void)
{
}

static float getUniformRandomValue(void)
{
    return (float)rand() / (float)RAND_MAX;
}

static uint32_t getPoisson(double lambda)
{
    // https://en.wikipedia.org/wiki/Poisson_distribution
    double l = exp(-lambda);
    uint32_t n = 0;
    double p = 1;

    do
    {
        n++;
        p = p * getUniformRandomValue();
    } while (p > l);

    return n - 1;
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
        tubeRandomBits = (tubeRandomBits << 1) | (getUniformRandomValue() > 0.5F);

    if (pulseCount)
    {
        uint32_t deltaTicks = currentTick - tubeLastTick;
        if (deltaTicks < 64)
        {
            uint32_t deadTime = 1000 * (deltaTicks - 1) + 1000 * getUniformRandomValue() + 80;
            if(deadTime < tubeDeadTime)
                tubeDeadTime = deadTime;
        }
        tubeLastTick = currentTick;
    }

#endif
}

bool readTubeDet(void)
{
    return false;
}

#endif
