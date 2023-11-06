/*
 * Rad Pro
 * User interface view
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "power.h"
#include "settings.h"
#include "view.h"

static struct
{
    const struct View *currentView;
    bool refresh;
} view;

void updateView(void)
{
    // Key events

    enum Event event = getKeyboardEvent();
    if (event != EVENT_NONE)
    {
#if defined(DISPLAY_COLOR)

        if ((settings.displaySleep != DISPLAY_SLEEP_ALWAYS_ON) &&
            !isDisplayTimerActive())
            event = EVENT_BACKLIGHT;

#endif

        triggerDisplay();

        if (event == EVENT_POWER_OFF)
            powerOff();
        else
            view.currentView->onEvent(view.currentView, event);
    }

    // Draw events

    if (view.refresh)
    {
        view.refresh = false;

        clearDisplayBuffer();
        drawStatusBar();

        view.currentView->onEvent(view.currentView, EVENT_DRAW);

        sendDisplayBuffer();
    }
}

void setView(const struct View *newView)
{
    view.currentView = newView;

    refreshView();
}

const struct View *getView(void)
{
    return view.currentView;
}

void refreshView(void)
{
    view.refresh = true;
}
