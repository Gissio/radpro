/*
 * Rad Pro
 * Keyboard management
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#ifdef KEYBOARD_5KEYS

enum KeyEnum
{
    KEY_POWER,
    KEY_UP,
    KEY_DOWN,
    KEY_ENTER,
    KEY_BACK,

    KEY_NUM,
};

#elif KEYBOARD_2KEYS

enum KeyEnum
{
    KEY_DOWN,
    KEY_POWER,

    KEY_NUM,
};

#endif

enum KeyEventEnum
{
    KEY_EVENT_BACKLIGHT,
    KEY_EVENT_UP,
    KEY_EVENT_DOWN,
    KEY_EVENT_ENTER,
    KEY_EVENT_BACK,

    KEY_EVENT_RESET,
    KEY_EVENT_POWER_OFF,
};

typedef int8_t KeyEvent;

void initKeyboard(void);
void initKeyboardKeyDown(void);
void getKeyboardKeyDown(bool *);

void onKeyboardTick(void);

KeyEvent getKeyEvent(void);

#endif
