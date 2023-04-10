/*
 * FS2011 Pro
 * SDL main module
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include "../cubeide/Core/fs2011pro/display.h"
#include "../cubeide/Core/fs2011pro/events.h"
#include "../cubeide/Core/fs2011pro/game.h"
#include "../cubeide/Core/fs2011pro/keyboard.h"
#include "../cubeide/Core/fs2011pro/measurements.h"
#include "../cubeide/Core/fs2011pro/power.h"
#include "../cubeide/Core/fs2011pro/settings.h"
#include "../cubeide/Core/fs2011pro/sim.h"
#include "../cubeide/Core/fs2011pro/menus.h"
#include "../cubeide/Core/fs2011pro/ui.h"

#include "SDL.h"

unsigned int sdlTimer;

int u8g_sdl_get_key();

void onSDLTick()
{
    const float cpmPerUSVh = 60.0F;

    if (sdlTimer < SDL_GetTicks())
    {
        u8g_sdl_get_key();

        simPulses(1.0F * cpmPerUSVh / 60);
        onEventsTick();

        sdlTimer++;
    }
}

void waitSDLTick()
{
    onSDLTick();

    while (sdlTimer == SDL_GetTicks());
}

int main(int argc, char *argv[])
{
    sdlTimer = SDL_GetTicks();

    initKeyboard();
    initPower();
    initDisplay();

    readSettings();

    initEvents();
    initMeasurements();
    initMenus();

    while (true)
    {
        updateGame();
        updateUI();
    }

    return 0;
}
