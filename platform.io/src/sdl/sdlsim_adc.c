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

#if defined(EMFMETER)

// Same front end as the FNIRSI GC-03
const FieldSensor electricFieldSensor = {
    126,
    2 * 0.9F,
    0.0F,
};

const FieldSensor magneticFieldSensor = {
    120,
    2 * 1.41421356237F * 0.000000005F * 2.8583F,
    2 * 1.41421356237F * 0.000000005F * 0.0072F,
};

#define SIMULATOR_FIELD_FREQUENCY 50.0
#define SIMULATOR_FIELD_NOISE 1.0
// ~76 V/m
#define SIMULATOR_ELECTRIC_FIELD_AMPLITUDE 84.0
// ~0.25 uT
#define SIMULATOR_MAGNETIC_FIELD_AMPLITUDE 12.0

static const uint16_t *generateFieldSamples(uint16_t *samples, int32_t offset, double amplitude)
{
    // Mains sine sampled at 1 kHz, clipped at the ADC range
    double phase = 2.0 * M_PI * getUniformRandomValue();

    for (uint32_t i = 0; i < FIELD_SAMPLE_NUM; i++)
    {
        double value = offset +
                       amplitude * sin(2.0 * M_PI * SIMULATOR_FIELD_FREQUENCY * i / 1000.0 + phase) +
                       SIMULATOR_FIELD_NOISE * getGaussianRandomValue();

        samples[i] = (uint16_t)fmin(fmax(round(value), 0), 4095);
    }

    return samples;
}

const uint16_t *getElectricFieldSamples(void)
{
    static uint16_t samples[FIELD_SAMPLE_NUM];

    return generateFieldSamples(samples, electricFieldSensor.offset, SIMULATOR_ELECTRIC_FIELD_AMPLITUDE);
}

const uint16_t *getMagneticFieldSamples(void)
{
    static uint16_t samples[FIELD_SAMPLE_NUM];

    return generateFieldSamples(samples, magneticFieldSensor.offset, SIMULATOR_MAGNETIC_FIELD_AMPLITUDE);
}

#endif

#endif
