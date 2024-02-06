/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(KEYBOARD_H)

#define KEYBOARD_H

#include <stdbool.h>

#include "view.h"

enum Key
{
    KEY_UP,
    KEY_DOWN,

#if defined(KEYBOARD_2KEYS)

    KEY_BACK = KEY_UP,
    KEY_ENTER = KEY_DOWN,
    KEY_POWER = KEY_DOWN,

#elif defined(KEYBOARD_5KEYS)

    KEY_BACK,
    KEY_ENTER,
    KEY_POWER,

#endif

    KEY_NUM,
    KEY_NONE = KEY_NUM,
};

void initKeyboard(void);
void initKeyboardHardware(void);

void getKeyboardKeys(bool *);

void setKeyboardDelayedBack(bool value);

void onKeyboardTick(void);

enum Event getKeyboardEvent(void);

#endif
