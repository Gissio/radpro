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

#include "../ui/menu.h"

extern const Menu soundPulsesMenu;

void initBuzzer(void);

void setupBuzzer(void);

void setBuzzer(bool value);

void onBuzzerTick(void);

#endif
