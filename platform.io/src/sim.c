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

static int getPoisson(float lambda)
{
    double l = exp(-lambda);
    int k = 0;
    double p = 1;

    do
    {
        k++;
        p = p * getUniformRandomValue();
    } while (p > l);

    return k - 1;
}

uint32_t simPulses(uint32_t n)
{
    return getPoisson(n * CPS / SYS_TICK_FREQUENCY);
}
