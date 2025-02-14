/*
 * Rad Pro
 * User interface view
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "events.h"
#include "init.h"
#include "keyboard.h"
#include "power.h"
#include "settings.h"
#include "system.h"
#include "view.h"

static struct
{
    const View *currentView;

    bool periodUpdate;
    bool drawUpdate;
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
    while (true)
    {
        Event event = getKeyboardEvent();

        if (event == EVENT_NONE)
            break;

        if (event == EVENT_KEY_UNLOCK)
            setLockMode(false);

        if (isPowerOffViewActive())
        {
            if (event == EVENT_KEY_POWER)
                setPowerOnView();
        }
        else
        {
            if (event == EVENT_KEY_POWER)
                setPowerOffView();
            else
            {
                if (event == EVENT_KEY_TOGGLEPULSECLICKS)
                {
                    togglePulseClicks();

                    triggerVibration();
                }
                else
                    view.currentView->onEvent(view.currentView, event);

                requestDisplayBacklightTrigger();
            }
        }
    }

    // Pre-draw
    if (isDisplayBacklightTriggerRequested())
        view.drawUpdate = true;
    else
    {
#if defined(DISPLAY_MONOCHROME)
        bool isDisplayActive = !isPowerOffViewActive();
#elif defined(DISPLAY_COLOR)
        bool isPulseFlashesActive = settings.pulseDisplayFlash &&
                                    !isPulseThresholdEnabled();
        bool isDisplayActive = !isPowerOffViewActive() &&
                               (isDisplayBacklightActive() ||
                                isPulseFlashesActive);
#endif

        if (!isDisplayActive)
        {
            view.drawUpdate = false;

            if (isDisplayEnabled())
                enableDisplay(false);
        }
    }

    // Draw
    if (view.drawUpdate)
    {
        view.drawUpdate = false;

        view.currentView->onEvent(view.currentView, EVENT_DRAW);

        refreshDisplay();

        if (!isDisplayEnabled())
            enableDisplay(true);

        if (isDisplayBacklightTriggerRequested())
            triggerDisplayBacklight();

        view.currentView->onEvent(view.currentView, EVENT_POST_DRAW);
    }
}

void setView(const View *newView)
{
    view.currentView = newView;

    updateView();
}

const View *getView(void)
{
    return view.currentView;
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
