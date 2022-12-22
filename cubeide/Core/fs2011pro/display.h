/*
 * FS2011 Pro
 * LCD interface
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>

#define HISTORY_VIEW_HEIGHT 40

#define GAME_MOVES_LINE_NUM 5

#define MENU_VIEW_LINE_NUM 4

typedef const char *GetMenuOption(void *userdata, unsigned int index);

void initDisplay();
void setDisplay(bool value);
void clearDisplay();
void updateDisplay();

void drawSelfTestError(unsigned value);

void drawWelcome();
void drawStatusBar();
void drawTitle(const char *title);
void drawSubtitle(const char *subtitle);

void drawMeasurementValue(const char *mantissa, const char *characteristic);
void drawConfidenceIntervals(int sampleNum);
void drawHistory(const char *minLabel, const char *maxLabel,
                 int offset, int range);

void drawStats();

void drawGameBoard(const char board[8][9],
                   const char time[2][6],
                   const char moveHistory[GAME_MOVES_LINE_NUM][2][6],
                   const char *buttonText, bool buttonSelected);

void drawMenu(GetMenuOption getMenuOption, void *userdate, int startIndex, int selectedIndex);

#endif
