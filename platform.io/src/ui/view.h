/*
 * Rad Pro
 * UI view
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(UI_VIEW_H)
#define UI_VIEW_H

#include <stdbool.h>

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

    EVENT_HEARTBEAT,

    EVENT_NONE,
} Event;

typedef void OnEvent(Event event);

struct View_
{
    OnEvent *onEvent;
    const void *userdata;
};

typedef const struct View_ View;
typedef View *const ViewPointer;

void initView(void);

void updateView(void);

void setView(View *view);
View *getView(void);

void requestViewUpdate(void);
void updateViewHeartbeat(void);

void setLockMode(bool value);
bool isInLockMode(void);

#endif
