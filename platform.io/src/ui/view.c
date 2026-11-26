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
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/view.h"

static struct
{
    OnViewEvent *onViewEvent;

    bool drawUpdate;
    bool lockModeEnabled;
} view;

void initView(void)
{
#if defined(DISPLAY_MONOCHROME)
    refreshDisplay();
    setDisplayEnabled(true);
#endif
}

void updateView(void)
{
    // Key events
    ViewEvent event;
    while ((event = getKeyboardEvent()) != EVENT_NONE)
    {
        // Powered off: power key powers on, other keys show the battery indicator
        if (!isPoweredOn())
        {
            if (event == EVENT_KEY_POWER)
                powerOn();
            else
                view.onViewEvent(EVENT_KEY_TOGGLEBACKLIGHT);

            continue;
        }

        if (event == EVENT_KEY_POWER)
        {
            // Locked: power key only wakes the display
            if (!view.lockModeEnabled)
            {
                triggerAndWaitForVibration();

                powerOff();

                continue;
            }
        }
        else if (event == EVENT_KEY_TOGGLELOCK)
        {
            bool lock = !view.lockModeEnabled;

            // Locking also turns the display off
            if (lock)
            {
                setMeasurementView();

                event = EVENT_KEY_TOGGLEBACKLIGHT;
            }

            setLockModeEnabled(lock);
        }
        else
            view.onViewEvent(event);

        if ((event == EVENT_KEY_TOGGLEBACKLIGHT) && isDisplayAwake())
            cancelBacklight();
        else
            requestBacklightTrigger();
    }

    // Pre-draw
    if (isBacklightTriggerRequested())
        view.drawUpdate = true;
#if defined(DISPLAY_COLOR)
    else if (!isBacklightActive() && !settings.pulseDisplayFlash)
    {
        // Display off: nothing to draw
        view.drawUpdate = false;

        if (isDisplayEnabled())
            setDisplayEnabled(false);
    }
#endif

    // Draw
    if (view.drawUpdate)
    {
        view.drawUpdate = false;

        view.onViewEvent(EVENT_DRAW);

#if defined(DISPLAY_MONOCHROME)
        refreshDisplay();
#elif defined(DISPLAY_COLOR)
        if (!isDisplayEnabled())
            setDisplayEnabled(true);
#endif

        if (isBacklightTriggerRequested())
            triggerBacklight();

        // Post-draw
        view.onViewEvent(EVENT_POSTDRAW);
    }
}

void requestViewUpdate(void)
{
    view.drawUpdate = true;
}

NOINLINE void showView(OnViewEvent *onViewEvent)
{
    view.onViewEvent = onViewEvent;

    requestViewUpdate();
}

void updateViewHeartbeat(void)
{
    view.onViewEvent(EVENT_HEARTBEAT);

    requestViewUpdate();
}

// Lock mode

void setLockModeEnabled(bool value)
{
    view.lockModeEnabled = value;

    requestViewUpdate();
    triggerVibration();
}

bool isLockModeEnabled(void)
{
    return view.lockModeEnabled;
}
