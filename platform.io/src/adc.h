/*
 * Rad Pro
 * ADC
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(ADC_H)

#define ADC_H

#include <stdint.h>

void initADC(void);

uint32_t readADC(uint8_t channel, uint8_t sampleTime);

void updateADC(void);
void updateADCHardware(void);

float getBatteryVoltage(void);

float getDeviceTemperature(void);

#endif
