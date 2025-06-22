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
#include "keyboard.h"
#include "measurements.h"
#include "power.h"
#include "settings.h"
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

        if (isPoweredOff())
        {
            if (event == EVENT_KEY_POWER)
                powerOn();
        }
        else
        {
            if (event == EVENT_KEY_POWER)
            {
                if (!isLockMode())
                    powerOff();
            }
            else
            {
                if (event == EVENT_KEY_TOGGLELOCK)
                {
                    if (!isLockMode())
                    {
                        setMeasurementView(-1);
                        setLockMode(true);
                    }
                    else
                        setLockMode(false);
                }
                else if (event == EVENT_KEY_TOGGLEPULSECLICKS)
                {
                    togglePulseClicks();

                    triggerVibration();
                }
                else
                    view.currentView->onEvent(view.currentView, event);

                if ((event == EVENT_KEY_TOGGLEBACKLIGHT) &&
                    isBacklightActive())
                    cancelBacklight();
                else
                    requestBacklightTrigger();
            }
        }
    }

    // Pre-draw
    if (isBacklightTriggerRequested())
        view.drawUpdate = true;
    else
    {
#if defined(DISPLAY_MONOCHROME)
        bool isDisplayActive = !isPoweredOff();
#elif defined(DISPLAY_COLOR)
        bool isPulseFlashesActive = settings.pulseDisplayFlash &&
                                    !isPulseThresholdEnabled();
        bool isDisplayActive = !isPoweredOff() &&
                               (isBacklightActive() ||
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

        if (isBacklightTriggerRequested())
            triggerBacklight();

        view.currentView->onEvent(view.currentView, EVENT_POST_DRAW);
    }
}

void setView(const View *newView)
{
    view.currentView = newView;

    requestViewUpdate();
}

const View *getView(void)
{
    return view.currentView;
}

void requestViewUpdate(void)
{
    view.drawUpdate = true;
}

void updateView(void)
{
    view.periodUpdate = true;
    view.drawUpdate = true;
}

// Lock view

bool systemLockMode;

void setLockMode(bool value)
{
    systemLockMode = value;

    triggerVibration();
}

bool isLockMode(void)
{
    return systemLockMode;
}
