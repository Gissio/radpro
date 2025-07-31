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
    View *currentView;

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

        if (isDeviceOff())
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

                        event = EVENT_KEY_TOGGLEBACKLIGHT;
                    }
                    else
                        setLockMode(false);
                }
                else
                    view.currentView->onEvent(event);

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
                                    isPulseThresholdExceeded();
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

        view.currentView->onEvent(EVENT_DRAW);

#if defined(DISPLAY_MONOCHROME)
        refreshDisplay();
#elif defined(DISPLAY_COLOR)
        if (!isDisplayEnabled())
            enableDisplay(true);
#endif

        if (isBacklightTriggerRequested())
            triggerBacklight();

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

void updateView(void)
{
    view.currentView->onEvent(EVENT_PERIOD);

    view.drawUpdate = true;
}

// Lock mode

static bool viewLockMode;

void setLockMode(bool value)
{
    viewLockMode = value;

    updateView();
    triggerVibration();
}

bool isLockMode(void)
{
    return viewLockMode;
}

// Pulse sound icon

static bool viewPulseSoundIconEnabled;

void setPulseSoundIconEnabled(bool value)
{
    viewPulseSoundIconEnabled = value;

    updateView();
}

bool isPulseSoundIconEnabled(void)
{
    return viewPulseSoundIconEnabled;
}
