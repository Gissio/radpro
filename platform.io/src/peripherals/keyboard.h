/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(KEYBOARD_H)
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "../ui/view.h"

typedef enum
{
    KEYBOARD_MODE_MEASUREMENT,
    KEYBOARD_MODE_MENU,
} KeyboardMode;

#if defined(KEYBOARD_2_KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_POWER = KEY_RIGHT,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#elif defined(KEYBOARD_3_KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_OK,
    KEY_POWER = KEY_OK,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#elif defined(KEYBOARD_4_KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_POWER = KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#elif defined(KEYBOARD_5_KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_OK,
    KEY_POWER = KEY_OK,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#endif

void initKeyboard(void);

void onKeyboardTick(void);
void onKeyboardUpdate(void);

bool isPowerKeyDown(void);
bool waitLongKeyPress(void);

void setKeyboardMode(KeyboardMode mode);

ViewEvent getKeyboardEvent(void);

// Platform interface
void initKeyboardHardware(void);
// Returns the raw key state as a bitmask of (1 << Key); called every tick
uint32_t readKeyboardKeys(void);
#if defined(KEYBOARD_KNOB)
// Returns a knob step as a (1 << KEY_UP) or (1 << KEY_DOWN) key press;
// called every keyboard update
uint32_t readKeyboardKnob(void);
#endif

#endif
