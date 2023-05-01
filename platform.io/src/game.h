/*
 * FS2011 Pro
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

#include "keyboard.h"

void resetGame(uint8_t playerIndex);
bool isGameStart(void);

void updateGame(void);
void updateGameTimer(void);

void drawGameView(void);
void onGameViewKey(KeyEvent keyEvent);

#endif
