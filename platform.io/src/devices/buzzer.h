/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(BUZZER_H)
#define BUZZER_H

#include <stdbool.h>

#include "../ui/view.h"

extern View soundPulsesMenuView;

void initBuzzer(void);

void setupBuzzer(void);

void setBuzzer(bool value);

void onBuzzerTick(void);

#endif
