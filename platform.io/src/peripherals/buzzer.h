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
#include <stdint.h>

void initBuzzer(void);

void setBuzzerVolume(uint8_t volume);
void setBuzzer(bool value);

void onBuzzerTick(void);

#endif
