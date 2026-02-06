/*
 * Rad Pro
 * Display
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(DISPLAY_H)
#define DISPLAY_H

#include <stdbool.h>

#include "../ui/view.h"

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

extern View displayMenuView;

void initDisplay(void);
void initBacklight(void);

void setupDisplay(void);

void setDisplayEnabled(bool value);
bool isDisplayEnabled(void);

void setBacklight(bool value);

void updateDisplayContrast(void);

void refreshDisplay(void);

#endif
