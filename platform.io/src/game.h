/*
 * Rad Pro
 * Nuclear chess
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(GAME_H)

#define GAME_H

#include "view.h"

#define GAME_MOVES_LINE_NUM 5

extern const struct View gameMenuView;

void initGame(void);

void updateGame(void);
void updateGameTimer(void);

#endif
