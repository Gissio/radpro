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
#include "power.h"
#include "settings.h"
#include "view.h"

static struct
{
    const View *currentView;

    bool drawUpdate;

    uint32_t displayTimer;
} view;

void dispatchViewEvents(void)
{
    syncTimerThread();

    // Key events
    while (true)
    {
        Event event = getKeyboardEvent();

        if (event == EVENT_NONE)
            break;
        else if (event == EVENT_KEY_POWER)
            requestPowerOff();
        else 
        {
            view.currentView->onEvent(view.currentView, event);

            triggerDisplay();
        }
    }

#if defined(DISPLAY_COLOR)
    // Pre-draw
    if (isDisplayBacklightOn())
    {
        if (!isDisplayOn())
            view.drawUpdate = true;
    }
    else
    {
        if (!settings.pulseFlashes ||
            isPulseThresholding())
        {
            view.drawUpdate = false;

            if (isDisplayOn())
                setDisplayOn(false);
        }
    }
#endif

    // Draw
    if (view.drawUpdate)
    {
        view.drawUpdate = false;
        view.currentView->onEvent(view.currentView, EVENT_DRAW);

        refreshDisplay();
    }

#if defined(DISPLAY_COLOR)
    // Post-draw
    if (isDisplayBacklightOn())
    {
        if (!isDisplayOn())
            setDisplayOn(true);
    }
#endif
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
