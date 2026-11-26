/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <assert.h>
#include <stdint.h>

#include "../peripherals/keyboard.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"

// Timing, in keyboard updates (KEY_TICKS)
#define KEY_PRESSED_SHORT ((uint32_t)(0.25 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_PRESSED_LONG ((uint32_t)(1.0 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_REPEAT_START ((uint32_t)(0.5 * SYSTICK_FREQUENCY / KEY_TICKS))
#define KEY_REPEAT_PERIOD ((uint32_t)(0.05 * SYSTICK_FREQUENCY / KEY_TICKS))

// A key counts as down only if it was sampled down on KEY_DEBOUNCE_SAMPLES
// consecutive ticks within the last KEY_TICKS ticks. This rejects isolated
// glitches (e.g. HV converter EMI coupling into the key lines, issue #54),
// while key release still needs KEY_TICKS clean samples to absorb bounce.
#define KEY_DEBOUNCE_SAMPLES 4
#define KEY_PRESSED_MASK ((1U << KEY_TICKS) - 1)

static_assert((KEY_TICKS + KEY_DEBOUNCE_SAMPLES - 1) <= 32,
              "Key sample window exceeds the 32-bit key history");
static_assert(KEY_DEBOUNCE_SAMPLES == 4,
              "getKeysDown() run detection assumes 4 samples");

#if defined(KEYBOARD_4_KEYS) || defined(KEYBOARD_5_KEYS)
#define KEYBOARD_REPEAT
#endif

#define EVENT_QUEUE_SIZE 8
#define EVENT_QUEUE_MASK (EVENT_QUEUE_SIZE - 1)

#define K(key) (1U << (key))

static struct
{
    // Per-key sample history, newest sample in bit 0
    volatile uint32_t keyHistory[KEY_NUM];

    uint32_t previousKeysDown;

    KeyboardMode mode;

    Key pressedKey;
    uint32_t pressedTicks;
#if defined(KEYBOARD_REPEAT)
    uint32_t nextRepeatTick;
#endif
    bool wasSleeping;

    volatile uint32_t eventQueueHead;
    volatile uint32_t eventQueueTail;
    volatile ViewEvent eventQueue[EVENT_QUEUE_SIZE];
} keyboard;

// Key map
//
// getKeyReleaseEvent(): event for the pressed key released after "ticks"
// keyboard updates.
// getKeyHoldEvent(): event for the pressed key held for "ticks" keyboard
// updates, with "keysDown" the keys currently down. A long press of the
// power key as the pressed key (EVENT_KEY_POWER) is handled by the caller.

#if defined(KEYBOARD_2_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, bool measurement, uint32_t ticks)
{
    if (ticks < KEY_PRESSED_SHORT)
    {
        if (key == KEY_RIGHT)
            return EVENT_KEY_DOWN;
        if (!measurement && (key == KEY_LEFT))
            return EVENT_KEY_UP;
    }

    if (measurement && (ticks < KEY_PRESSED_LONG) && (key == KEY_LEFT))
        return EVENT_KEY_BACK;

    return EVENT_NONE;
}

static ViewEvent getKeyHoldEvent(Key key, uint32_t keysDown, bool measurement, uint32_t ticks)
{
    if (ticks == KEY_PRESSED_SHORT)
    {
        if (keysDown == K(KEY_RIGHT))
            return EVENT_KEY_SELECT;
        if (!measurement && (keysDown == K(KEY_LEFT)))
            return EVENT_KEY_BACK;
    }
    else if (ticks == KEY_PRESSED_LONG)
    {
        if (keysDown == (K(KEY_LEFT) | K(KEY_RIGHT)))
            return EVENT_KEY_TOGGLELOCK;
        if (measurement && (key == KEY_LEFT))
            return EVENT_KEY_RESET;
    }

    return EVENT_NONE;
}

#elif defined(KEYBOARD_3_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, bool measurement, uint32_t ticks)
{
    if (ticks < KEY_PRESSED_SHORT)
    {
        if (key == KEY_RIGHT)
            return EVENT_KEY_DOWN;
        if (!measurement && (key == KEY_LEFT))
            return EVENT_KEY_UP;
    }

    if (ticks < KEY_PRESSED_LONG)
    {
        if (measurement && (key == KEY_LEFT))
            return EVENT_KEY_UP;
        if (key == KEY_OK)
            return measurement ? EVENT_KEY_BACK : EVENT_KEY_SELECT;
    }

    return EVENT_NONE;
}

static ViewEvent getKeyHoldEvent(Key key, uint32_t keysDown, bool measurement, uint32_t ticks)
{
    if (ticks == KEY_PRESSED_SHORT)
    {
        if (measurement)
        {
            if ((keysDown & (K(KEY_LEFT) | K(KEY_RIGHT))) == K(KEY_RIGHT))
                return EVENT_KEY_SELECT;
        }
        else
        {
            if ((keysDown & (K(KEY_LEFT) | K(KEY_OK))) == K(KEY_LEFT))
                return EVENT_KEY_BACK;
            if (keysDown & K(KEY_RIGHT))
                return EVENT_KEY_SELECT;
        }
    }
    else if (ticks == KEY_PRESSED_LONG)
    {
        if ((keysDown & (K(KEY_LEFT) | K(KEY_OK))) == (K(KEY_LEFT) | K(KEY_OK)))
            return EVENT_KEY_TOGGLELOCK;
        if (keysDown & K(KEY_OK))
            return EVENT_KEY_POWER;
        if (measurement)
        {
            if ((keysDown & (K(KEY_LEFT) | K(KEY_RIGHT))) == (K(KEY_LEFT) | K(KEY_RIGHT)))
                return EVENT_KEY_TOGGLEPULSESOUND;
            if (keysDown & K(KEY_LEFT))
                return EVENT_KEY_RESET;
        }
    }

    return EVENT_NONE;
}

#elif defined(KEYBOARD_4_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, bool measurement, uint32_t ticks)
{
    if (ticks < KEY_PRESSED_LONG)
    {
        if (key == KEY_LEFT)
            return EVENT_KEY_BACK;
        if (key == KEY_RIGHT)
            return EVENT_KEY_SELECT;
        if (measurement)
            return EVENT_KEY_START + key; // KEY_UP, KEY_DOWN
    }

    return EVENT_NONE;
}

static ViewEvent getKeyHoldEvent(Key key, uint32_t keysDown, bool measurement, uint32_t ticks)
{
    if (ticks == KEY_PRESSED_LONG)
    {
        if ((keysDown & (K(KEY_LEFT) | K(KEY_RIGHT))) == (K(KEY_LEFT) | K(KEY_RIGHT)))
            return EVENT_KEY_TOGGLELOCK;
        if (measurement)
        {
            if (key == KEY_UP)
                return EVENT_KEY_PLAY;
            if (key == KEY_DOWN)
                return EVENT_KEY_TOGGLEPULSESOUND;
            if (key == KEY_LEFT)
                return EVENT_KEY_RESET;
        }
    }

    return EVENT_NONE;
}

#elif defined(KEYBOARD_5_KEYS)

static ViewEvent getKeyReleaseEvent(Key key, bool measurement, uint32_t ticks)
{
    if (ticks < KEY_PRESSED_LONG)
    {
        if (key == KEY_LEFT)
            return EVENT_KEY_BACK;
        if (key == KEY_RIGHT)
            return EVENT_KEY_SELECT;
        if (key == KEY_OK)
#if defined(KEYBOARD_KEY_POWER_OK)
            return measurement ? EVENT_KEY_TOGGLEBACKLIGHT : EVENT_KEY_SELECT;
#else
            return EVENT_KEY_TOGGLEBACKLIGHT;
#endif
        if (measurement)
            return EVENT_KEY_START + key; // KEY_UP, KEY_DOWN
    }

    return EVENT_NONE;
}

static ViewEvent getKeyHoldEvent(Key key, uint32_t keysDown, bool measurement, uint32_t ticks)
{
    if (ticks == KEY_PRESSED_LONG)
    {
        if ((keysDown & (K(KEY_LEFT) | K(KEY_OK))) == (K(KEY_LEFT) | K(KEY_OK)))
            return EVENT_KEY_TOGGLELOCK;
        if (measurement)
        {
            if (key == KEY_LEFT)
                return EVENT_KEY_RESET;
            if (key == KEY_RIGHT)
                return EVENT_KEY_TOGGLEPULSESOUND;
        }
    }

    return EVENT_NONE;
}

#endif

// Returns, as a bitmask, the keys sampled down within the last KEY_TICKS
// ticks: on any tick, or if debounced, on KEY_DEBOUNCE_SAMPLES consecutive ticks
static NOINLINE uint32_t getKeysDown(bool debounced)
{
    uint32_t keysDown = 0;

    for (int32_t i = KEY_NUM - 1; i >= 0; i--)
    {
        uint32_t run = keyboard.keyHistory[i];

        if (debounced)
        {
            // Keep the bits that start a run of 4 down samples
            run &= run >> 1;
            run &= run >> 2;
        }

        keysDown = (keysDown << 1) | ((run & KEY_PRESSED_MASK) != 0);
    }

    return keysDown;
}

void initKeyboard(void)
{
    initKeyboardHardware();

    onKeyboardTick();

    // Keys held at startup must not trigger events
    keyboard.previousKeysDown = getKeysDown(false);

    keyboard.pressedTicks = ((uint32_t)(10.0 * SYSTICK_FREQUENCY / KEY_TICKS));
    keyboard.pressedKey = KEY_NONE;
}

void onKeyboardTick(void)
{
    uint32_t keys = readKeyboardKeys();

    for (uint32_t i = 0; i < KEY_NUM; i++, keys >>= 1)
        keyboard.keyHistory[i] = (keyboard.keyHistory[i] << 1) | (keys & 1);
}

bool isPowerKeyDown(void)
{
    return getKeysDown(true) & K(KEY_POWER);
}

bool waitLongKeyPress(void)
{
    for (uint32_t i = 0; i < KEY_PRESSED_LONG; i++)
    {
        sleep(KEY_TICKS);

        if (!isPowerKeyDown())
            return false;
    }

    return true;
}

static void enqueueEvent(ViewEvent event)
{
    if (event == EVENT_NONE)
        return;

    uint32_t head = keyboard.eventQueueHead;
    uint32_t nextHead = (head + 1) & EVENT_QUEUE_MASK;

    // Drop the event if the queue is full
    if (nextHead == keyboard.eventQueueTail)
        return;

    keyboard.eventQueue[head] = event;
    keyboard.eventQueueHead = nextHead;
}

static void onKeyPressed(Key key, bool poweredOn)
{
    bool wasSleeping = !isDisplayAwake();
#if defined(DISPLAY_MONOCHROME)
    wasSleeping &= (settings.displaySleep != DISPLAY_SLEEP_ALWAYS_OFF);
#endif

    keyboard.pressedKey = key;
    keyboard.pressedTicks = 0;
    keyboard.wasSleeping = wasSleeping;

    if (!poweredOn)
        return;

#if defined(KEYBOARD_REPEAT)
    keyboard.nextRepeatTick = KEY_REPEAT_START;

    // Menu navigation reacts on press (and repeats while held)
    if ((keyboard.mode == KEYBOARD_MODE_MENU) &&
        ((key == KEY_UP) || (key == KEY_DOWN)))
    {
        enqueueEvent(EVENT_KEY_START + key);

        return;
    }
#endif

    if (wasSleeping)
        enqueueEvent(EVENT_KEY_TOGGLEBACKLIGHT);
}

static void onKeyReleased(bool poweredOn)
{
    if (!poweredOn)
        enqueueEvent(EVENT_KEY_TOGGLEBACKLIGHT);
    else if (!keyboard.wasSleeping)
        enqueueEvent(getKeyReleaseEvent(keyboard.pressedKey,
                                        keyboard.mode == KEYBOARD_MODE_MEASUREMENT,
                                        keyboard.pressedTicks));

    keyboard.pressedKey = KEY_NONE;
}

static void onKeyHeld(uint32_t keysDown, bool poweredOn)
{
    Key key = keyboard.pressedKey;
    uint32_t ticks = ++keyboard.pressedTicks;
    ViewEvent event = EVENT_NONE;

    // Power key works even when powered off or with the display asleep
    bool powerKeyLong = (key == KEY_POWER) && (ticks == KEY_PRESSED_LONG);
    if (powerKeyLong)
        event = EVENT_KEY_POWER;

    if (poweredOn && !keyboard.wasSleeping)
    {
        ViewEvent keyEvent;

#if defined(KEYBOARD_REPEAT)
        if ((keyboard.mode != KEYBOARD_MODE_MEASUREMENT) &&
            ((key == KEY_UP) || (key == KEY_DOWN)) &&
            (ticks >= KEY_REPEAT_START) &&
            (ticks >= keyboard.nextRepeatTick))
        {
            keyboard.nextRepeatTick += KEY_REPEAT_PERIOD;

            keyEvent = EVENT_KEY_START + key;
        }
        else
#endif
            keyEvent = getKeyHoldEvent(key, keysDown,
                                       keyboard.mode == KEYBOARD_MODE_MEASUREMENT,
                                       ticks);

        if (keyEvent != EVENT_NONE)
            event = keyEvent;
    }

    if (powerKeyLong)
        keyboard.pressedKey = KEY_NONE;

    enqueueEvent(event);
}

void onKeyboardUpdate(void)
{
    bool poweredOn = isPoweredOn();

#if defined(KEYBOARD_KNOB)
    uint32_t keysDown = readKeyboardKnob() | getKeysDown(true);
#else
    uint32_t keysDown = getKeysDown(true);
#endif

    uint32_t keysChanged = keysDown ^ keyboard.previousKeysDown;
    keyboard.previousKeysDown = keysDown;

    // Release first, so rolling over to another key keeps the release event
    // (K(KEY_NONE) is never set in keysChanged)
    if (keysChanged & ~keysDown & K(keyboard.pressedKey))
        onKeyReleased(poweredOn);

    uint32_t keysPressed = keysChanged & keysDown;
    for (uint32_t i = 0; keysPressed; i++, keysPressed >>= 1)
    {
        if (keysPressed & 1)
            onKeyPressed(i, poweredOn);
    }

    if (keyboard.pressedKey != KEY_NONE)
        onKeyHeld(keysDown, poweredOn);
}

NOINLINE void setKeyboardMode(KeyboardMode mode)
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
