/*
 * FS2011 Pro
 * Keyboard handling
 * 
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

enum Keys
{
    KEY_POWER,
    KEY_UP,
    KEY_DOWN,
    KEY_SELECT,
    KEY_BACK,

    KEY_NUM,

    KEY_RESET = KEY_NUM,
    KEY_POWER_OFF,
    KEY_BACK_UP,
};

void initKeyboard();
int getKeyboardKey();

#endif
