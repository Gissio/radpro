/*
 * Rad Pro
 * SDLSim events
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <SDL.h>

#include "../events.h"

#define SDL_EVENTS_CALLS_MAX 4

extern volatile uint32_t eventsCurrentTick;

static uint32_t sdlEventsCalls;

void updateDisplay(void);

void initEventsHardware(void)
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
        {
            SDL_Event event;
            if (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                    exit(0);
            }

            updateDisplay();
        }

        if (value > deltaTicks)
            value -= deltaTicks;
        else
            return;
    }
}

#endif
