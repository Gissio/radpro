/*
 * FS2011 Pro
 * Buzzer
 * 
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

#define BUZZER_TICK_FREQUENCY 100000

void triggerBuzzer(uint32_t buzzerTicks);
void onBuzzerOff();

#endif
