/*
 * Rad Pro
 * SDLSim ADC
 *
 * (C) 2022-2023 Gissio
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

void updateADCHardware(void)
{
}

uint32_t getBatteryValue(void)
{
    return 3300;
}

float getBatteryVoltage(void)
{
    return getBatteryValue() * (3.3F / 4096 / 2);
}

float getDeviceTemperature(void)
{
    return 24.2F;
}

#endif
