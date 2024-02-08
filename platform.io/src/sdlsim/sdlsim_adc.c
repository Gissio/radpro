/*
 * Rad Pro
 * SDLSim ADC
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdint.h>

#include "../adc.h"

void initADC(void)
{
}

uint32_t readADC(uint8_t channel, uint8_t sampleTime)
{
    return 0x1234;
}

uint32_t readBatteryValue(void)
{
    return 3200;
}

uint32_t readDeviceTemperatureValue(void)
{
    return 242;
}

void updateADC(void)
{
}

void resetADCFilters(void)
{
}

float getBatteryVoltage(void)
{
    return readBatteryValue() * (3.3F / 4096 / 2);
}

float getDeviceTemperature(void)
{
    return 0.1F * readDeviceTemperatureValue();
}

#endif
