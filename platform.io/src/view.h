/*
 * Rad Pro
 * User interface view
 *
 * (C) 2022-2023 Gissio
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

struct View;

typedef void OnEvent(const struct View *view, enum Event event);

struct View
{
    OnEvent *onEvent;
    const void *userdata;
};

void updateView(void);

void setView(const struct View *view);
const struct View *getView(void);

void refreshView(void);

#endif
