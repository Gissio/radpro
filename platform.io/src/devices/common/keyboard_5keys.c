/*
 * Rad Pro
 * 5-key keyboard management
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "../../events.h"
#include "../../keyboard.h"

#define KEY_REPEAT_START ((uint32_t)(0.5F * SYS_TICK_FREQUENCY / KEY_TICKS))
#define KEY_REPEAT_INTERVAL ((uint32_t)(0.05F * SYS_TICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_LONG ((uint32_t)(1 * SYS_TICK_FREQUENCY / KEY_TICKS))

static struct
{
    bool wasKeyDown[KEY_NUM];

    KeyEvent pressedKey;
    uint32_t pressedTicks;

    volatile KeyEvent keyEvent;
    volatile KeyEvent lastKeyEvent;
} keyboard;

void initKeyboard(void)
{
    initKeyboardKeyDown();

    keyboard.wasKeyDown[KEY_POWER] = true;

    keyboard.pressedKey = -1;

    startKeyboardTimer();
}

void onKeyboardTick(void)
{
    bool isKeyDown[KEY_NUM];
    KeyEvent keyEvent = -1;

    getKeyboardKeyDown(isKeyDown);

    for (int32_t i = 0; i < KEY_NUM; i++)
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
            keyboard.pressedKey = -1;

        keyboard.wasKeyDown[i] = isKeyDown[i];
    }

    if (keyboard.pressedKey >= 0)
    {
        keyboard.pressedTicks++;

        if (keyboard.pressedTicks >= KEY_REPEAT_START)
        {
            uint32_t repeatTicks = (keyboard.pressedTicks - KEY_REPEAT_START) %
                                   KEY_REPEAT_INTERVAL;
            if (repeatTicks == 0)
            {
                if ((keyboard.pressedKey == KEY_UP) || (keyboard.pressedKey == KEY_DOWN))
                    keyEvent = keyboard.pressedKey;
            }
        }

        if (keyboard.pressedTicks == KEY_PRESSED_LONG)
        {
            switch (keyboard.pressedKey)
            {
            case KEY_BACK:
                keyEvent = KEY_EVENT_RESET;

                break;

            case KEY_POWER:
                keyEvent = KEY_EVENT_POWER_OFF;

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

        return keyEvent & 0xf;
    }

    return -1;
}
