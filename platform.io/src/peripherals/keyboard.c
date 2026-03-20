/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#include "../peripherals/keyboard.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"

#if defined(KEYBOARD_2_KEYS) || defined(KEYBOARD_3_KEYS)
#define KEY_PRESSED_SHORT ((uint32_t)(0.25 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_LONG ((uint32_t)(1.0 * SYSTICK_FREQUENCY / KEY_TICKS))
#elif defined(KEYBOARD_4_KEYS) || defined(KEYBOARD_5_KEYS)
#define KEY_REPEAT_START ((uint32_t)(0.5 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_REPEAT_PERIOD ((uint32_t)(0.05 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_LONG ((uint32_t)(1.0 * SYSTICK_FREQUENCY / KEY_TICKS))
#endif

#define EVENT_QUEUE_SIZE 8
#define EVENT_QUEUE_MASK (EVENT_QUEUE_SIZE - 1)

#define KEY_PRESSED_MASK ((1 << KEY_TICKS) - 1)

static struct
{
    volatile bool enabled;

    bool previousKeyDown[KEY_NUM];

    KeyboardMode mode;

    Key pressedKey;
    uint32_t pressedTicks;
#if defined(KEYBOARD_4_KEYS) || defined(KEYBOARD_5_KEYS)
    uint32_t nextRepeatTick;
#endif
    bool wasSleeping;

    volatile uint32_t eventQueueHead;
    volatile uint32_t eventQueueTail;
    volatile ViewEvent eventQueue[EVENT_QUEUE_SIZE];
} keyboard;

uint32_t keyboardKeyDown[KEY_NUM];

void initKeyboard(void)
{
    initKeyboardHardware();

    onKeyboardTick();

    keyboard.pressedTicks = ((uint32_t)(10.0 * SYSTICK_FREQUENCY / KEY_TICKS));
    keyboard.pressedKey = KEY_NONE;

    keyboard.enabled = true;
}

bool isPowerKeyDown(void)
{
    return keyboardKeyDown[KEY_POWER];
}

void waitLongKeyPress(void)
{
    sleep(KEY_TICKS * KEY_PRESSED_LONG);
}

// 2-key keyboard
#if defined(KEYBOARD_2_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, ViewEvent event)
{
    uint32_t ticks = keyboard.pressedTicks;

    if (ticks < KEY_PRESSED_SHORT)
    {
        if (key == KEY_RIGHT)
            return EVENT_KEY_DOWN;
        if ((key == KEY_LEFT) && (keyboard.mode != KEYBOARD_MODE_MEASUREMENT))
            return EVENT_KEY_UP;
    }

    if ((ticks < KEY_PRESSED_LONG) && (key == KEY_LEFT) && (keyboard.mode == KEYBOARD_MODE_MEASUREMENT))
        return EVENT_KEY_BACK;

    return event;
}

static ViewEvent getKeyTimerEvent(const bool *keyDown, ViewEvent event)
{
    uint32_t ticks = keyboard.pressedTicks;

    if (ticks == KEY_PRESSED_SHORT)
    {
        if (!keyDown[KEY_LEFT] && keyDown[KEY_RIGHT])
            return EVENT_KEY_SELECT;
        if ((keyboard.mode != KEYBOARD_MODE_MEASUREMENT) && keyDown[KEY_LEFT] && !keyDown[KEY_RIGHT])
            return EVENT_KEY_BACK;
    }
    else if (ticks == KEY_PRESSED_LONG)
    {
        if (keyDown[KEY_LEFT] && keyDown[KEY_RIGHT])
            return EVENT_KEY_TOGGLELOCK;
        if (keyDown[KEY_RIGHT])
            return EVENT_KEY_POWER;
        if ((keyboard.mode == KEYBOARD_MODE_MEASUREMENT) && keyDown[KEY_LEFT])
            return EVENT_KEY_RESET;
    }

    return event;
}

#elif defined(KEYBOARD_3_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, ViewEvent event)
{
    uint32_t ticks = keyboard.pressedTicks;

    if (ticks < KEY_PRESSED_SHORT)
    {
        if (key == KEY_RIGHT)
            return EVENT_KEY_DOWN;
        if ((key == KEY_LEFT) && (keyboard.mode != KEYBOARD_MODE_MEASUREMENT))
            return EVENT_KEY_UP;
    }

    if (ticks < KEY_PRESSED_LONG)
    {
        if ((key == KEY_LEFT) && (keyboard.mode == KEYBOARD_MODE_MEASUREMENT))
            return EVENT_KEY_UP;
        if (key == KEY_OK)
            return (keyboard.mode == KEYBOARD_MODE_MEASUREMENT) ? EVENT_KEY_BACK : EVENT_KEY_SELECT;
    }

    return event;
}

static ViewEvent getKeyTimerEvent(const bool *keyDown, ViewEvent event)
{
    uint32_t ticks = keyboard.pressedTicks;

    if (ticks == KEY_PRESSED_SHORT)
    {
        if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (keyDown[KEY_RIGHT] && !keyDown[KEY_LEFT])
                return EVENT_KEY_SELECT;
        }
        else
        {
            if (keyDown[KEY_LEFT] && !keyDown[KEY_OK])
                return EVENT_KEY_BACK;
            else if (keyDown[KEY_RIGHT])
                return EVENT_KEY_SELECT;
        }
    }
    else if (ticks == KEY_PRESSED_LONG)
    {
        if (keyDown[KEY_LEFT] && keyDown[KEY_OK])
            return EVENT_KEY_TOGGLELOCK;

        if (keyDown[KEY_OK])
            return EVENT_KEY_POWER;

        if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (keyDown[KEY_LEFT] && keyDown[KEY_RIGHT])
                return EVENT_KEY_TOGGLEPULSESOUND;
            else if (keyDown[KEY_LEFT])
                return EVENT_KEY_RESET;
        }
    }

    return event;
}

#elif defined(KEYBOARD_4_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, ViewEvent event)
{
    if (keyboard.pressedTicks < KEY_PRESSED_LONG)
    {
        if (key == KEY_LEFT)
            return EVENT_KEY_BACK;
        else if (key == KEY_RIGHT)
            return EVENT_KEY_SELECT;
        else if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (key == KEY_UP)
                return EVENT_KEY_UP;
            else if (key == KEY_DOWN)
                return EVENT_KEY_DOWN;
        }
    }

    return event;
}

static ViewEvent getKeyTimerEvent(const bool *keyDown, ViewEvent event)
{
    uint32_t ticks = keyboard.pressedTicks;
    Key pressedKey = keyboard.pressedKey;

    if ((keyboard.mode != KEYBOARD_MODE_MEASUREMENT) && (ticks >= KEY_REPEAT_START))
    {
        if ((pressedKey == KEY_UP) || (pressedKey == KEY_DOWN))
        {
            if (ticks >= keyboard.nextRepeatTick)
            {
                keyboard.nextRepeatTick += KEY_REPEAT_PERIOD;
                return EVENT_KEY_START + pressedKey;
            }
        }
    }

    if (ticks == KEY_PRESSED_LONG)
    {
        if (keyDown[KEY_LEFT] && keyDown[KEY_RIGHT])
            return EVENT_KEY_TOGGLELOCK;
        if (pressedKey == KEY_RIGHT)
            return EVENT_KEY_POWER;
        if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (pressedKey == KEY_UP)
                return EVENT_KEY_PLAY;
            else if (pressedKey == KEY_DOWN)
                return EVENT_KEY_TOGGLEPULSESOUND;
            else if (pressedKey == KEY_LEFT)
                return EVENT_KEY_RESET;
        }
    }

    return event;
}

#elif defined(KEYBOARD_5_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, ViewEvent event)
{
    if (keyboard.pressedTicks < KEY_PRESSED_LONG)
    {
        if (key == KEY_LEFT)
            return EVENT_KEY_BACK;
        else if (key == KEY_RIGHT)
            return EVENT_KEY_SELECT;
        else if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (key == KEY_UP)
                return EVENT_KEY_UP;
            else if (key == KEY_DOWN)
                return EVENT_KEY_DOWN;
            else if ((key == KEY_OK) && !keyboard.wasSleeping)
                return EVENT_KEY_TOGGLEBACKLIGHT;
        }
        else
        {
#if !defined(KEYBOARD_KEY_POWER_OK)
            if ((key == KEY_OK) && !keyboard.wasSleeping)
                return EVENT_KEY_TOGGLEBACKLIGHT;
#else
            if (key == KEY_OK)
                return EVENT_KEY_SELECT;
#endif
        }
    }

    return event;
}

static ViewEvent getKeyTimerEvent(const bool *keyDown, ViewEvent event)
{
    uint32_t ticks = keyboard.pressedTicks;
    Key pressedKey = keyboard.pressedKey;

    if ((keyboard.mode != KEYBOARD_MODE_MEASUREMENT) && (ticks >= KEY_REPEAT_START))
    {
        if ((pressedKey == KEY_UP) || (pressedKey == KEY_DOWN))
        {
            if (ticks >= keyboard.nextRepeatTick)
            {
                keyboard.nextRepeatTick += KEY_REPEAT_PERIOD;
                return EVENT_KEY_START + pressedKey;
            }
        }
    }

    if (ticks == KEY_PRESSED_LONG)
    {
        if (keyDown[KEY_LEFT] && keyDown[KEY_OK])
            return EVENT_KEY_TOGGLELOCK;

        if (pressedKey == KEY_OK)
            return EVENT_KEY_POWER;

        if (keyboard.mode == KEYBOARD_MODE_MEASUREMENT)
        {
            if (pressedKey == KEY_LEFT)
                return EVENT_KEY_RESET;
            else if (pressedKey == KEY_RIGHT)
                return EVENT_KEY_TOGGLEPULSESOUND;
        }
    }

    return event;
}

#endif

void updateKeyboard(void)
{
    if (!keyboard.enabled)
        return;

    ViewEvent event = EVENT_NONE;

    bool keyDown[KEY_NUM];

    bool poweredOn = isPoweredOn();

    updateKeyboardState();

    for (int32_t i = 0; i < KEY_NUM; i++)
    {
        keyDown[i] = keyboardKeyDown[i] & KEY_PRESSED_MASK;

        bool keyPressed = !keyboard.previousKeyDown[i] && keyDown[i];
        bool keyReleased = keyboard.previousKeyDown[i] && !keyDown[i];
        keyboard.previousKeyDown[i] = keyDown[i];

        // Key pressed
        if (keyPressed)
        {
            bool wasSleeping = !isDisplayAwake();
#if defined(DISPLAY_MONOCHROME)
            wasSleeping &= (settings.displaySleep != DISPLAY_SLEEP_ALWAYS_OFF);
#endif
            keyboard.pressedTicks = 0;
            keyboard.pressedKey = i;
            keyboard.wasSleeping = wasSleeping;

            if (poweredOn)
            {
                if (wasSleeping)
                    event = EVENT_KEY_TOGGLEBACKLIGHT;

#if defined(KEYBOARD_4_KEYS) || defined(KEYBOARD_5_KEYS)
                if (keyboard.mode == KEYBOARD_MODE_MENU)
                    if ((i == KEY_UP) || (i == KEY_DOWN))
                        event = EVENT_KEY_START + i;

                keyboard.nextRepeatTick = KEY_REPEAT_START;
#endif
            }
        }

        // Key released
        if (keyReleased && (i == keyboard.pressedKey))
        {
            if (!poweredOn)
                event = EVENT_KEY_TOGGLEBACKLIGHT;
            else if (!keyboard.wasSleeping)
                event = getKeyReleaseEvent(i, event);

            keyboard.pressedKey = KEY_NONE;
        }
    }

    // Key timer
    if (keyboard.pressedKey != KEY_NONE)
    {
        keyboard.pressedTicks += 1;

        if (keyboard.pressedTicks == KEY_PRESSED_LONG)
        {
            if (keyboard.pressedKey == KEY_POWER)
            {
                keyboard.pressedKey = KEY_NONE;

                event = EVENT_KEY_POWER;
            }
        }

        if (poweredOn && !keyboard.wasSleeping)
            event = getKeyTimerEvent(keyDown, event);
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
    if (mode == KEYBOARD_MODE_MEASUREMENT)
        keyboard.pressedKey = KEY_NONE;

    keyboard.mode = mode;
}

ViewEvent getKeyboardEvent(void)
{
    if (keyboard.eventQueueHead != keyboard.eventQueueTail)
    {
        // Dequeue event
        ViewEvent event = keyboard.eventQueue[keyboard.eventQueueTail];
        keyboard.eventQueueTail = (keyboard.eventQueueTail + 1) & EVENT_QUEUE_MASK;

        return event;
    }

    return EVENT_NONE;
}
