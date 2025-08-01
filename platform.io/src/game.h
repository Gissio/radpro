/*
 * Rad Pro
 * Nuclear chess
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(GAME_H)
#define GAME_H

#include "view.h"

#define GAME_HISTORY_TURN_NUM 3

extern View gameMenuView;

void resetGame(void);

void dispatchGameEvents(void);
void updateGame(void);

#endif
