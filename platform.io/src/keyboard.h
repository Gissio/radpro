/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(KEYBOARD_H)
#define KEYBOARD_H

#include <stdbool.h>

#include "view.h"

typedef enum
{
    KEYBOARD_MODE_MEASUREMENT,
    KEYBOARD_MODE_MENU,
} KeyboardMode;

#if defined(KEYBOARD_5KEYS)

enum Key
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_SELECT,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
};

#elif defined(KEYBOARD_2KEYS)

enum Key
{
    KEY_LEFT,
    KEY_SELECT,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
};

#endif

void initKeyboard(void);
void initKeyboardHardware(void);

void getKeyboardState(bool *);

void setKeyboardMode(KeyboardMode mode);

void onKeyboardTick(void);

enum Event getKeyboardEvent(void);

#endif
