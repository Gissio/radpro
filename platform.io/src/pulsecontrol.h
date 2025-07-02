/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(PULSECONTROL_H)
#define PULSECONTROL_H

#include <stdbool.h>

void initPulseControl(void);

void setPulseControl(bool value);
void updatePulseControl(void);

void onPulseControlTick(void);

#endif
