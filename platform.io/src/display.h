/*
 * Rad Pro
 * LCD interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#include "menu.h"

#ifdef DISPLAY_128X64

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define DISPLAY_CENTER_X (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_Y (DISPLAY_HEIGHT / 2)

#define TITLE_Y (DISPLAY_CENTER_Y - 19)
#define SUBTITLE_Y (DISPLAY_CENTER_Y + 19 + 5)

#define MENU_VIEW_LINE_NUM 4

#define MEASUREMENT_VALUE_X (DISPLAY_CENTER_X - 54)
#define MEASUREMENT_VALUE_Y (DISPLAY_CENTER_Y + 24 / 2)
#define MEASUREMENT_VALUE_SIDE_X (DISPLAY_CENTER_X + 29)

#define HISTORY_VIEW_HEIGHT 40
#define HISTORY_VIEW_X ((DISPLAY_WIDTH - HISTORY_VIEW_WIDTH) / 2)
#define HISTORY_VIEW_Y_TOP 14
#define HISTORY_VIEW_Y_BOTTOM (HISTORY_VIEW_Y_TOP + HISTORY_VIEW_HEIGHT)

#define STATS_VIEW_Y 30

#define GAME_MOVES_LINE_NUM 5

#define GAME_VIEW_BOARD_X 0
#define GAME_VIEW_BOARD_Y 8
#define GAME_VIEW_BOARD_WIDTH 9 * 8
#define GAME_VIEW_TIME_X (GAME_VIEW_BOARD_WIDTH + 7)
#define GAME_VIEW_TIME_UPPER_Y 8
#define GAME_VIEW_TIME_LOWER_Y (DISPLAY_HEIGHT - 3)
#define GAME_VIEW_MOVES_LINE_WIDTH 25
#define GAME_VIEW_MOVES_LINE_HEIGHT 6
#define GAME_VIEW_MOVES_X (GAME_VIEW_BOARD_WIDTH + 6)
#define GAME_VIEW_MOVES_Y (DISPLAY_CENTER_Y + 5 - GAME_VIEW_MOVES_LINE_HEIGHT * GAME_MOVES_LINE_NUM / 2)
#define GAME_VIEW_BUTTON_X 101
#define GAME_VIEW_BUTTON_Y 54
#define GAME_VIEW_BUTTON_WIDTH 23
#define GAME_VIEW_BUTTON_HEIGHT 9

#define MENU_VIEW_Y_TOP 14
#define MENU_VIEW_LINE_HEIGHT 12
#define MENU_VIEW_LINE_TEXT_X 6

#endif

void initDisplay(void);

void setDisplay(bool value);
void clearDisplayBuffer(void);
void sendDisplayBuffer(void);

void setBacklight(bool value);
bool getBacklight(void);

void drawLowBattery(void);
void drawWelcome(void);
void drawStatusBar(void);
void drawTitle(const char *title);
void drawSubtitle(const char *subtitle);

void drawMeasurementValue(const char *mantissa, const char *characteristic);
void drawConfidenceIntervals(uint32_t sampleNum);
void drawHistory(const char *minLabel, const char *maxLabel,
                 int32_t offset, uint32_t range);

void drawRNGText(char *text);

void drawStats(void);

void drawGameBoard(const char board[8][9],
                   const char time[2][6],
                   const char moveHistory[GAME_MOVES_LINE_NUM][2][6],
                   const char *buttonText, bool buttonSelected);

void drawMenu(const struct Menu *menu);

#endif
