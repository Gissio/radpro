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

void initBuzzer(void);

#if defined(SIMULATOR)

void updateBuzzer(void);

#endif

void setBuzzer(bool value);

#endif
