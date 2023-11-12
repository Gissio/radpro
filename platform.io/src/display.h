/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(DISPLAY_H)

#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#include "menu.h"
#include "view.h"

enum DisplayPalette
{
    COLOR_STATUS_BACKGROUND,
    COLOR_STATUS_FOREGOUND,
    COLOR_PRIMARY_BACKGROUND,
    COLOR_PRIMARY_FOREGROUND,
    COLOR_SECONDARY_FOREGROUND,
    COLOR_ALERT,
};

#if defined(PULSE_LED)

extern const struct View pulseLEDMenuView;

#endif

#if defined(DISPLAY_MONO)

extern const struct View backlightMenuView;

#elif defined(DISPLAY_COLOR)

extern const struct View displayMenuView;

#endif

extern const uint32_t menuLineNum;

void initDisplay(void);
void initDisplayHardware(void);

#if defined(PULSE_LED)

void setPulseLED(bool value);

#endif

void setDisplay(bool value);

#if defined(DISPLAY_MONO)

void setBacklight(bool value);
bool getBacklight(void);

#endif

void clearDisplayBuffer(void);
void sendDisplayBuffer(void);

void drawTextLeft(const char *str,
                  const uint8_t *font,
                  uint32_t x, uint32_t y);

void drawLowBattery(void);
void drawWelcome(void);
void drawStatusBar(void);
void drawMenu(const struct Menu *menu);
void drawTitle(const char *title);
void drawSubtitle(const char *subtitle);

void drawMeasurementValue(const char *value, const char *units);
void drawConfidenceIntervals(float lowerConfidenceInterval,
                             float upperConfidenceInterval);
void drawHistory(const char *minLabel, const char *maxLabel,
                 int32_t minLimit, uint32_t decades);

void drawRNGText(char *str);

void drawStatistics(void);

void drawGameBoard(const uint8_t board[8][8],
                   const char time[2][8],
                   const char moveHistory[][2][6]);

#endif
