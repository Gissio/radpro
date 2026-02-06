/*
 * Rad Pro
 * Statistics
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stddef.h>

#include "../system/settings.h"
#include "../system/statistics.h"
#include "../ui/statistics.h"

// Statistics view

static void onStatisticsViewEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setSettingsMenu();

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
