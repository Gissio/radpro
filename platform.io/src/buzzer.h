/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(BUZZER_H)
#define BUZZER_H

#include <stdbool.h>

#include "view.h"

extern const View pulseClicksMenuView;

void initBuzzer(void);
void initBuzzerController(void);

#if defined(SIMULATOR)

void updateBuzzer(void);

#endif

void setBuzzer(bool value);

void playSystemAlert(void);

#endif
