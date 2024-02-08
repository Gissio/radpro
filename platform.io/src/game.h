/*
 * Rad Pro
 * Nuclear chess
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(GAME_H)
#define GAME_H

#include "view.h"

#define GAME_HISTORY_TURN_NUM 3

extern const View gameMenuView;
extern const View gameView;

void initGame(void);

void updateGame(void);
void onGameOneSecond(void);

#endif
