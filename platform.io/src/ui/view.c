/*
 * Rad Pro
 * UI view
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/measurements.h"
#include "../peripherals/display.h"
#include "../peripherals/keyboard.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/view.h"

static struct
{
    View *currentView;

    bool drawUpdate;
} view;

void initView(void)
{
    view.currentView = &powerOffView;

#if defined(DISPLAY_MONOCHROME)
    refreshDisplay();
    setDisplayEnabled(true);
#endif
}

void updateView(void)
{
    // Key events
    while (true)
    {
        Event event = getKeyboardEvent();

        if (event == EVENT_NONE)
            break;

        if (!isPoweredOn())
        {
            if (event == EVENT_KEY_POWER)
                powerOn(false);
            else
                view.currentView->onEvent(EVENT_KEY_TOGGLEBACKLIGHT);
        }
        else
        {
            if (event == EVENT_KEY_POWER)
            {
                if (!isInLockMode())
                    powerOff(false);
            }
            else
            {
                if (event == EVENT_KEY_TOGGLELOCK)
                {
                    if (!isInLockMode())
                    {
                        setMeasurementViewCurrent();
                        setLockMode(true);

                        event = EVENT_KEY_TOGGLEBACKLIGHT;
                    }
                    else
                        setLockMode(false);
                }
                else
                    view.currentView->onEvent(event);

                if ((event == EVENT_KEY_TOGGLEBACKLIGHT) && isDisplayAwake())
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
        bool isDisplayActive = isBacklightActive() || settings.pulseDisplayFlash;
        if (!isDisplayActive)
        {
            view.drawUpdate = false;

            if (isDisplayEnabled())
                setDisplayEnabled(false);
        }
#endif
    }

    // Draw
    if (view.drawUpdate)
    {
        view.drawUpdate = false;

        view.currentView->onEvent(EVENT_DRAW);

#if defined(DISPLAY_MONOCHROME)
        refreshDisplay();
#elif defined(DISPLAY_COLOR)
        if (!isDisplayEnabled())
            setDisplayEnabled(true);
#endif

        if (isBacklightTriggerRequested())
            triggerBacklight();

        // Post-draw
        view.currentView->onEvent(EVENT_POST_DRAW);
    }
}

void setView(View *newView)
{
    view.currentView = newView;

    requestViewUpdate();
}

View *getView(void)
{
    return view.currentView;
}

void requestViewUpdate(void)
{
    view.drawUpdate = true;
}

void updateViewHeartbeat(void)
{
    view.currentView->onEvent(EVENT_HEARTBEAT);

    view.drawUpdate = true;
}

// Lock mode

static bool viewLockMode;

void setLockMode(bool value)
{
    viewLockMode = value;

    requestViewUpdate();
    triggerVibration();
}

bool isInLockMode(void)
{
    return viewLockMode;
}
