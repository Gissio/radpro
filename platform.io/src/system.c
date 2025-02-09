/*
 * Rad Pro
 * System
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "events.h"
#include "measurements.h"
#include "settings.h"

// Statistics view

static void onStatisticsViewEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setView(&settingsMenuView);

        break;

    case EVENT_DRAW:
        drawStatistics();

        break;

    default:
        break;
    }
}

const View statisticsView = {
    onStatisticsViewEvent,
    NULL,
};

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

static void onLockViewEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setView(&settingsMenuView);

        break;

    case EVENT_KEY_SELECT:
        setLockMode(true);

        setMeasurementView(-1);

        break;

    case EVENT_DRAW:
#if defined(FS2011)
        drawNotification("Enter lock mode?",
                         "POWER+PLAY to unlock.");
#elif defined(FS1000)
        drawNotification("Enter lock mode?",
                         "SET+MODE to unlock.");
#else
        drawNotification("Enter lock mode?",
                         "OK+BACK to unlock.");
#endif
        break;

    default:
        break;
    }
}

const View lockView = {
    onLockViewEvent,
    NULL,
};
