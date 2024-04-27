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

// STM32 8 MHz
// #define SDL_EVENTS_CALLS_MAX 4

// STM32 72 MHz
#define SDL_EVENTS_CALLS_MAX (4 * 9)

#define TIM_FREQUENCY 8000000

extern volatile uint32_t eventsCurrentTick;

float timerCountToSeconds = (1.0F / PULSE_MEASUREMENT_FREQUENCY);

static uint32_t sdlEventsCalls;

void updateDisplay(void);

void initEventsController(void)
{
}

void sleep(uint32_t value)
{
    if (!value)
    {
        sdlEventsCalls++;

        if (sdlEventsCalls < SDL_EVENTS_CALLS_MAX)
            return;

        sdlEventsCalls = 0;
    }

    while (true)
    {
        uint32_t realtimeTick = SDL_GetTicks();

        uint32_t deltaTicks = realtimeTick - eventsCurrentTick;

        while (eventsCurrentTick != realtimeTick)
        {
            eventsCurrentTick++;

            onTick();
        }

        if (deltaTicks)
            updateDisplay();

        if (value > deltaTicks)
            value -= deltaTicks;
        else
            return;
    }
}

#endif
