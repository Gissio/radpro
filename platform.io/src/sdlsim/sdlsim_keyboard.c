/*
 * Rad Pro
 * SDLSim keyboard
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include "SDL.h"

#include "../keyboard.h"

void initKeyboardHardware(void)
{
}

void getKeyboardState(bool *isKeyDown)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

#if defined(KEYBOARD_2KEYS)

    isKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    isKeyDown[KEY_SELECT] = state[SDL_SCANCODE_RIGHT];

#elif defined(KEYBOARD_5KEYS)

    isKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    isKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
    isKeyDown[KEY_SELECT] = state[SDL_SCANCODE_SPACE];
 
#endif
}

#endif
