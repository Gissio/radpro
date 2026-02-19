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
#define PWR_CHRG_SAMPLE_NUM 1
#define PWR_CHRG_USB_CHARGING_THRESHOLD 500
#define PWR_CHRG_USB_POWERED_THRESHOLD 2500
#elif defined(EMFMETER)
// 400 samples @ 1 kHz fit whole cycles of both 50 Hz and 60 Hz
#define ELECTRIC_FIELD_SAMPLE_NUM 400
#define ELECTRIC_FIELD_OFFSET 126
// 0.9*sqrt(2)
#define ELECTRIC_FIELD_SCALE 1.27279220614F
#define MAGNETIC_FIELD_SAMPLE_NUM 400
#define MAGNETIC_FIELD_OFFSET 120
// 0.000000005*2.8583*sqrt(2)
#define MAGNETIC_FIELD_LINEAR_SCALE 20.2112331E-9F
// 0.000000005*0.0072*sqrt(2)
#define MAGNETIC_FIELD_QUADRATIC_SCALE 50.911688E-12F
// #define MAGNETIC_FIELD_QUADRATIC_SCALE 36E-9F
#endif

#define ADC_END_SAMPLE 1
#define ADC_PWR_BAT_SAMPLE (ADC_END_SAMPLE + PWR_BAT_SAMPLE_NUM)
#define ADC_VREF_SAMPLE (ADC_PWR_BAT_SAMPLE + VREF_SAMPLE_NUM)
#if defined(PWR_CHRG_CHANNEL)
#define ADC_PWR_CHRG_SAMPLE (ADC_VREF_SAMPLE + PWR_CHRG_SAMPLE_NUM)
#define ADC_START_SAMPLE ADC_PWR_CHRG_SAMPLE
#elif defined(EMFMETER)
#define ADC_MAGNETIC_FIELD_SAMPLE (ADC_VREF_SAMPLE + MAGNETIC_FIELD_SAMPLE_NUM)
#define ADC_ELECTRIC_FIELD_SAMPLE (ADC_MAGNETIC_FIELD_SAMPLE + ELECTRIC_FIELD_SAMPLE_NUM)
#define ADC_START_SAMPLE ADC_ELECTRIC_FIELD_SAMPLE
#else
#define ADC_START_SAMPLE ADC_VREF_SAMPLE
#endif

typedef enum
{
    ADC_STAGE_IDLE,
#if defined(PWR_CHRG_CHANNEL)
    ADC_STAGE_PWR_CHRG,
#elif defined(EMFMETER)
    ADC_STAGE_ELECTRIC_FIELD,
    ADC_STAGE_MAGNETIC_FIELD,
#endif
    ADC_STAGE_VREF,
    ADC_STAGE_PWR_BAT,
} ADCStage;

typedef struct
{
    uint32_t battery;
    uint32_t vref;
#if defined(EMFMETER)
    uint32_t electricField;
    uint32_t magneticField;
#endif
} ADCValues;

static struct
{
    ADCValues live;
    ADCValues snapshot;

#if defined(PWR_CHRG_CHANNEL)
    volatile bool usbPowered;
    volatile bool batteryCharging;
#endif

    bool initialized;
    bool enabled;
    ADCStage stage;
} adcHardware;

#if defined(PWR_CHRG_CHANNEL)

void updateADCPowerCharge(int32_t value)
{
    adcHardware.batteryCharging = (value < PWR_CHRG_USB_CHARGING_THRESHOLD);
    adcHardware.usbPowered = adcHardware.batteryCharging || (value > PWR_CHRG_USB_POWERED_THRESHOLD);
}

#endif

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

    adc_enable(ADC1);

    adc_set_sampletime(ADC1, 7);
    adc_set_channel(ADC1, PWR_BAT_CHANNEL);
    adc_trigger_conversion(ADC1);
    while (!adc_ready(ADC1))
        ;
    adcHardware.snapshot.battery = PWR_BAT_SAMPLE_NUM * adc_read(ADC1);

#if defined(PWR_CHRG_CHANNEL)
    adc_set_channel(ADC1, PWR_CHRG_CHANNEL);
    adc_trigger_conversion(ADC1);
    while (!adc_ready(ADC1))
        ;
    int32_t value = adc_read(ADC1);
    updateADCPowerCharge(value);
#endif

    adc_disable(ADC1);

    adcHardware.initialized = true;
}

void onADCTick(uint32_t index)
{
    if (!adcHardware.enabled)
    {
        if ((index == ADC_END_SAMPLE) && adcHardware.initialized)
            adcHardware.enabled = true;

        return;
    }

    if (index > ADC_START_SAMPLE)
        return;

    // Stage processing
    int32_t value = adc_read(ADC1);
    switch (adcHardware.stage)
    {
    case ADC_STAGE_IDLE:
        break;

#if defined(PWR_CHRG_CHANNEL)
    case ADC_STAGE_PWR_CHRG:
        updateADCPowerCharge(value);

        break;

#elif defined(EMFMETER)
    case ADC_STAGE_ELECTRIC_FIELD:
        value -= ELECTRIC_FIELD_OFFSET;
        if (value > 0)
            adcHardware.live.electricField = addClamped(adcHardware.live.electricField, value * value);

        break;

    case ADC_STAGE_MAGNETIC_FIELD:
        value -= MAGNETIC_FIELD_OFFSET;
        if (value > 0)
            adcHardware.live.magneticField = addClamped(adcHardware.live.magneticField, value * value);

        break;
#endif

    case ADC_STAGE_VREF:
        adcHardware.live.vref += value;

        break;

    case ADC_STAGE_PWR_BAT:
        adcHardware.live.battery += value;

        break;
    }

    // Stage setup
    switch (index)
    {
#if defined(PWR_CHRG_CHANNEL)
    case ADC_PWR_CHRG_SAMPLE:
        adcHardware.stage = ADC_STAGE_PWR_CHRG;

        adc_enable(ADC1);

        adc_set_channel(ADC1, PWR_CHRG_CHANNEL);

        break;

#elif defined(EMFMETER)
    case ADC_ELECTRIC_FIELD_SAMPLE:
        adcHardware.stage = ADC_STAGE_ELECTRIC_FIELD;

        adc_enable(ADC1);

        adc_set_channel(ADC1, ELECTRIC_FIELD_CHANNEL);

        break;

    case ADC_MAGNETIC_FIELD_SAMPLE:
        adcHardware.stage = ADC_STAGE_MAGNETIC_FIELD;

        adc_set_channel(ADC1, MAGNETIC_FIELD_CHANNEL);

        break;
#endif

    case ADC_VREF_SAMPLE:
        adcHardware.stage = ADC_STAGE_VREF;

#if !defined(PWR_CHRG_CHANNEL) && !defined(EMFMETER)
        adc_enable(ADC1);
#endif

        adc_enable_vref_channel(ADC1);
        adc_enable_temperature_vref_channel(ADC1);
        adc_set_channel(ADC1, ADC_VREF_CHANNEL);

        break;

    case ADC_PWR_BAT_SAMPLE:
        adcHardware.stage = ADC_STAGE_PWR_BAT;

        adc_disable_vref_channel(ADC1);
        adc_disable_temperature_vref_channel(ADC1);
        adc_set_channel(ADC1, PWR_BAT_CHANNEL);

        break;

    case ADC_END_SAMPLE:
        adc_disable(ADC1);

        adcHardware.stage = ADC_STAGE_IDLE;

        adcHardware.snapshot = adcHardware.live;
        memset(&adcHardware.live, 0, sizeof(adcHardware.live));

        break;
    }

    // Stage conversion
    adc_trigger_conversion(ADC1);
}

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

float readBatteryVoltage(void)
{
#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    // +++ TEST
    return ((ADC_VDD / ADC_VALUE_MAX / PWR_BAT_SAMPLE_NUM) * PWR_BAT_SCALE_FACTOR) * adcHardware.snapshot.battery;
    // return ((VREFINT_CAL_VOLTAGE / ADC_VALUE_MAX) * PWR_BAT_SCALE_FACTOR / PWR_BAT_SAMPLE_NUM) * VREFINT_CAL_VALUE * adcHardware.snapshot.battery / adcHardware.snapshot.vref;
    // +++ TEST
#else
    // return (VREFINT_VOLTAGE * PWR_BAT_SCALE_FACTOR * VREF_SAMPLE_NUM / PWR_BAT_SAMPLE_NUM) * adcHardware.snapshot.battery / adcHardware.snapshot.vref;
#endif
}

#if defined(EMFMETER)

float readElectricFieldStrength(void)
{
    float rmsValue = sqrtf((float)adcHardware.snapshot.electricField / ELECTRIC_FIELD_SAMPLE_NUM);

    float value = ELECTRIC_FIELD_SCALE * rmsValue;

    if (value < 1E-6F)
        value = 1E-6F;

    return value;
}

float readMagneticFieldStrength(void)
{
    float rmsValue = sqrtf((float)adcHardware.snapshot.magneticField / MAGNETIC_FIELD_SAMPLE_NUM);

    float value = MAGNETIC_FIELD_LINEAR_SCALE * rmsValue + MAGNETIC_FIELD_QUADRATIC_SCALE * rmsValue * rmsValue;

    if (value < 1E-12F)
        value = 1E-12F;

    return value;
}

#endif

#endif
