/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(DISPLAY_H)
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#include "menu.h"
#include "view.h"

extern const View displayMenuView;

#if defined(DISPLAY_128X64)

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_UPSCALE 3

#elif defined(DISPLAY_320X240)

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_UPSCALE 1

#elif defined(DISPLAY_240X320)

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320
#define DISPLAY_UPSCALE 1

#endif

enum MeasurementStyle
{
    MEASUREMENTSTYLE_NORMAL,
    MEASUREMENTSTYLE_ALARM,
    MEASUREMENTSTYLE_HOLD,
};

extern const uint32_t menuLineNum;

extern uint16_t displayBrightnessValue[];

void initDisplay(void);
void initDisplayHardware(void);

void setDisplay(bool value);
void updateDisplayContrast();
void setDisplayBacklight(bool value);
void syncDisplayBacklight(void);
void refreshDisplay(void);

void drawStatusBar(void);
void drawNotification(const char *title,
                      const char *subtitle,
                      bool fullscreen);
void drawMenu(const Menu *menu);
void drawLowBattery(void);
void drawMeasurement(const char *title,
                     const char *valueString,
                     const char *unitString,
                     float confidence,
                     const char *time,
                     const char *stateString,
                     const char *stateValueString,
                     const char *stateUnitString,
                     enum MeasurementStyle style);
void drawHistory(const char *title,
                 const char *topLegendString,
                 const char *bottomLegendString,
                 const uint8_t *data,
                 uint32_t xTickNum,
                 uint32_t yTickNum);
void drawRNG(const char *title,
             const char *rngString,
             const char *stateString);
void drawStatistics(void);
void drawGame(const uint8_t board[8][8],
              const char time[2][16],
              const char history[3][2][6]);
void drawTestMode(const char lines[8][32]);

#endif
