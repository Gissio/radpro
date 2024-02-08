/*
 * Rad Pro
 * ADC
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(ADC_H)
#define ADC_H

#include <stdint.h>

void initADC(void);

uint32_t readADC(uint8_t channel, uint8_t sampleTime);

void updateADC(void);
void resetADCFilters(void);

uint32_t readBatteryValue(void);
float getBatteryVoltage(void);

uint32_t readDeviceTemperatureValue(void);
float getDeviceTemperature(void);

#endif
