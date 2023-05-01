/*
 * FS2011 Pro
 * Statistics
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "stats.h"
#include "ui.h"

void drawStatsView(void)
{
    drawTitle("Statistics");
    drawStats();
}

void onStatsViewKey(KeyEvent keyEvent)
{
    if (keyEvent == KEY_BACK)
        setView(VIEW_MENU);
}
