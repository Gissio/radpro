/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "events.h"
#include "keyboard.h"
#include "system.h"

#define KEY_REPEAT_START ((uint32_t)(0.5 * SYS_TICK_FREQUENCY / KEY_TICKS))
#define KEY_REPEAT_PERIOD ((uint32_t)(0.05 * SYS_TICK_FREQUENCY / KEY_TICKS))

#if defined(KEYBOARD_2KEYS)

#define KEY_PRESSED_LONG ((uint32_t)(0.25 * SYS_TICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_POWEROFF ((uint32_t)(1.25 * SYS_TICK_FREQUENCY / KEY_TICKS))

#elif defined(KEYBOARD_5KEYS)

#define KEY_PRESSED_POWEROFF ((uint32_t)(1.0 * SYS_TICK_FREQUENCY / KEY_TICKS))

#endif

static struct
{
    bool isInitialized;

    bool wasKeyDown[KEY_NUM];

    enum Key pressedKey;
    uint32_t pressedTicks;

    volatile enum Event event;
    volatile enum Event lastEvent;
} keyboard;

void initKeyboard(void)
{
    initKeyboardHardware();

    keyboard.wasKeyDown[KEY_POWER] = true;

    keyboard.pressedKey = KEY_NONE;

    bool isKeyDown[KEY_NUM];
    getKeyboardKeys(isKeyDown);

    if (isKeyDown[KEY_UP])
        startBootloader();

    keyboard.isInitialized = true;
}

void onKeyboardTick(void)
{
    if (!keyboard.isInitialized)
        return;

    bool isKeyDown[KEY_NUM];

#if defined(KEYBOARD_2KEYS)

    bool isAnyKeyDown = false;

#endif

    enum Event event = EVENT_NONE;

    getKeyboardKeys(isKeyDown);

    for (int32_t i = 0; i < KEY_NUM; i++)
    {
        // Key down

        if (!keyboard.wasKeyDown[i] &&
            isKeyDown[i])
        {
#if defined(KEYBOARD_5KEYS)

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
#if defined(KEYBOARD_2KEYS)

            event = i;

#endif
            keyboard.pressedKey = KEY_NONE;
        }

        keyboard.wasKeyDown[i] = isKeyDown[i];

#if defined(KEYBOARD_2KEYS)

        if (isKeyDown[i])
            isAnyKeyDown = true;

#endif
    }

#if defined(KEYBOARD_2KEYS)

    if (isAnyKeyDown)
    {
        keyboard.pressedTicks++;

        if (keyboard.pressedTicks == KEY_PRESSED_LONG)
        {
            if (isKeyDown[KEY_UP])
            {
                if (isKeyDown[KEY_DOWN])
                    event = EVENT_RESET;
                else
                    event = EVENT_BACK;
            }
            else
                event = EVENT_ENTER;

            keyboard.pressedKey = KEY_NONE;
        }
        else if (keyboard.pressedTicks == KEY_PRESSED_POWEROFF)
        {
            if (isKeyDown[KEY_DOWN])
                event = EVENT_POWER_OFF;
        }
    }

#elif defined(KEYBOARD_5KEYS)

    if (keyboard.pressedKey != KEY_NONE)
    {
        keyboard.pressedTicks++;

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

        if (keyboard.pressedTicks == KEY_PRESSED_POWEROFF)
        {
            switch (keyboard.pressedKey)
            {
            case KEY_BACK:
                event = EVENT_RESET;

                break;

            case KEY_POWER:
                event = EVENT_POWER_OFF;

                break;

            default:
                break;
            }
        }
    }

#endif

    if (event != EVENT_NONE)
        keyboard.event = ((keyboard.event & ~0xf) + 0x10) | event;
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
