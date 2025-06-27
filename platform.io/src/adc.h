/*
 * Rad Pro
 * ADC
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(ADC_H)
#define ADC_H

#include <stdint.h>

void initADC(void);

float readBatteryVoltage(void);

#endif
