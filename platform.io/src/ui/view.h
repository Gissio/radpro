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
    EVENT_NONE,

    EVENT_KEY_START,
    EVENT_KEY_BACK = EVENT_KEY_START,
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
    EVENT_POSTDRAW,

    EVENT_HEARTBEAT,
} ViewEvent;

typedef void OnViewEvent(ViewEvent event);

typedef void ShowView(void);

void initView(void);

void updateView(void);

void requestViewUpdate(void);
void showView(OnViewEvent *onViewEvent);
void updateViewHeartbeat(void);

void setLockModeEnabled(bool value);
bool isLockModeEnabled(void);

#endif
