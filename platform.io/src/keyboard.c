/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "cmath.h"
#include "events.h"
#include "keyboard.h"
#include "system.h"

#if defined(KEYBOARD_5KEYS)

#define KEY_REPEAT_START ((uint32_t)(0.5 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_REPEAT_PERIOD ((uint32_t)(0.05 * SYSTICK_FREQUENCY / KEY_TICKS))

#define KEY_PRESSED_LONG ((uint32_t)(1.0 * SYSTICK_FREQUENCY / KEY_TICKS))

#elif defined(KEYBOARD_2KEYS)

#define KEY_PRESSED_LONG ((uint32_t)(0.25 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_POWEROFF ((uint32_t)(1.25 * SYSTICK_FREQUENCY / KEY_TICKS))

#endif

static struct
{
    bool isInitialized;

    bool wasKeyDown[KEY_NUM];

    enum Key pressedKey;
    uint32_t pressedTicks;

#if defined(KEYBOARD_5KEYS)

    bool delayedBack;

#endif

    volatile enum Event event;
    volatile enum Event lastEvent;
} keyboard;

void initKeyboard(void)
{
    initKeyboardHardware();

    keyboard.wasKeyDown[KEY_POWER] = true;

    keyboard.pressedKey = KEY_NONE;

    bool isKeyDown[KEY_NUM];
    getKeyboardState(isKeyDown);

    if (isKeyDown[KEY_UP])
        startBootloader();

    keyboard.isInitialized = true;
}

void onKeyboardTick(void)
{
    if (!keyboard.isInitialized)
        return;

    bool isKeyDown[KEY_NUM];

    enum Event event = EVENT_NONE;

    getKeyboardState(isKeyDown);

    for (int32_t i = 0; i < KEY_NUM; i++)
    {
        // Key down

        if (!keyboard.wasKeyDown[i] &&
            isKeyDown[i])
        {
#if defined(KEYBOARD_5KEYS)

            if ((i != KEY_BACK) || !keyboard.delayedBack)
                event = i;

#endif

            keyboard.pressedKey = i;
            keyboard.pressedTicks = 0;
        }

        // Key up

        if (keyboard.wasKeyDown[i] &&
            !isKeyDown[i] &&
            (i == keyboard.pressedKey))
        {
#if defined(KEYBOARD_5KEYS)

            if ((i == KEY_BACK) &&
                keyboard.delayedBack &&
                (keyboard.pressedTicks < KEY_PRESSED_LONG))
                event = EVENT_KEY_BACK;

#elif defined(KEYBOARD_2KEYS)

            if (keyboard.pressedTicks < KEY_PRESSED_LONG)
                event = i;

#endif
            keyboard.pressedKey = KEY_NONE;
        }

        keyboard.wasKeyDown[i] = isKeyDown[i];
    }

    // Key timer

    if (keyboard.pressedKey != KEY_NONE)
    {
        addClamped(&keyboard.pressedTicks, 1);

#if defined(KEYBOARD_5KEYS)

        if (keyboard.pressedTicks >= KEY_REPEAT_START)
        {
            uint32_t repeatTicks = (keyboard.pressedTicks - KEY_REPEAT_START) %
                                   KEY_REPEAT_PERIOD;
            if (repeatTicks == 0)
            {
                if ((keyboard.pressedKey == KEY_UP) || (keyboard.pressedKey == KEY_DOWN))
                    event = keyboard.pressedKey;
            }
        }

        if (keyboard.pressedTicks == KEY_PRESSED_LONG)
        {
            switch (keyboard.pressedKey)
            {
            case KEY_BACK:
                event = EVENT_KEY_RESET;

                break;

            case KEY_POWER:
                event = EVENT_KEY_POWER_OFF;

                break;

            default:
                break;
            }
        }

#elif defined(KEYBOARD_2KEYS)

        if (keyboard.pressedTicks == KEY_PRESSED_LONG)
        {
            if (isKeyDown[KEY_UP])
            {
                if (isKeyDown[KEY_DOWN])
                    event = EVENT_KEY_RESET;
                else
                    event = EVENT_KEY_BACK;
            }
            else
                event = EVENT_KEY_ENTER;
        }
        else if (keyboard.pressedTicks == KEY_PRESSED_POWEROFF)
        {
            if (isKeyDown[KEY_DOWN])
                event = EVENT_KEY_POWER_OFF;
        }

#endif
    }

    // Send key event

    if (event != EVENT_NONE)
        keyboard.event = ((keyboard.event & ~0xf) + 0x10) | event;
}

void setKeyboardDelayedBack(bool value)
{
#if defined(KEYBOARD_5KEYS)

    keyboard.pressedKey = KEY_NONE;

    keyboard.delayedBack = value;

#endif
}

enum Event getKeyboardEvent(void)
{
    enum Event event = keyboard.event;
    if (keyboard.lastEvent != event)
    {
        keyboard.lastEvent = event;

        return event & 0xf;
    }

    return EVENT_NONE;
}
