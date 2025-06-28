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

float readBatteryVoltage(void)
{
    if (!adc.initialized)
        return 0.0F;

    startADC();

    float value;

#if (defined(STM32F0) && !defined(GD32)) || defined(STM32G0) || defined(STM32L4)
    value = (VREFINT_CAL_VOLTAGE * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX) *
            VREFINT_CAL_VALUE *
            readADC(PWR_BAT_CHANNEL) /
            readADC(ADC_VREF_CHANNEL);
#else
    value = ((ADC_VDD * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX)) *
            readADC(PWR_BAT_CHANNEL);
    // value = (VREFINT_VOLTAGE * PWR_BAT_SCALE_FACTOR) *
    //         readADC(PWR_BAT_CHANNEL) /
    //         readADC(ADC_VREF_CHANNEL);
#endif

    stopADC();

    return value;
}

#endif
