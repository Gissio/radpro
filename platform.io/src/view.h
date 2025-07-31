/*
 * Rad Pro
 * User interface view
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(VIEW_H)
#define VIEW_H

#include "cstring.h"

typedef enum
{
    EVENT_KEY_BACK,
    EVENT_KEY_SELECT,
    EVENT_KEY_UP,
    EVENT_KEY_DOWN,
    EVENT_KEY_RESET,
    EVENT_KEY_POWER,
    EVENT_KEY_TOGGLELOCK,
    EVENT_KEY_TOGGLEPULSESOUND,
    EVENT_KEY_TOGGLEBACKLIGHT,
    EVENT_KEY_PLAY,

    EVENT_DRAW,
    EVENT_POST_DRAW,

    EVENT_PERIOD,

    EVENT_NONE,
} Event;

typedef const struct View_ View;
typedef View *const ViewPointer;

typedef void OnEvent(Event event);

struct View_
{
    OnEvent *onEvent;
    const void *userdata;
};

void initView(void);

void dispatchViewEvents(void);

void setView(View *view);
View *getView(void);

void requestViewUpdate(void);
void updateView(void);

void setLockMode(bool value);
bool isLockMode(void);

void setPulseSoundIconEnabled(bool value);
bool isPulseSoundIconEnabled(void);

#endif
