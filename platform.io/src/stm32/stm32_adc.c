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

#define ADC_VDD 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)

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
}

static uint32_t readADC(uint8_t channel)
{
    return adc_convert_oneshot(ADC1, channel, 0x7);
}

float readBatteryVoltage(void)
{
    adc_enable(ADC1);

#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    uint32_t batteryValue = readADC(PWR_BAT_CHANNEL);

    adc_enable_vref_channel(ADC1);
    uint32_t vrefValue = readADC(ADC_VREF_CHANNEL);
    adc_disable_vref_channel(ADC1);

    float value = (VREFINT_CAL_VOLTAGE * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX) *
                  VREFINT_CAL_VALUE *
                  batteryValue /
                  vrefValue;
#else
    uint32_t batteryValue = readADC(PWR_BAT_CHANNEL);

    adc_enable_temperature_vref_channel(ADC1);
    uint32_t vrefValue = readADC(ADC_VREF_CHANNEL);
    adc_disable_temperature_vref_channel(ADC1);

    float value = (VREFINT_VOLTAGE * PWR_BAT_SCALE_FACTOR) *
                  batteryValue /
                  vrefValue;
#endif

    adc_disable(ADC1);

    return value;
}

#endif
