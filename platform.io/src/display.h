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
extern const View displayFlashesMenuView;

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

#if defined(DISPLAY_128X64)
#define HISTORY_BUFFER_SIZE 120
#elif defined(DISPLAY_320X240)
#define HISTORY_BUFFER_SIZE 300
#elif defined(DISPLAY_240X320)
#define HISTORY_BUFFER_SIZE 200
#endif

#define HISTORY_VALUE_MIN 0.02F
#define HISTORY_DECADE 40

typedef enum
{
    MEASUREMENTSTYLE_NORMAL,
    MEASUREMENTSTYLE_ALARM,
    MEASUREMENTSTYLE_HOLD,
} MeasurementStyle;

extern const uint32_t menuLineNum;

extern uint16_t displayBrightnessValue[];

void initDisplay(void);
void initDisplayController(void);
void initDisplayBacklight(void);

void setDisplayOn(bool value);
bool isDisplayOn(void);
void updateDisplayContrast(void);
void setDisplayBacklight(bool value);
void refreshDisplay(void);

void drawTitleBar(const char *title);
void drawNotification(const char *title,
                      const char *subtitle);
void drawMenu(const Menu *menu);
void drawLowBattery(void);
void drawMeasurementValue(const char *valueString,
                          const char *unitString,
                          float confidence,
                          MeasurementStyle style);
void drawMeasurementInfo(const char *infoKeyString,
                         const char *infoValueString,
                         const char *infoUnitString,
                         const char *stateString,
                         MeasurementStyle style);
void drawMeasurementBar(float value,
                        int32_t minExponent,
                        float alertZone1Value,
                        float alertZone2Value);
void drawHistory(float scale,
                 const char *unitString,
                 uint32_t timeTickNum,
                 const uint8_t *data,
                 uint8_t alertZone1Value,
                 uint8_t alertZone2Value);
void drawRNG(const char *title,
             const char *rngString,
             const char *stateString);
void drawStatistics(void);
void drawGame(const uint8_t board[8][8],
              const char time[2][16],
              const char history[3][2][6]);
void drawDataMode(void);
void drawTestMode(const char lines[8][32]);

#endif
