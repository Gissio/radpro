/*
 * FS2011 Pro
 * Nuclear chess
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef GAME_H
#define GAME_H

void resetGame(int isPlayerBlack);
bool isGameStart();

void updateGame();
void updateGameTimer();

void drawGameView();
void onGameViewKey(int key);

#endif
