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
#include "init.h"
#include "keyboard.h"
#include "power.h"
#include "settings.h"
#include "view.h"

static struct
{
    const View *currentView;

    bool periodUpdate;
    bool drawUpdate;

    uint32_t displayTimer;
} view;

void dispatchViewEvents(void)
{
    // Period events
    if (view.periodUpdate)
    {
        view.periodUpdate = false;
        view.currentView->onEvent(view.currentView, EVENT_PERIOD);
    }

    // Key events
    syncTimerThread();

    while (true)
    {
        Event event = getKeyboardEvent();

        if (event == EVENT_NONE)
            break;

        if (isPowerOffViewEnabled())
        {
            if (event == EVENT_KEY_POWER)
                setSplashView();
        }
        else
        {
            if (event == EVENT_KEY_POWER)
                setPowerOffView();
            else
            {
                view.currentView->onEvent(view.currentView, event);

                triggerDisplay();
            }
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

    updateView();
}

void updateView(void)
{
    view.drawUpdate = true;
}

void updateViewPeriod(void)
{
    view.periodUpdate = true;
    view.drawUpdate = true;
}

const View *getView(void)
{
    return view.currentView;
}
