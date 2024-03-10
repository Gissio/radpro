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
#include "game.h"
#include "power.h"
#include "settings.h"
#include "view.h"

static struct
{
    const View *currentView;

    bool drawUpdate;

    bool displayEnabled;
} view;

void dispatchViewEvents(void)
{
    syncTimerThread();

#if defined(DISPLAY_COLOR)
    bool displayOn = isDisplayOn();
#endif
    bool displayTimerActive = isDisplayTimerActive();

    // Key events

    while (true)
    {
        Event event = getKeyboardEvent();
        if (event == EVENT_NONE)
            break;

#if defined(DISPLAY_MONOCHROME)
        if (((settings.displaySleep == DISPLAY_SLEEP_ALWAYS_OFF) ||
             (settings.displaySleep == DISPLAY_SLEEP_PULSE_FLASHES)) ||
            displayTimerActive)
#elif defined(DISPLAY_COLOR)
        if (displayTimerActive)
#endif
        {
            if (event == EVENT_KEY_POWER)
                requestPowerOff();
            else
            {
                triggerDisplay();

                view.currentView->onEvent(view.currentView, event);
            }
        }
        else
            triggerDisplay();
    }

    // Draw events

#if defined(DISPLAY_COLOR)
    if (displayOn &&
        !displayTimerActive)
    {
        setDisplayBacklight(false);
        setDisplayOn(false);
    }

    if (displayOn != displayTimerActive)
        view.drawUpdate = displayTimerActive;
#endif

    if (view.drawUpdate)
    {
        view.drawUpdate = false;

        drawStatusBar();

        view.currentView->onEvent(view.currentView, EVENT_DRAW);

        refreshDisplay();
    }

#if defined(DISPLAY_COLOR)
    if (!displayOn &&
        displayTimerActive)
    {
        setDisplayBacklight(true);
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
