/*
 * Rad Pro
 * SDLSim event handler
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include "../../events.h"

#include <SDL.h>

void updateDisplay(void);

void initEventsHardware(void)
{
}

void sleep(uint32_t value)
{
    if (!value)
        return;

    uint32_t tickStart = SDL_GetTicks();
    uint32_t tickUpdated = tickStart;

    while (true)
    {
        uint32_t tickCurrent = SDL_GetTicks();

        while ((tickCurrent - tickUpdated) > 0)
        {
            tickUpdated++;
            onTick();
        }

        if ((tickCurrent - tickStart) >= value)
            break;

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                exit(0);
        }

        updateDisplay();
    }
}

#endif
