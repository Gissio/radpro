/*
 * Rad Pro
 * STM32 ADC
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../adc.h"

#include "device.h"

// ADC constants

#define ADC_VDD 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)

// Battery voltage

// First order filter (n: time constant in taps): k = exp(-1 / n)
// For n = 100 (seconds):
#define BATTERY_VOLTAGE_FILTER_CONSTANT 0.99F

static struct
{
    bool initialized;
    float batteryVoltage;
    float filteredBatteryVoltage;
} adc;

void initADC(void)
{
    rcc_enable_adc(ADC1);

#if defined(STM32G0)
    adc_enable_vreg(ADC1);
#elif defined(STM32L4)
    adc_disable_deep_power_down(ADC1);
    adc_enable_vreg(ADC1);
#endif

    adc_calibrate(ADC1);

    adc.initialized = true;

    updateADC();
}

static void startADC(void)
{
#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    adc_enable_vref_channel(ADC1);
#elif (defined(STM32F0) && defined(GD32)) || defined(STM32F1)
    adc_enable_temperature_vref_channel(ADC1);
#endif
    adc_enable(ADC1);
}

static uint32_t readADC(uint8_t channel)
{
    return adc_convert_oneshot(ADC1, channel, 0x7);
}

static void stopADC(void)
{
    adc_disable(ADC1);
#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    adc_disable_vref_channel(ADC1);
#elif (defined(STM32F0) && defined(GD32)) || defined(STM32F1)
    adc_disable_temperature_vref_channel(ADC1);
#endif
}

static float readBatteryVoltage(void)
{
    float value;

    startADC();

#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    value = (VREFINT_CAL_VOLTAGE * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX) *
            VREFINT_CAL_VALUE *
            readADC(PWR_BAT_CHANNEL) /
            readADC(ADC_VREF_CHANNEL);
#else
    value = (VREFINT_VOLTAGE * PWR_BAT_SCALE_FACTOR) *
            readADC(PWR_BAT_CHANNEL) /
            readADC(ADC_VREF_CHANNEL);
    // value = ((ADC_VDD * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX)) *
    //         readADC(PWR_BAT_CHANNEL);
#endif

    stopADC();

    return value;
}

void updateADC(void)
{
    if (!adc.initialized)
        return;

    adc.batteryVoltage = readBatteryVoltage();

    if (adc.filteredBatteryVoltage == 0.0F)
        adc.filteredBatteryVoltage = adc.batteryVoltage;
    else
        adc.filteredBatteryVoltage = adc.batteryVoltage + BATTERY_VOLTAGE_FILTER_CONSTANT * (adc.filteredBatteryVoltage - adc.batteryVoltage);
}

float getBatteryVoltage(void)
{
    return PWR_BAT_NUM * adc.batteryVoltage;
}

float getFilteredBatteryVoltage(void)
{
    return adc.filteredBatteryVoltage;
}

#endif
