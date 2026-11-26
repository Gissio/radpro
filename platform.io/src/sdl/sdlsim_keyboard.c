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

static const SDL_Scancode keyScancodes[KEY_NUM] = {
    [KEY_LEFT] = SDL_SCANCODE_LEFT,
    [KEY_RIGHT] = SDL_SCANCODE_RIGHT,
#if defined(KEYBOARD_4_KEYS) || defined(KEYBOARD_5_KEYS)
    [KEY_UP] = SDL_SCANCODE_UP,
    [KEY_DOWN] = SDL_SCANCODE_DOWN,
#endif
#if defined(KEYBOARD_5_KEYS)
    [KEY_OK] = SDL_SCANCODE_RETURN,
#elif defined(KEYBOARD_3_KEYS)
    [KEY_OK] = SDL_SCANCODE_DOWN,
#endif
};

uint32_t readKeyboardKeys(void)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    uint32_t keys = 0;
    for (uint32_t i = 0; i < KEY_NUM; i++)
        keys |= (state[keyScancodes[i]] != 0) << i;

    return keys;
}

#endif
