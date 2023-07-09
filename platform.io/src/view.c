/*
 * Rad Pro
 * User interface view manager
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "power.h"
#include "view.h"

static struct
{
    const struct View *currentView;
    bool refresh;
} view;

void updateView(void)
{
    sleep(0);
    updateEvents();

    // Key events
    KeyEvent keyEvent = getKeyEvent();

    if (keyEvent >= 0)
    {
        triggerBacklight();

        if (keyEvent == KEY_EVENT_POWER_OFF)
            powerOff();
        else
            view.currentView->onKey(view.currentView, keyEvent);
    }

    // Draw events
    if (view.refresh)
    {
        view.refresh = false;

        clearDisplayBuffer();
        drawStatusBar();

        view.currentView->onDraw(view.currentView);

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
