/*
 * Rad Pro
 * UI history
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(UI_HISTORY_H)
#define UI_HISTORY_H

#include <stdint.h>

#if defined(DISPLAY_128X64)
#define HISTORY_BIN_NUM 121
#elif defined(DISPLAY_320X240)
#define HISTORY_BIN_NUM 301
#elif defined(DISPLAY_240X320)
#define HISTORY_BIN_NUM 201
#endif

#define HISTORY_VALUE_MIN 0.02F
#define HISTORY_DECADE 40
#define HISTORY_MARGIN ((uint32_t)(0.2F * HISTORY_DECADE))

void drawHistory(float scale, const char *unitString, uint32_t timeTicksNum, const char *periodLabel, const uint8_t *value, uint32_t warningValue, uint32_t alarmValue);

#endif
