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

void resetGame(int isPlayerBlack);
bool isGameStart(void);

void updateGame(void);
void updateGameTimer(void);

void drawGameView(void);
void onGameViewKey(int keyEvent);

#endif
