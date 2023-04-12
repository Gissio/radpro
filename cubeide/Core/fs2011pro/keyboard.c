/*
 * FS2011 Pro
 * Keyboard handling
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifndef SDL_MODE
#include "main.h"
#else
#include "SDL.h"
#include "u8g2/u8g2.h"
#endif

#include "events.h"
#include "keyboard.h"
#include "power.h"

#define KEY_PRESSED_TICKS ((int)(0.5F * TICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_REPEAT_TICKS ((int)(0.05F * TICK_FREQUENCY / KEY_TICKS))
#define KEY_LONGPRESS_TICKS ((int)(1 * TICK_FREQUENCY / KEY_TICKS))

struct Keyboard
{
    bool wasKeyDown[KEY_NUM];

    signed char pressedKey;
    uint32_t pressedTicks;
    uint32_t pressedRepeatTicks;
} keyboard;

void initKeyboard()
{
    powerWait(500);

    keyboard.wasKeyDown[KEY_POWER] = 1;

    keyboard.pressedKey = -1;
}

int getKeyboardKey()
{
    bool isKeyDown[KEY_NUM];
    int key = -1;

#ifndef SDL_MODE
    isKeyDown[KEY_POWER] = !HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port, KEY_POWER_Pin);
    isKeyDown[KEY_UP] = !HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
    isKeyDown[KEY_DOWN] = !HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin);
    isKeyDown[KEY_SELECT] = !HAL_GPIO_ReadPin(KEY_SELECT_GPIO_Port, KEY_SELECT_Pin);
    isKeyDown[KEY_BACK] = !HAL_GPIO_ReadPin(KEY_BACK_GPIO_Port, KEY_BACK_Pin);
#else
    u8g_sdl_get_key();

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    isKeyDown[KEY_POWER] = state[SDL_SCANCODE_SPACE];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
    isKeyDown[KEY_SELECT] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_BACK] = state[SDL_SCANCODE_LEFT];
#endif

    for (int i = 0; i < KEY_NUM; i++)
    {
        // Key down
        if (!keyboard.wasKeyDown[i] && isKeyDown[i])
        {
            key = i;

            keyboard.pressedKey = i;
            keyboard.pressedTicks = 0;
            keyboard.pressedRepeatTicks = KEY_PRESSED_REPEAT_TICKS;
        }

        // Key up
        if (keyboard.wasKeyDown[i] && !isKeyDown[i] && (i == keyboard.pressedKey))
        {
            if ((keyboard.pressedKey == KEY_BACK) &&
                (keyboard.pressedTicks < KEY_PRESSED_TICKS))
                key = KEY_BACK_UP;

            keyboard.pressedKey = -1;
        }

        keyboard.wasKeyDown[i] = isKeyDown[i];
    }

    if (keyboard.pressedKey >= 0)
    {
        keyboard.pressedTicks++;

        if (keyboard.pressedTicks >= KEY_PRESSED_TICKS)
        {
            keyboard.pressedRepeatTicks++;

            if (keyboard.pressedRepeatTicks >= KEY_PRESSED_REPEAT_TICKS)
            {
                keyboard.pressedRepeatTicks = 0;

                if ((keyboard.pressedKey == KEY_UP) || (keyboard.pressedKey == KEY_DOWN))
                    key = keyboard.pressedKey;
            }
        }

        if (keyboard.pressedTicks == KEY_LONGPRESS_TICKS)
        {
            switch (keyboard.pressedKey)
            {
            case KEY_BACK:
                key = KEY_RESET;
                break;

            case KEY_POWER:
                key = KEY_POWER_OFF;
                break;
            }
        }
    }

#ifdef SDL_MODE
    // if (key >= 0)
    //     printf("Key: %d\n", key);
#endif

    return key;
}
