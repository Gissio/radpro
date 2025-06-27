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

typedef enum
{
    EVENT_KEY_BACK,
    EVENT_KEY_SELECT,
    EVENT_KEY_UP,
    EVENT_KEY_DOWN,
    EVENT_KEY_RESET,
    EVENT_KEY_POWER,
    EVENT_KEY_TOGGLELOCK,
    EVENT_KEY_TOGGLEPULSECLICKS,
    EVENT_KEY_TOGGLEBACKLIGHT,
    EVENT_KEY_PLAY,

    EVENT_DRAW,
    EVENT_POST_DRAW,

    EVENT_PERIOD,

    EVENT_NONE,
} Event;

typedef struct View_ View;

typedef void OnEvent(const View *view, Event event);

struct View_
{
    OnEvent *onEvent;
    const void *userdata;
};

typedef struct
{
    const char *option;
    const View *view;
} OptionView;

void initView(void);

void dispatchViewEvents(void);

void setView(const View *view);

void requestViewUpdate(void);
void updateView(void);

void setLockMode(bool value);
bool isLockMode(void);

#endif
