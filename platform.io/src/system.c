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

static void onStatisticsViewEvent(Event event)
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

View statisticsView = {
    onStatisticsViewEvent,
    NULL,
};
