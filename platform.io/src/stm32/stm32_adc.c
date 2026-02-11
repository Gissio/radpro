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

#include "../devices/adc.h"
#include "../stm32/device.h"
#include "../system/cmath.h"
#include "../system/events.h"

#define ADC_VDD 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)

#define PWR_BAT_SAMPLE_NUM 16
#define VREF_SAMPLE_NUM 16

#if defined(PWR_CHRGANALOG_PORT)
#define PWR_CHRGANALOG_SAMPLE_NUM 1
#define PWR_CHRGANALOG_USB_CHARGING_THRESHOLD 500
#define PWR_CHRGANALOG_USB_POWERED_THRESHOLD 2500
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
#if defined(PWR_CHRGANALOG_PORT)
#define ADC_PWR_CHRGANALOG1_SAMPLE (ADC_VREF_SAMPLE + PWR_CHRGANALOG_SAMPLE_NUM)
#define ADC_PWR_CHRGANALOG2_SAMPLE (SYSTICK_FREQUENCY / 2)
#define ADC_START_SAMPLE ADC_PWR_CHRGANALOG2_SAMPLE
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
#if defined(PWR_CHRGANALOG_PORT)
    ADC_STAGE_PWR_CHRGANALOG,
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

#if defined(PWR_CHRGANALOG_PORT)
    volatile bool usbPowered;
    volatile bool batteryCharging;
#endif

    bool initialized;
    bool enabled;
    ADCStage stage;
} adcHardware;

#if defined(PWR_CHRGANALOG_PORT)
void updateADCChrgAnalog(int32_t value);
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

#if defined(PWR_CHRGANALOG_PORT)
    gpio_setup(PWR_CHRGANALOG_PORT, PWR_CHRGANALOG_PIN, GPIO_MODE_INPUT_ANALOG);
#elif defined(EMFMETER)
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

#if defined(PWR_CHRGANALOG_PORT)
    adc_set_channel(ADC1, PWR_CHRGANALOG_CHANNEL);
    adc_trigger_conversion(ADC1);
    while (!adc_ready(ADC1))
        ;
    int32_t value = adc_read(ADC1);
    updateADCChrgAnalog(value);
#endif

    adcHardware.initialized = true;
}

#if defined(PWR_CHRGANALOG_PORT)

void updateADCChrgAnalog(int32_t value)
{
    adcHardware.batteryCharging = (value < PWR_CHRGANALOG_USB_CHARGING_THRESHOLD);
    adcHardware.usbPowered = adcHardware.batteryCharging || (value > PWR_CHRGANALOG_USB_POWERED_THRESHOLD);
}

#endif

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

#if defined(PWR_CHRGANALOG_PORT)
    case ADC_STAGE_PWR_CHRGANALOG:
        adcHardware.stage = ADC_STAGE_IDLE;

        updateADCChrgAnalog(value);

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
#if defined(PWR_CHRGANALOG_PORT)
    case ADC_PWR_CHRGANALOG1_SAMPLE:
    case ADC_PWR_CHRGANALOG2_SAMPLE:
        adcHardware.stage = ADC_STAGE_PWR_CHRGANALOG;

        adc_set_channel(ADC1, PWR_CHRGANALOG_CHANNEL);

        break;

#elif defined(EMFMETER)
    case ADC_ELECTRIC_FIELD_SAMPLE:
        adcHardware.stage = ADC_STAGE_ELECTRIC_FIELD;

        adc_set_channel(ADC1, ELECTRIC_FIELD_CHANNEL);

        break;

    case ADC_MAGNETIC_FIELD_SAMPLE:
        adcHardware.stage = ADC_STAGE_MAGNETIC_FIELD;

        adc_set_channel(ADC1, MAGNETIC_FIELD_CHANNEL);

        break;
#endif

    case ADC_VREF_SAMPLE:
        adcHardware.stage = ADC_STAGE_VREF;

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
        adcHardware.stage = ADC_STAGE_IDLE;

        adcHardware.snapshot = adcHardware.live;
        memset(&adcHardware.live, 0, sizeof(adcHardware.live));

        break;
    }

    // Stage conversion
    adc_trigger_conversion(ADC1);
}

#if defined(PWR_CHRGANALOG_PORT)

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
    return ((VREFINT_CAL_VOLTAGE / ADC_VALUE_MAX) * PWR_BAT_SCALE_FACTOR / PWR_BAT_SAMPLE_NUM) * VREFINT_CAL_VALUE * adcHardware.snapshot.battery / adcHardware.snapshot.vref;
#else
    return (VREFINT_VOLTAGE * PWR_BAT_SCALE_FACTOR * VREF_SAMPLE_NUM / PWR_BAT_SAMPLE_NUM) * adcHardware.snapshot.battery / adcHardware.snapshot.vref;
#endif
}

#if defined(EMFMETER)

float readElectricFieldStrength(void)
{
    float rmsValue = sqrtf((float)adcHardware.snapshot.electricField / ELECTRIC_FIELD_SAMPLE_NUM);

    return ELECTRIC_FIELD_SCALE * rmsValue;
}

float readMagneticFieldStrength(void)
{
    float rmsValue = sqrtf((float)adcHardware.snapshot.magneticField / MAGNETIC_FIELD_SAMPLE_NUM);

    return MAGNETIC_FIELD_LINEAR_SCALE * rmsValue + MAGNETIC_FIELD_QUADRATIC_SCALE * rmsValue * rmsValue;
}

#endif

#endif
