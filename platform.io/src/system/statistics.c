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
#include "../ui/view.h"

// Statistics view

static void onStatisticsViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        showSettingsMenu();

        break;

    case EVENT_DRAW:
        drawStatistics();

        break;

    default:
        break;
    }
}

void showStatisticsView(void)
{
    showView(onStatisticsViewEvent);
}
