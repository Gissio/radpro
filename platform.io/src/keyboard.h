/*
 * Rad Pro
 * Keyboard
 *
 * (C) 2022-2025 Gissio
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

#if defined(KEYBOARD_2KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#elif defined(KEYBOARD_3KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_OK,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#elif defined(KEYBOARD_4KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#elif defined(KEYBOARD_5KEYS)
typedef enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_OK,

    KEY_NUM,
    KEY_NONE = KEY_NUM,
} Key;
#endif

void initKeyboard(void);
void initKeyboardController(void);

void getKeyboardState(bool *);

void setKeyboardMode(KeyboardMode mode);

void onKeyboardTick(void);

Event getKeyboardEvent(void);
void clearKeyboardEvents();

#endif
