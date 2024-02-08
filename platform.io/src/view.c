/*
 * Rad Pro
 * User interface view
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "game.h"
#include "power.h"
#include "settings.h"
#include "view.h"

static struct
{
    const View *currentView;

    bool drawUpdate;
} view;

void dispatchViewEvents(void)
{
    // Key events

    enum Event event = getKeyboardEvent();
    if (event != EVENT_NONE)
    {
#if defined(DISPLAY_COLOR)

        if ((settings.displayTimer != DISPLAY_TIMER_ALWAYS_ON) &&
            !isBacklightTimerActive())
            event = EVENT_KEY_BACKLIGHT;

#endif

        triggerBacklight();

        if (event == EVENT_KEY_POWER_OFF)
            powerOff();
        else
            view.currentView->onEvent(view.currentView, event);
    }

    if (view.drawUpdate)
    {
        view.drawUpdate = false;

        drawStatusBar();

        view.currentView->onEvent(view.currentView, EVENT_DRAW);

        refreshDisplay();
    }
}

void setView(const View *newView)
{
    view.currentView = newView;

    view.drawUpdate = true;
}

void updateView(void)
{
    view.drawUpdate = true;
}

const View *getView(void)
{
    return view.currentView;
}
