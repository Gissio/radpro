/*
 * Rad Pro
 * Statistics
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "settings.h"
#include "stats.h"

static void onStatsViewDraw(const struct View *view)
{
    drawTitle("Statistics");
    drawStats();
}

static void onStatsViewKey(const struct View *view, KeyEvent keyEvent)
{
    if (keyEvent == KEY_EVENT_BACK)
        setView(&settingsMenuView);
}

const struct View statsView = {
    onStatsViewDraw,
    onStatsViewKey,
};
