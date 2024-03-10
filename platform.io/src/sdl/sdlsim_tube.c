/*
 * Rad Pro
 * Simulator Geiger-Müller tube
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <math.h>
#include <stdlib.h>

#include <SDL.h>

#include "../events.h"
#include "../settings.h"
#include "../tube.h"

#if !defined(M_PI)

#define M_PI 3.14159265F

#endif

#define SIM_CPS 0.3825F

static struct
{
    uint32_t pulseTime;

    uint32_t pulseIndex;
    uint32_t pulseCount;
} tube;

void initTubeController(void)
{
    srand(time(NULL));
}

void setTubeHV(bool value)
{
}

void updateTubeHV(void)
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

    double cps = SIM_CPS;
    if (state[SDL_SCANCODE_5])
        cps *= 100000;
    else if (state[SDL_SCANCODE_4])
        cps *= 10000;
    else if (state[SDL_SCANCODE_3])
        cps *= 1000;
    else if (state[SDL_SCANCODE_2])
        cps *= 100;
    else if (state[SDL_SCANCODE_1])
        cps *= 10;

    return getPoisson(cps / SYSTICK_FREQUENCY);
}

bool getTubePulse(uint32_t *pulseTime)
{
    if (tube.pulseIndex == tube.pulseCount)
    {
        tube.pulseTime += 8000;

        tube.pulseIndex = 0;
        tube.pulseCount = simPulses();

        return false;
    }

    tube.pulseIndex++;

    *pulseTime =
        tube.pulseTime +
        (uint32_t)(8000 * (tube.pulseIndex + getUniformRandomValue()) /
                   tube.pulseCount);

    return true;
}

#endif
