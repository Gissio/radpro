/*
 * Rad Pro
 * System
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "settings.h"

// Statistics view

static void onStatisticsViewEvent(const struct View *view, enum Event event)
{
    switch (event)
    {
    case EVENT_BACK:
        setView(&settingsMenuView);

        break;

    case EVENT_DRAW:
        drawTitle("Statistics");
        drawStatistics();

        break;

    default:
        break;
    }
}

const struct View statisticsView = {
    onStatisticsViewEvent,
    NULL,
};
