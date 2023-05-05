/*
 * FS2011 Pro
 * Radiation simulator
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <math.h>
#include <stdlib.h>

#include "events.h"
#include "sim.h"

#define CPS 1.0F

static float getUniformRandomValue(void)
{
    return (float)rand() / RAND_MAX;
}

static uint32_t getPoisson(float lambda)
{
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

uint32_t simPulses(void)
{
    return getPoisson(CPS / SYS_TICK_FREQUENCY);
}
