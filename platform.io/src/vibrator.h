/*
 * Rad Pro
 * Vibrator
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(VIBRATOR_H)
#define VIBRATOR_H

#include <stdbool.h>

#include "view.h"

extern const View pulseVibrationsMenuView;

void initVibrator(void);
void initVibratorController(void);

void setVibrator(bool value);

#endif
