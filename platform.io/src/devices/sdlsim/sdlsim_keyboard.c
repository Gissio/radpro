/*
 * Rad Pro
 * SDLSim keyboard interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */


#ifdef SDLSIM

#include "SDL.h"

#include "../../keyboard.h"

void initKeyboardKeyDown(void)
{
}

void getKeyboardKeyDown(bool *isKeyDown)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    isKeyDown[KEY_POWER] = state[SDL_SCANCODE_SPACE];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
    isKeyDown[KEY_ENTER] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_BACK] = state[SDL_SCANCODE_LEFT];
}

#endif
