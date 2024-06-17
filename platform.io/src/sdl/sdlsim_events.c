/*
 * Rad Pro
 * Simulator events
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <SDL.h>

#include "../events.h"

#define TIM_FREQUENCY 8000000

// STM32 8 MHz
// #define SDL_EVENTS_CALLS_MAX 4
// STM32 72 MHz
#define SDL_EVENTS_CALLS_MAX (4 * 9)

extern volatile uint32_t eventsCurrentTick;

float timerCountToSeconds = (1.0F / PULSE_MEASUREMENT_FREQUENCY);

static uint32_t sdlCallsPerTick;

void updateDisplay(void);

void initEventsController(void)
{
}

void sleep(uint32_t value)
{
    if (!value)
    {
        sdlCallsPerTick++;

        if (sdlCallsPerTick < SDL_EVENTS_CALLS_MAX)
            return;

        sdlCallsPerTick = 0;
        value = 1;
    }

    while (value--)
    {
        int32_t deltaTicks = SDL_GetTicks() - eventsCurrentTick;
        eventsCurrentTick++;

        onTick();

        if (deltaTicks < 0)
            updateDisplay();
    }
}

#endif
