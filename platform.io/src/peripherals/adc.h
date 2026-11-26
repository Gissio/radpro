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

#if defined(EMFMETER)

// 400 samples @ 1 kHz to fit whole cycles of both 50 Hz and 60 Hz
#define FIELD_SAMPLE_NUM 400

// Field sensor front end: ADC baseline and empirical calibration
// (value = linearScale * r + quadraticScale * r^2, where r is the RMS of
// the half-wave above the baseline)
typedef struct
{
    int32_t offset;
    float linearScale;
    float quadraticScale;
} FieldSensor;

extern const FieldSensor electricFieldSensor;
extern const FieldSensor magneticFieldSensor;

const uint16_t *getElectricFieldSamples(void);
const uint16_t *getMagneticFieldSamples(void);

#endif

#endif
