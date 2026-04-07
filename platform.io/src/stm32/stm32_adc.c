/*
 * Rad Pro
 * STM32 ADC
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <string.h>

#include "../peripherals/adc.h"
#include "../stm32/device.h"
#include "../system/cmath.h"
#include "../system/events.h"

#define ADC_VDD 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)

#define PWR_BAT_SAMPLE_NUM 16
#define VREF_SAMPLE_NUM 16

#if defined(PWR_CHRG_CHANNEL)
#define PWR_CHRG_SAMPLE_NUM 16
#define PWR_CHRG_USB_CHARGING_THRESHOLD 500
#define PWR_CHRG_USB_POWERED_THRESHOLD 2500
#endif

#if defined(EMFMETER)
#define SQRT2 1.41421356237F
// 400 samples @ 1 kHz to fit whole cycles of both 50 Hz and 60 Hz
#define ELECTRIC_FIELD_SAMPLE_NUM 400
#define ELECTRIC_FIELD_SCALE (SQRT2 * 0.9F)
// 400 samples @ 1 kHz to fit whole cycles of both 50 Hz and 60 Hz
#define MAGNETIC_FIELD_SAMPLE_NUM 400
#define MAGNETIC_FIELD_LINEAR_SCALE (SQRT2 * 0.000000005F * 2.8583F)
#define MAGNETIC_FIELD_QUADRATIC_SCALE (SQRT2 * 0.000000005F * 0.0072F)

#define ADC_END_SAMPLE 1
#define ADC_PWR_BAT_SAMPLE (ADC_END_SAMPLE + PWR_BAT_SAMPLE_NUM)
#define ADC_VREF_SAMPLE (ADC_PWR_BAT_SAMPLE + VREF_SAMPLE_NUM)
#define ADC_MAGNETIC_FIELD_SAMPLE (ADC_VREF_SAMPLE + MAGNETIC_FIELD_SAMPLE_NUM)
#define ADC_ELECTRIC_FIELD_SAMPLE (ADC_MAGNETIC_FIELD_SAMPLE + ELECTRIC_FIELD_SAMPLE_NUM)
#define ADC_START_SAMPLE ADC_ELECTRIC_FIELD_SAMPLE

typedef enum
{
    ADC_STAGE_IDLE,
    ADC_STAGE_ELECTRIC_FIELD,
    ADC_STAGE_MAGNETIC_FIELD,
    ADC_STAGE_VREF,
    ADC_STAGE_PWR_BAT,
} ADCStage;
#endif

typedef struct
{
    uint32_t battery;
    uint32_t vref;
#if defined(EMFMETER)
    uint16_t electricField[ELECTRIC_FIELD_SAMPLE_NUM];
    uint16_t magneticField[MAGNETIC_FIELD_SAMPLE_NUM];
    uint32_t electricFieldIndex;
    uint32_t magneticFieldIndex;
#endif
} ADCValues;

static struct
{
#if defined(PWR_CHRG_CHANNEL)
    volatile bool usbPowered;
    volatile bool batteryCharging;
#endif

#if defined(EMFMETER)
    bool initialized;
    bool enabled;
    ADCStage stage;

    ADCValues live;
#endif
    ADCValues snapshot;
} adcHardware;

// Conversion

static uint32_t convertADC(uint32_t sampleNum)
{
    uint32_t value = 0;

    for (uint32_t i = 0; i < sampleNum; i++)
    {
        adc_trigger_conversion(ADC1);
        while (!adc_ready(ADC1))
            ;
        value += adc_read(ADC1);
    }

    return value;
}

static void updatePowerState(void)
{
    adc_enable(ADC1);

    adc_set_sampletime(ADC1, 5);

    // Sample battery
    adc_set_channel(ADC1, PWR_BAT_CHANNEL);
    adcHardware.snapshot.battery = convertADC(PWR_BAT_SAMPLE_NUM);

    // Sample VREF
    adc_enable_vref_channel(ADC1);
    adc_enable_temperature_vref_channel(ADC1);
    adc_set_channel(ADC1, ADC_VREF_CHANNEL);
    adcHardware.snapshot.vref = convertADC(VREF_SAMPLE_NUM);
    adc_disable_vref_channel(ADC1);
    adc_disable_temperature_vref_channel(ADC1);

    // Sample PWR_CHRG
#if defined(PWR_CHRG_CHANNEL)
    adc_set_channel(ADC1, PWR_CHRG_CHANNEL);
    uint32_t pwrChrgValue = convertADC(PWR_CHRG_SAMPLE_NUM) / PWR_CHRG_SAMPLE_NUM;

    adcHardware.batteryCharging = (pwrChrgValue < PWR_CHRG_USB_CHARGING_THRESHOLD);
    adcHardware.usbPowered = adcHardware.batteryCharging || (pwrChrgValue > PWR_CHRG_USB_POWERED_THRESHOLD);
#endif

    adc_disable(ADC1);
}

#if defined(EMFMETER)

void onADCTick(uint32_t index)
{
    if (!adcHardware.enabled)
    {
        if ((index == ADC_START_SAMPLE) && adcHardware.initialized)
            adcHardware.enabled = true;

        return;
    }

    if (index > ADC_START_SAMPLE)
        return;

    // Processing
    int32_t value = adc_read(ADC1);
    switch (adcHardware.stage)
    {
    case ADC_STAGE_IDLE:
        break;

    case ADC_STAGE_ELECTRIC_FIELD:
        adcHardware.live.electricField[adcHardware.live.electricFieldIndex++] = value;

        break;

    case ADC_STAGE_MAGNETIC_FIELD:
        adcHardware.live.magneticField[adcHardware.live.magneticFieldIndex++] = value;

        break;

    case ADC_STAGE_VREF:
        adcHardware.live.vref += value;

        break;

    case ADC_STAGE_PWR_BAT:
        adcHardware.live.battery += value;

        break;
    }

    // Setup
    switch (index)
    {
    case ADC_STAGE_IDLE:
        break;

    case ADC_ELECTRIC_FIELD_SAMPLE:
        adcHardware.stage = ADC_STAGE_ELECTRIC_FIELD;

        adc_set_channel(ADC1, ELECTRIC_FIELD_CHANNEL);

        break;

    case ADC_MAGNETIC_FIELD_SAMPLE:
        adcHardware.stage = ADC_STAGE_MAGNETIC_FIELD;

        adc_set_channel(ADC1, MAGNETIC_FIELD_CHANNEL);

        break;

    case ADC_VREF_SAMPLE:
        adcHardware.stage = ADC_STAGE_VREF;

        adc_enable_temperature_vref_channel(ADC1);
        adc_set_channel(ADC1, ADC_VREF_CHANNEL);

        break;

    case ADC_PWR_BAT_SAMPLE:
        adcHardware.stage = ADC_STAGE_PWR_BAT;

        adc_disable_temperature_vref_channel(ADC1);
        adc_set_channel(ADC1, PWR_BAT_CHANNEL);

        break;

    case ADC_END_SAMPLE:
        adcHardware.stage = ADC_STAGE_IDLE;

        adcHardware.snapshot = adcHardware.live;

        adcHardware.live.electricFieldIndex = 0;
        adcHardware.live.magneticFieldIndex = 0;

        break;
    }

    // Trigger conversion
    adc_trigger_conversion(ADC1);
}

#endif

// Initialization

void initADC(void)
{
    rcc_enable_adc(ADC1);

#if defined(PWR_BAT_PORT)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_analog(PWR_BAT_PORT, PWR_BAT_PIN, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PWR_BAT_PORT, PWR_BAT_PIN, GPIO_MODE_INPUT_ANALOG);
#endif
#endif

#if defined(EMFMETER)
    gpio_setup(ELECTRIC_FIELD_PORT, ELECTRIC_FIELD_PIN, GPIO_MODE_INPUT_ANALOG);
    gpio_setup(MAGNETIC_FIELD_PORT, MAGNETIC_FIELD_PIN, GPIO_MODE_INPUT_ANALOG);
#endif

#if defined(STM32G0)
    adc_enable_vreg(ADC1);
#elif defined(STM32L4)
    adc_disable_deep_power_down(ADC1);
    adc_enable_vreg(ADC1);
#endif

    adc_calibrate(ADC1);

#if defined(EMFMETER)
    updatePowerState();

    adc_enable(ADC1);

    adcHardware.initialized = true;
#endif
}

// Data access

#if defined(PWR_CHRG_CHANNEL)

bool isUSBPowered(void)
{
    return adcHardware.usbPowered;
}

bool isBatteryCharging(void)
{
    return adcHardware.batteryCharging;
}

#endif

float readBatteryVoltage()
{
#if !defined(EMFMETER)
    updatePowerState();
#endif

#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    float value = (VREFINT_CAL_VOLTAGE / ADC_VALUE_MAX * PWR_BAT_SCALE_FACTOR * VREF_SAMPLE_NUM / PWR_BAT_SAMPLE_NUM);
    value *= (float)(uint32_t)VREFINT_CAL_VALUE;
#else
    float value = (VREFINT_VOLTAGE * PWR_BAT_SCALE_FACTOR * VREF_SAMPLE_NUM / PWR_BAT_SAMPLE_NUM);
#endif
    value *= (float)adcHardware.snapshot.battery;
    value /= (float)adcHardware.snapshot.vref;

    return value;
}

#if defined(EMFMETER)

float calculateRMSValue(uint16_t *values, uint32_t count)
{
    if (count == 0)
        return 0;

    // Calculate mean value
    uint32_t meanValue = 0;
    for (uint32_t i = 0; i < count; i++)
        meanValue += values[i];
    meanValue /= count;

    // Filter by mean, and calculate low and high RMS values
    uint32_t squaredLow = 0;
    uint32_t squaredLowNum = 0;
    uint32_t squaredHigh = 0;
    uint32_t squaredHighNum = 0;
    for (uint32_t i = 0; i < count; i++)
    {
        uint32_t value = values[i];
        if (value < meanValue)
        {
            squaredLow = addClamped(squaredLow, value * value);
            squaredLowNum++;
        }
        else
        {
            squaredHigh = addClamped(squaredHigh, value * value);
            squaredHighNum++;
        }
    }

    if (squaredLowNum == 0 || squaredHighNum == 0)
        return 0;

    float rmsLow = sqrtf(squaredLow / squaredLowNum);
    float rmsHigh = sqrtf(squaredHigh / squaredHighNum);

    // Effective RMS value is the difference of high and low RMS values
    return rmsHigh - rmsLow;
}

float readElectricFieldStrength(void)
{
    float rmsValue = calculateRMSValue(adcHardware.snapshot.electricField,
                                       adcHardware.snapshot.electricFieldIndex);

    float electricFieldStrength = ELECTRIC_FIELD_SCALE * rmsValue;

    if (electricFieldStrength < 1E-6F)
        electricFieldStrength = 1E-6F;

    return electricFieldStrength;
}

float readMagneticFieldStrength(void)
{
    float rmsValue = calculateRMSValue(adcHardware.snapshot.magneticField,
                                       adcHardware.snapshot.magneticFieldIndex);

    float magneticFieldStrength = MAGNETIC_FIELD_LINEAR_SCALE * rmsValue + MAGNETIC_FIELD_QUADRATIC_SCALE * rmsValue * rmsValue;

    if (magneticFieldStrength < 1E-12F)
        magneticFieldStrength = 1E-12F;

    return magneticFieldStrength;
}

#endif

#endif
