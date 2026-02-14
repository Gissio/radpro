/*
 * Rad Pro
 * Simulator ADC
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "../peripherals/adc.h"

#if !defined(M_PI)
#define M_PI 3.1415926535F
#endif

void initADC(void)
{
}

void onADCTick(uint32_t index)
{
}

float getGaussianRandomValue(void)
{
    float u1 = (float)rand() / (float)RAND_MAX;
    float u2 = (float)rand() / (float)RAND_MAX;

    return sqrtf(-2 * logf(u1)) * cosf(2 * M_PI * u2);
}

float readBatteryVoltage(void)
{
    return 3.854F + 0.005F * getGaussianRandomValue();
}

float readElectricFieldStrength(void)
{
    return 76.0F + 0.1F * getGaussianRandomValue();
}

float readMagneticFieldStrength(void)
{
    return 0.25E-6F + 0.1E-8F * getGaussianRandomValue();
}

#endif
