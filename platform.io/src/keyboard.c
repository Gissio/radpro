/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "cmath.h"
#include "events.h"
#include "keyboard.h"
#include "power.h"
#include "settings.h"

#if defined(KEYBOARD_2KEYS) || defined(KEYBOARD_3KEYS)
#define KEY_PRESSED_LONG ((uint32_t)(0.25 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_EXTENDED ((uint32_t)(1.0 * SYSTICK_FREQUENCY / KEY_TICKS))
#elif defined(KEYBOARD_4KEYS) || defined(KEYBOARD_5KEYS)
#define KEY_REPEAT_START ((uint32_t)(0.5 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_REPEAT_PERIOD ((uint32_t)(0.05 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_LONG ((uint32_t)(1.0 * SYSTICK_FREQUENCY / KEY_TICKS))
#endif

#define EVENT_QUEUE_SIZE 8
#define EVENT_QUEUE_MASK (EVENT_QUEUE_SIZE - 1)

static struct
{
    bool isInitialized;

    bool wasKeyPressed[KEY_NUM];

    KeyboardMode mode;

    Key pressedKey;
    uint32_t pressedTicks;

    volatile uint32_t eventQueueHead;
    volatile uint32_t eventQueueTail;
    volatile Event eventQueue[EVENT_QUEUE_SIZE];
} keyboard;

void initKeyboard(void)
{
    initKeyboardHardware();

#if defined(START_POWERON)
    getKeyboardState(keyboard.wasKeyPressed);

    keyboard.pressedTicks = ((uint32_t)(10.0 * SYSTICK_FREQUENCY / KEY_TICKS));
#endif
    keyboard.pressedKey = KEY_NONE;
    keyboard.isInitialized = true;
}

void onKeyboardTick(void)
{
    if (!keyboard.isInitialized)
        return;

    bool isKeyPressed[KEY_NUM];
    getKeyboardState(isKeyPressed);

    Event event = EVENT_NONE;

    for (int32_t i = 0; i < KEY_NUM; i++)
    {
        // Key pressed
        if (!keyboard.wasKeyPressed[i] &&
            isKeyPressed[i])
        {
#if defined(DISPLAY_MONOCHROME)
            if ((settings.displaySleep != DISPLAY_SLEEP_ALWAYS_OFF) &&
                !isBacklightActive() &&
                !isPoweredOff())
#elif defined(DISPLAY_COLOR)
            if (!isBacklightActive() &&
                !isPoweredOff())
#endif
                event = EVENT_KEY_TOGGLEBACKLIGHT;
            else
            {
#if defined(KEYBOARD_4KEYS) || defined(KEYBOARD_5KEYS)
                if (keyboard.mode == KEYBOARD_MODE_MENU)
                    if ((i == KEY_UP) || (i == KEY_DOWN))
                        event = i;
#endif

                keyboard.pressedTicks = 0;
                keyboard.pressedKey = i;
            }
        }

        // Key released
        if (keyboard.wasKeyPressed[i] &&
            !isKeyPressed[i] &&
            (i == keyboard.pressedKey))
        {
#if defined(KEYBOARD_2KEYS)
            if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
            {
                if (keyboard.pressedTicks < KEY_PRESSED_LONG)
                {
                    if (i == KEY_RIGHT)
                        event = EVENT_KEY_BACK;
                }
                if (keyboard.pressedTicks < KEY_PRESSED_EXTENDED)
                {
                    if (i == KEY_LEFT)
                        event = EVENT_KEY_DOWN;
                }
            }
            else
            {
                if (keyboard.pressedTicks < KEY_PRESSED_LONG)
                {
                    if (i == KEY_LEFT)
                        event = EVENT_KEY_UP;
                    else if (i == KEY_RIGHT)
                        event = EVENT_KEY_DOWN;
                }
            }
#elif defined(KEYBOARD_3KEYS)
            if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
            {
                if (keyboard.pressedTicks < KEY_PRESSED_LONG)
                {
                    if (i == KEY_RIGHT)
                        event = EVENT_KEY_DOWN;
                }
                if (keyboard.pressedTicks < KEY_PRESSED_EXTENDED)
                {
                    if (i == KEY_LEFT)
                        event = EVENT_KEY_UP;
                    else if (i == KEY_OK)
                        event = EVENT_KEY_BACK;
                }
            }
            else
            {
                if (keyboard.pressedTicks < KEY_PRESSED_LONG)
                {
                    if (i == KEY_LEFT)
                        event = EVENT_KEY_UP;
                    else if (i == KEY_RIGHT)
                        event = EVENT_KEY_DOWN;
                }
                if (keyboard.pressedTicks < KEY_PRESSED_EXTENDED)
                {
                    if (i == KEY_OK)
                        event = EVENT_KEY_SELECT;
                }
            }
#elif defined(KEYBOARD_4KEYS) || defined(KEYBOARD_5KEYS)
            if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
            {
                if (keyboard.pressedTicks < KEY_PRESSED_LONG)
                {
                    if (i == KEY_LEFT)
                        event = EVENT_KEY_BACK;
                    else if (i == KEY_RIGHT)
                        event = EVENT_KEY_SELECT;
                    else if (i == KEY_UP)
                        event = EVENT_KEY_UP;
                    else if (i == KEY_DOWN)
                        event = EVENT_KEY_DOWN;
#if defined(KEYBOARD_5KEYS)
                    else if (i == KEY_OK)
                        event = EVENT_KEY_TOGGLEBACKLIGHT;
#endif
                }
            }
            else
            {
                if (keyboard.pressedTicks < KEY_PRESSED_LONG)
                {
                    if (i == KEY_LEFT)
                        event = EVENT_KEY_BACK;
                    else if (i == KEY_RIGHT)
                        event = EVENT_KEY_SELECT;
#if defined(KEYBOARD_5KEYS)
                    else if (i == KEY_OK)
                        event = EVENT_KEY_SELECT;
#endif
                }
            }
#endif

            keyboard.pressedKey = KEY_NONE;
        }

        keyboard.wasKeyPressed[i] = isKeyPressed[i];
    }

    // Key timeout
    if (keyboard.pressedKey != KEY_NONE)
    {
        keyboard.pressedTicks += 1;

#if defined(KEYBOARD_2KEYS)
        if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (keyboard.pressedTicks == KEY_PRESSED_LONG)
            {
                if (!isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_SELECT;
            }
            else if (keyboard.pressedTicks == KEY_PRESSED_EXTENDED)
            {
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (isKeyPressed[KEY_LEFT])
                    event = EVENT_KEY_RESET;
                else if (isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_POWER;
            }
        }
        else
        {
            if (keyboard.pressedTicks == KEY_PRESSED_LONG)
            {
                if (isKeyPressed[KEY_LEFT] && !isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_BACK;
                else if (!isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_SELECT;
            }
            else if (keyboard.pressedTicks == KEY_PRESSED_EXTENDED)
            {
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_POWER;
            }
        }
#elif defined(KEYBOARD_3KEYS)
        if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (keyboard.pressedTicks == KEY_PRESSED_LONG)
            {
                if (isKeyPressed[KEY_RIGHT] && !isKeyPressed[KEY_LEFT])
                    event = EVENT_KEY_SELECT;
            }
            else if (keyboard.pressedTicks == KEY_PRESSED_EXTENDED)
            {
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_OK])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_TOGGLEPULSECLICKS;
                else if (isKeyPressed[KEY_LEFT])
                    event = EVENT_KEY_RESET;
                else if (isKeyPressed[KEY_OK])
                    event = EVENT_KEY_POWER;
            }
        }
        else
        {
            if (keyboard.pressedTicks == KEY_PRESSED_LONG)
            {
                if (isKeyPressed[KEY_LEFT] && !isKeyPressed[KEY_OK])
                    event = EVENT_KEY_BACK;
                else if (isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_SELECT;
            }
            else if (keyboard.pressedTicks == KEY_PRESSED_EXTENDED)
            {
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_OK])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (isKeyPressed[KEY_OK])
                    event = EVENT_KEY_POWER;
            }
        }
#elif defined(KEYBOARD_4KEYS) || defined(KEYBOARD_5KEYS)
        if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (keyboard.pressedTicks == KEY_PRESSED_LONG)
            {
#if defined(KEYBOARD_4KEYS)
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (keyboard.pressedKey == KEY_UP)
                    event = EVENT_KEY_PLAY;
                else if (keyboard.pressedKey == KEY_DOWN)
                    event = EVENT_KEY_TOGGLEPULSECLICKS;
                else if (keyboard.pressedKey == KEY_LEFT)
                    event = EVENT_KEY_RESET;
                else if (keyboard.pressedKey == KEY_RIGHT)
                    event = EVENT_KEY_POWER;
#elif defined(KEYBOARD_5KEYS)
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_OK])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (keyboard.pressedKey == KEY_LEFT)
                    event = EVENT_KEY_RESET;
                else if (keyboard.pressedKey == KEY_RIGHT)
                    event = EVENT_KEY_TOGGLEPULSECLICKS;
                else if (keyboard.pressedKey == KEY_OK)
                    event = EVENT_KEY_POWER;
#endif
            }
        }
        else
        {
            if (keyboard.pressedTicks >= KEY_REPEAT_START)
            {
                if ((keyboard.pressedKey == KEY_UP) ||
                    (keyboard.pressedKey == KEY_DOWN))
                {
                    uint32_t repeatTicks = (keyboard.pressedTicks - KEY_REPEAT_START) % KEY_REPEAT_PERIOD;
                    if (repeatTicks == 0)
                        event = (Event)keyboard.pressedKey;
                }
            }
#if defined(KEYBOARD_4KEYS)
            if (keyboard.pressedTicks == KEY_PRESSED_LONG)
            {
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_RIGHT])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (keyboard.pressedKey == KEY_RIGHT)
                    event = EVENT_KEY_POWER;
            }
#elif defined(KEYBOARD_5KEYS)
            if (keyboard.pressedTicks == KEY_PRESSED_LONG)
            {
                if (isKeyPressed[KEY_LEFT] && isKeyPressed[KEY_OK])
                    event = EVENT_KEY_TOGGLELOCK;
                else if (keyboard.pressedKey == KEY_OK)
                    event = EVENT_KEY_POWER;
            }
#endif
        }
#endif
    }

    // Enqueue event
    if (event != EVENT_NONE)
    {
        keyboard.eventQueue[keyboard.eventQueueHead] = event;
        keyboard.eventQueueHead = (keyboard.eventQueueHead + 1) & EVENT_QUEUE_MASK;
    }
}

void setKeyboardMode(KeyboardMode mode)
{
#if defined(KEYBOARD_2KEYS)
    if (mode == KEYBOARD_MODE_MEASUREMENT)
        keyboard.pressedKey = KEY_NONE;
#else
    keyboard.pressedKey = KEY_NONE;
#endif

    keyboard.mode = mode;
}

Event getKeyboardEvent(void)
{
    if (keyboard.eventQueueHead != keyboard.eventQueueTail)
    {
        // Dequeue event
        Event event = keyboard.eventQueue[keyboard.eventQueueTail];
        keyboard.eventQueueTail = (keyboard.eventQueueTail + 1) & EVENT_QUEUE_MASK;

        return event;
    }

    return EVENT_NONE;
}
