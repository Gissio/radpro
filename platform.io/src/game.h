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

extern const View gameMenuView;

void initGame(void);
void initGameMenus(void);

void dispatchGameEvents(void);
void updateGame(void);

#endif
