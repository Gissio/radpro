/*
 * Rad Pro
 * Simulator ADC
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdint.h>

#include "../adc.h"

void initADC(void)
{
}

uint32_t readADC(uint8_t channel, uint8_t sampleTime)
{
    return 0x1234;
}

void updateADC(void)
{
}

float getDeviceBatteryVoltage(void)
{
    return 1.296F;
}

float getDeviceTemperature(void)
{
    return 24.2F;
}

#endif
