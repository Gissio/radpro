/*
 * Rad Pro
 * Simulator Geiger-Müller tube
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "../events.h"
#include "../settings.h"
#include "../tube.h"

#if !defined(M_PI)
#define M_PI 3.14159265F
#endif

#define SIM_CONVERSION_FACTOR 153.8F
#define SIM_USVH 0.15F
#define SIM_CPS (SIM_USVH * SIM_CONVERSION_FACTOR / 60.0F)

float tubeCPS;

static struct
{
    uint32_t pulseTime;

    uint32_t pulseIndex;
    uint32_t pulseCount;
} tube;

void initTubeController(void)
{
    srand(time(NULL));

    tubeCPS = SIM_CPS;

    updateTubeHV();
}

void setTubeHV(bool value)
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

bool getTubePulse(uint32_t *pulseTime)
{
    if (tube.pulseIndex == tube.pulseCount)
    {
        tube.pulseTime += 8000;

        tube.pulseIndex = 0;
        tube.pulseCount = getPoisson(tubeCPS / SYSTICK_FREQUENCY);

        return false;
    }

    tube.pulseIndex++;

    *pulseTime =
        tube.pulseTime +
        (uint32_t)(8000 * (tube.pulseIndex + getUniformRandomValue()) /
                   tube.pulseCount);

    return true;
}

bool getTubeDet(void)
{
    return false;
}

#endif
