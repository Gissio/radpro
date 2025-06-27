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

    bool drawUpdate;
} view;

void initView(void)
{
    view.currentView = &powerOffView;

#if defined(DISPLAY_MONOCHROME)
    refreshDisplay();
    enableDisplay(true);
#endif
}

void dispatchViewEvents(void)
{
    // Key events
    while (true)
    {
        Event event = getKeyboardEvent();

        if (event == EVENT_NONE)
            break;

        if (!isPowered())
        {
            if (event == EVENT_KEY_POWER)
                powerOn();
            else
                view.currentView->onEvent(view.currentView, EVENT_KEY_TOGGLEBACKLIGHT);
        }
        else
        {
            if (event == EVENT_KEY_POWER)
            {
                if (!isLockMode())
                    powerOff(false);
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
#if defined(DISPLAY_COLOR)
        bool isPulseFlashesActive = settings.pulseDisplayFlash &&
                                    !isPulseThresholdEnabled();
        bool isDisplayActive = (isBacklightActive() ||
                                isPulseFlashesActive);

        if (!isDisplayActive)
        {
            view.drawUpdate = false;

            if (isDisplayEnabled())
                enableDisplay(false);
        }
#endif
    }

    // Draw
    if (view.drawUpdate)
    {
        view.drawUpdate = false;

        view.currentView->onEvent(view.currentView, EVENT_DRAW);

#if defined(DISPLAY_MONOCHROME)
        refreshDisplay();
#elif defined(DISPLAY_COLOR)
        if (!isDisplayEnabled())
            enableDisplay(true);
#endif

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

void requestViewUpdate(void)
{
    view.drawUpdate = true;
}

void updateView(void)
{
    view.currentView->onEvent(view.currentView, EVENT_PERIOD);

    view.drawUpdate = true;
}

// Lock mode

bool viewLockMode;

void setLockMode(bool value)
{
    viewLockMode = value;

    triggerVibration();
}

bool isLockMode(void)
{
    return viewLockMode;
}
