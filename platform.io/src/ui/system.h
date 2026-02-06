/*
 * Rad Pro
 * UI system
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(UI_SYSTEM_H)
#define UI_SYSTEM_H

#include <stdbool.h>

void drawPowerOff(bool displayBatteryIcon);
void drawTitleBar(const char *title);
void drawSplash(const char *message);
void drawNotification(const char *title, const char *message);

#endif
