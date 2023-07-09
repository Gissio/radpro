/*
 * Rad Pro
 * SDLSim Geiger-Müller tube interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include <math.h>
#include <stdlib.h>

#include <SDL.h>

#include "../../gm.h"
#include "../../events.h"

#define M_PI 3.14159265358979323846

#define SIM_CPS_LOW 0.25
#define SIM_CPS_HIGH 3000.0

static struct
{
    uint32_t pulseCount;
    uint32_t pulseTime;
} gm;

void initGM(void)
{
}

void syncGMHVPulse(void)
{
}

static float getUniformRandomValue(void)
{
    return (float)rand() / RAND_MAX;
}

// https://en.wikipedia.org/wiki/Poisson_distribution

static uint32_t getPoisson(double lambda)
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
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    bool isHighCPS = state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL];
    double cps = isHighCPS ? SIM_CPS_HIGH : SIM_CPS_LOW;

    return getPoisson(cps / SYS_TICK_FREQUENCY);
}

bool getGMPulse(uint32_t *pulseTime)
{
    if (!gm.pulseCount)
    {
        gm.pulseCount += simPulses();

        return false;
    }

    gm.pulseCount--;

    gm.pulseTime += (uint32_t)(100 * getUniformRandomValue());
    *pulseTime = gm.pulseTime;

    return true;
}

#endif
