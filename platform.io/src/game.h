/*
 * Rad Pro
 * Nuclear chess
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#include "view.h"

extern const struct View gameMenuView;

void initGame(void);

void updateGame(void);
void updateGameTimer(void);

#endif
