/*
 * Rad Pro
 * System
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "settings.h"

// Statistics view

static void onStatisticsViewEvent(const View *view, enum Event event)
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
