/*
 * FS2011 Pro
 * Keyboard handling
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>

#ifdef SDL_MODE
#include "SDL.h"
#endif

#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "main.h"

#define KEY_PRESSED_TICKS ((int)(0.5F * SYS_TICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_REPEAT_TICKS ((int)(0.05F * SYS_TICK_FREQUENCY / KEY_TICKS))
#define KEY_LONGPRESS_TICKS ((int)(1 * SYS_TICK_FREQUENCY / KEY_TICKS))

struct
{
    bool wasKeyDown[KEY_EVENT_NUM];

    KeyEvent pressedKey;
    uint32_t pressedTicks;

    volatile KeyEvent keyEvent;
    volatile KeyEvent lastKeyEvent;
} keyboard;

void initKeyboard(void)
{
#ifndef SDL_MODE
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    KEY_UP_PIN);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    KEY_POWER_PIN | KEY_SELECT_PIN | KEY_BACK_PIN | KEY_DOWN_PIN);
#endif

    keyboard.wasKeyDown[KEY_POWER] = 1;

    keyboard.pressedKey = -1;
}

void onKeyboardTick(void)
{
    bool isKeyDown[KEY_EVENT_NUM];
    KeyEvent keyEvent = -1;

#ifndef SDL_MODE
    isKeyDown[KEY_POWER] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_SELECT] = !gpio_get(KEY_SELECT_PORT, KEY_SELECT_PIN);
    isKeyDown[KEY_BACK] = !gpio_get(KEY_BACK_PORT, KEY_BACK_PIN);
#else
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    isKeyDown[KEY_POWER] = state[SDL_SCANCODE_SPACE];
    isKeyDown[KEY_UP] = state[SDL_SCANCODE_UP];
    isKeyDown[KEY_DOWN] = state[SDL_SCANCODE_DOWN];
    isKeyDown[KEY_SELECT] = state[SDL_SCANCODE_RIGHT];
    isKeyDown[KEY_BACK] = state[SDL_SCANCODE_LEFT];
#endif

    for (int32_t i = 0; i < KEY_EVENT_NUM; i++)
    {
        // Key down
        if (!keyboard.wasKeyDown[i] && isKeyDown[i])
        {
            keyEvent = i;

            keyboard.pressedKey = i;
            keyboard.pressedTicks = 0;
        }

        // Key up
        if (keyboard.wasKeyDown[i] && !isKeyDown[i] && (i == keyboard.pressedKey))
        {
            if ((keyboard.pressedKey == KEY_BACK) &&
                (keyboard.pressedTicks < KEY_PRESSED_TICKS))
                keyEvent = KEY_BACK_DELAYED;

            keyboard.pressedKey = -1;
        }

        keyboard.wasKeyDown[i] = isKeyDown[i];
    }

    if (keyboard.pressedKey >= 0)
    {
        keyboard.pressedTicks++;

        if (keyboard.pressedTicks >= KEY_PRESSED_TICKS)
        {
            uint32_t repeatTicks = (keyboard.pressedTicks - KEY_PRESSED_TICKS) %
                                   KEY_PRESSED_REPEAT_TICKS;
            if (repeatTicks == 0)
            {
                if ((keyboard.pressedKey == KEY_UP) || (keyboard.pressedKey == KEY_DOWN))
                    keyEvent = keyboard.pressedKey;
            }
        }

        if (keyboard.pressedTicks == KEY_LONGPRESS_TICKS)
        {
            switch (keyboard.pressedKey)
            {
            case KEY_BACK:
                keyEvent = KEY_RESET;
                break;

            case KEY_POWER:
                keyEvent = KEY_POWER_OFF;
                break;
            }
        }
    }

    if (keyEvent >= 0)
        keyboard.keyEvent = ((keyboard.keyEvent & ~0xf) + 0x10) | keyEvent;
}

KeyEvent getKeyEvent(void)
{
    KeyEvent keyEvent = keyboard.keyEvent;
    if (keyboard.lastKeyEvent != keyEvent)
    {
        keyboard.lastKeyEvent = keyEvent;

        updateBacklight();

        return keyEvent & 0xf;
    }

    return -1;
}
