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
#if defined(DISPLAY_MONOCHROME)

        if (((settings.displayTimer == DISPLAY_TIMER_ALWAYS_OFF) ||
             (settings.displayTimer == DISPLAY_TIMER_PULSE_FLASHES)) ||
            isBacklightTimerActive())

#elif defined(DISPLAY_COLOR)

        if (isBacklightTimerActive())

#endif
        {
            if (event == EVENT_KEY_POWER)
                powerOff();
            else
                view.currentView->onEvent(view.currentView, event);
        }

        triggerBacklight();
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
