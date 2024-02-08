/*
 * Rad Pro
 * User interface view
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(VIEW_H)
#define VIEW_H

enum Event
{
    EVENT_KEY_UP,
    EVENT_KEY_DOWN,
    EVENT_KEY_BACK,
    EVENT_KEY_ENTER,
    EVENT_KEY_BACKLIGHT,
    EVENT_KEY_RESET,
    EVENT_KEY_POWER_OFF,

    EVENT_DRAW,

    EVENT_NONE,
};

typedef struct View_ View;

typedef void OnEvent(const View *view, enum Event event);

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

void dispatchViewEvents(void);

void setView(const View *view);
const View *getView(void);

void updateView(void);

#endif
