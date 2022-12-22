/*
 * FS2011 Pro
 * Radiation simulator
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "events.h"

float getRandom()
{
    return (float)rand() / RAND_MAX;
}

int getPoisson(float lambda)
{
    double l = exp(-lambda);
    int k = 0;
    double p = 1;

    do
    {
        k++;
        p = p * getRandom();
    } while (p > l);

    return k - 1;
}

void simPulses(float cps)
{
    int n = getPoisson(cps / TICK_FREQUENCY);

    for (int i = 0; i < n; i++)
    {
#ifdef SDL_MODE
        // printf("simPulses: triggerPulse\n");
#endif
        triggerPulse();
    }
}
