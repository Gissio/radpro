/*
 * FS2011 Pro
 * User interface
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef UI_H
#define UI_H

#include <stdint.h>

enum ViewIndex
{
    VIEW_WELCOME,
    VIEW_INSTANTANEOUS_RATE,
    VIEW_AVERAGE_RATE,
    VIEW_DOSE,
    VIEW_HISTORY,
    VIEW_MENU,
    VIEW_STATS,
    VIEW_GAME,
};

void setView(uint8_t viewIndex);
int getView();
void updateView();

void updateUI();

#endif
