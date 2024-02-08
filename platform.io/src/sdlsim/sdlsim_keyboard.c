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

    isKeyDown[KEY_UP] = state[SDL_SCANCODE_LEFT];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_RIGHT];

#elif defined(KEYBOARD_5KEYS)

    isKeyDown[KEY_POWER] = state[SDL_SCANCODE_SPACE];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
    isKeyDown[KEY_ENTER] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_BACK] = state[SDL_SCANCODE_LEFT];

#endif
}

#endif
