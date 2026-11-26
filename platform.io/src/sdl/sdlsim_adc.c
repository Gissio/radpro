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

#include "../peripherals/adc.h"
#include "sdlsim.h"

#if !defined(M_PI)
#define M_PI 3.14159265358979323846
#endif

void initADC(void)
{
}

void onADCTick(uint32_t index)
{
}

static float getGaussianRandomValue(void)
{
    // Box-Muller transform; u1 in (0, 1] so log(u1) stays finite
    double u1 = 1.0 - getUniformRandomValue();
    double u2 = getUniformRandomValue();

    return (float)(sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2));
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
