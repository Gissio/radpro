/*
 * Rad Pro
 * ADC
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(ADC_H)
#define ADC_H

#include <stdint.h>

void initADC(void);

void onADCTick(uint32_t index);

float readBatteryVoltage(void);
float readElectricFieldStrength(void);
float readMagneticFieldStrength(void);

#endif
