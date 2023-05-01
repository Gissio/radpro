/*
 * FS2011 Pro
 * Keyboard handling
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

enum KeyEventEnum
{
    KEY_POWER,
    KEY_UP,
    KEY_DOWN,
    KEY_SELECT,
    KEY_BACK,

    KEY_EVENT_NUM,

    KEY_RESET = KEY_EVENT_NUM,
    KEY_POWER_OFF,
    KEY_BACK_DELAYED,
};

typedef int8_t KeyEvent;

void initKeyboard(void);
void onKeyboardTick(void);
KeyEvent getKeyEvent(void);

#endif
