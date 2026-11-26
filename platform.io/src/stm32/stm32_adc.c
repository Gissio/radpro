/*
 * Rad Pro
 * STM32 ADC
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/adc.h"
#include "../stm32/device.h"

#define ADC_VALUE_MAX ((1 << 12) - 1)

// 55.5 cycles (F0/F1), 39.5 cycles (G0), 92.5 cycles (L4)
#define ADC_SAMPLETIME_DEFAULT 5
// VREFINT requires >= 17.1 us sampling time on F1-class MCUs:
// 239.5 cycles (F0/F1), 160.5 cycles (G0), 640.5 cycles (L4)
#define ADC_SAMPLETIME_VREF 7

#define PWR_BAT_SAMPLE_NUM 16
#define VREF_SAMPLE_NUM 16

#if defined(PWR_CHRG_CHANNEL)
#define PWR_CHRG_SAMPLE_NUM 16
#define PWR_CHRG_USB_CHARGING_THRESHOLD 500
#define PWR_CHRG_USB_POWERED_THRESHOLD 2500
#endif

#if defined(EMFMETER)
// The ADC tick index counts down once per cycle. Each stage begins at
// its tick, which selects the channel and starts the first conversion.
// The value read at tick n was converted at tick n + 1.
#define ADC_END_SAMPLE 1
#define ADC_PWR_BAT_SAMPLE (ADC_END_SAMPLE + PWR_BAT_SAMPLE_NUM)
#define ADC_VREF_SAMPLE (ADC_PWR_BAT_SAMPLE + VREF_SAMPLE_NUM)
#define ADC_MAGNETIC_FIELD_SAMPLE (ADC_VREF_SAMPLE + FIELD_SAMPLE_NUM)
#define ADC_ELECTRIC_FIELD_SAMPLE (ADC_MAGNETIC_FIELD_SAMPLE + FIELD_SAMPLE_NUM)
#define ADC_START_SAMPLE ADC_ELECTRIC_FIELD_SAMPLE
#endif

typedef struct
{
    uint32_t battery;
    uint32_t vref;
#if defined(EMFMETER)
    // Electric field samples, followed by magnetic field samples
    uint16_t field[2 * FIELD_SAMPLE_NUM];
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

    // Double buffer: the ADC tick fills buffers[snapshotIndex ^ 1] while
    // readers use buffers[snapshotIndex]
    ADCValues buffers[2];
    volatile uint32_t snapshotIndex;
#else
    ADCValues snapshot;
#endif
} adcHardware;

static ADCValues *getSnapshot(void)
{
#if defined(EMFMETER)
    return &adcHardware.buffers[adcHardware.snapshotIndex];
#else
    return &adcHardware.snapshot;
#endif
}

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

static void readPowerState(void)
{
    ADCValues *snapshot = getSnapshot();

    adc_enable(ADC1);

    // Sample VREF
    adc_enable_vref_channel(ADC1);
    adc_enable_temperature_vref_channel(ADC1);
    adc_set_sampletime(ADC1, ADC_SAMPLETIME_VREF);
    adc_set_channel(ADC1, ADC_VREF_CHANNEL);
    snapshot->vref = convertADC(VREF_SAMPLE_NUM);
#if !defined(EMFMETER)
    adc_disable_vref_channel(ADC1);
    adc_disable_temperature_vref_channel(ADC1);
#endif

    // Sample battery
    adc_set_sampletime(ADC1, ADC_SAMPLETIME_DEFAULT);
    adc_set_channel(ADC1, PWR_BAT_CHANNEL);
    snapshot->battery = convertADC(PWR_BAT_SAMPLE_NUM);

    // Sample PWR_CHRG
#if defined(PWR_CHRG_CHANNEL)
    adc_set_channel(ADC1, PWR_CHRG_CHANNEL);
    uint32_t pwrChrgValue = convertADC(PWR_CHRG_SAMPLE_NUM) / PWR_CHRG_SAMPLE_NUM;

    adcHardware.batteryCharging = (pwrChrgValue < PWR_CHRG_USB_CHARGING_THRESHOLD);
    adcHardware.usbPowered = adcHardware.batteryCharging || (pwrChrgValue > PWR_CHRG_USB_POWERED_THRESHOLD);
#endif

    // With the EMF meter, the ADC and VREF channel stay enabled for the
    // ADC tick
#if !defined(EMFMETER)
    adc_disable(ADC1);
#endif
}

#if defined(EMFMETER)

void onADCTick(uint32_t index)
{
    if (!adcHardware.enabled)
    {
        // Enable on the start sample, so that a full cycle is acquired
        if ((index != ADC_START_SAMPLE) || !adcHardware.initialized)
            return;

        adcHardware.enabled = true;
    }

    if (index > ADC_START_SAMPLE)
        return;

    ADCValues *live = &adcHardware.buffers[adcHardware.snapshotIndex ^ 1];

    // Store value (the value read at the start sample is stale)
    uint32_t value = adc_read(ADC1);
    if (index < ADC_PWR_BAT_SAMPLE)
        live->battery += value;
    else if (index < ADC_VREF_SAMPLE)
        live->vref += value;
    else if (index < ADC_START_SAMPLE)
        live->field[(ADC_START_SAMPLE - 1) - index] = value;

    // Setup stage
    switch (index)
    {
    case ADC_ELECTRIC_FIELD_SAMPLE:
        // Reset here rather than at the end sample, as readers may still
        // be using this buffer until the snapshot swap
        live->battery = 0;
        live->vref = 0;

        adc_set_channel(ADC1, ELECTRIC_FIELD_CHANNEL);

        break;

    case ADC_MAGNETIC_FIELD_SAMPLE:
        adc_set_channel(ADC1, MAGNETIC_FIELD_CHANNEL);

        break;

    case ADC_VREF_SAMPLE:
        adc_set_sampletime(ADC1, ADC_SAMPLETIME_VREF);
        adc_set_channel(ADC1, ADC_VREF_CHANNEL);

        break;

    case ADC_PWR_BAT_SAMPLE:
        adc_set_sampletime(ADC1, ADC_SAMPLETIME_DEFAULT);
        adc_set_channel(ADC1, PWR_BAT_CHANNEL);

        break;

    case ADC_END_SAMPLE:
        // Publish the live buffer
        adcHardware.snapshotIndex ^= 1;

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
    readPowerState();

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

float readBatteryVoltage(void)
{
#if !defined(EMFMETER)
    readPowerState();
#endif

#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    float value = (VREFINT_CAL_VOLTAGE / ADC_VALUE_MAX * PWR_BAT_SCALE_FACTOR * VREF_SAMPLE_NUM / PWR_BAT_SAMPLE_NUM);
    value *= (float)(uint32_t)VREFINT_CAL_VALUE;
#else
    float value = (VREFINT_VOLTAGE * PWR_BAT_SCALE_FACTOR * VREF_SAMPLE_NUM / PWR_BAT_SAMPLE_NUM);
#endif
    const ADCValues *snapshot = getSnapshot();
    value *= (float)snapshot->battery;
    value /= (float)snapshot->vref;

    return value;
}

#if defined(EMFMETER)

const FieldSensor electricFieldSensor = {
    ELECTRIC_FIELD_OFFSET,
    ELECTRIC_FIELD_LINEAR_SCALE,
    ELECTRIC_FIELD_QUADRATIC_SCALE,
};

const FieldSensor magneticFieldSensor = {
    MAGNETIC_FIELD_OFFSET,
    MAGNETIC_FIELD_LINEAR_SCALE,
    MAGNETIC_FIELD_QUADRATIC_SCALE,
};

const uint16_t *getElectricFieldSamples(void)
{
    return getSnapshot()->field;
}

const uint16_t *getMagneticFieldSamples(void)
{
    return getSnapshot()->field + FIELD_SAMPLE_NUM;
}

#endif

#endif
