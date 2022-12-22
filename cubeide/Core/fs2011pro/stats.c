/*
 * FS2011 Pro
 * Statistics
 * 
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include "display.h"
#include "keyboard.h"
#include "stats.h"
#include "ui.h"

void drawStatsView()
{
    drawTitle("Statistics");
    drawStats();
}

void onStatsViewKey(int key)
{
    if (key == KEY_BACK)
        setView(VIEW_MENU);
}
