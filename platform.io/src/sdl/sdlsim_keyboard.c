/*
 * Rad Pro
 * Simulator keyboard
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include "SDL.h"

#include "../keyboard.h"

void initKeyboardController(void)
{
}

void getKeyboardState(bool *isKeyDown)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    isKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
#if defined(KEYBOARD_5KEYS)
    isKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
#endif
    isKeyDown[KEY_SELECT] = state[SDL_SCANCODE_RETURN];
}

#endif
