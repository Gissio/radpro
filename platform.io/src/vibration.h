/*
 * Rad Pro
 * Vibration
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(VIBRATION_H)
#define VIBRATION_H

#include <stdbool.h>

#include "view.h"

extern const View pulseVibrationMenuView;

void initVibration(void);
void initVibrationController(void);

void setVibration(bool value);

#endif
