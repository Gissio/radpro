/*
 * Rad Pro
 * Simulator keyboard
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include "SDL.h"

#include "../peripherals/keyboard.h"

void initKeyboardHardware(void)
{
}

void onKeyboardTick(void)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

#if defined(KEYBOARD_5_KEYS)
    keyboardKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    keyboardKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    keyboardKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    keyboardKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
    keyboardKeyDown[KEY_OK] = state[SDL_SCANCODE_RETURN];
#elif defined(KEYBOARD_4_KEYS)
    keyboardKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    keyboardKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    keyboardKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    keyboardKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
#elif defined(KEYBOARD_3_KEYS)
    keyboardKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    keyboardKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
    keyboardKeyDown[KEY_OK] = state[SDL_SCANCODE_DOWN];
#elif defined(KEYBOARD_2_KEYS)
    keyboardKeyDown[KEY_LEFT] = state[SDL_SCANCODE_LEFT];
    keyboardKeyDown[KEY_RIGHT] = state[SDL_SCANCODE_RIGHT];
#endif
}

void updateKeyboardState(void)
{
}

#endif
