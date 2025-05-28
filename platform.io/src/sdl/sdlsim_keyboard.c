/*
 * Rad Pro
 * Simulator keyboard
 *
 * (C) 2022-2025 Gissio
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

#if defined(KEYBOARD_5KEYS)
    isKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    isKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
    isKeyDown[KEY_OK] = state[SDL_SCANCODE_RETURN];
#elif defined(KEYBOARD_4KEYS)
    isKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    isKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
#elif defined(KEYBOARD_3KEYS)
    isKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    isKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_OK] = state[SDL_SCANCODE_DOWN];
#elif defined(KEYBOARD_2KEYS)
    isKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    isKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
#endif
}

#endif
