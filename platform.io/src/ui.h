/*
 * FS2011 Pro
 * User interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef UI_H
#define UI_H

#include <stdint.h>

enum ViewEnum
{
    VIEW_INSTANTANEOUS_RATE,
    VIEW_AVERAGE_RATE,
    VIEW_DOSE,
    VIEW_HISTORY,
    VIEW_MENU,
    VIEW_RNG,
    VIEW_STATS,
    VIEW_GAME,
};

typedef uint8_t View;

void updateUI(void);

void setView(View view);
View getView(void);
void updateView(void);

#endif
