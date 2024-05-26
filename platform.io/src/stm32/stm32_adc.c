/*
 * Rad Pro
 * STM32 ADC
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../adc.h"
#include "../buzzer.h"
#include "../display.h"
#include "../events.h"
#include "../system.h"
#include "../tube.h"

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
    float filteredBatteryVoltage;
} adc;

void initADC(void)
{
    syncTimerThread();

    rcc_enable_adc(ADC1);

#if defined(STM32L4)
    adc_disable_deep_power_down(ADC1);
#endif

#if (defined(STM32F0) && defined(GD32)) || defined(STM32F1)
    adc_enable(ADC1);
    sleep(1);

    adc_reset_calibration(ADC1);
    sleep(1);
#elif defined(STM32G0) || defined(STM32L4)
    adc_enable_vreg(ADC1);
    sleep(1);
#endif

    adc_start_calibration(ADC1);
    sleep(1);

#if (defined(STM32F0) && defined(GD32)) || defined(STM32F1)
    adc_disable(ADC1);
    sleep(1);
#elif defined(STM32G0) || defined(STM32L4)
    adc_disable_vreg(ADC1);
#endif

    updateADC();
}

static uint32_t readADC(uint8_t channel)
{
    syncTimerThread();

#if (defined(STM32F0) && !defined(GD32))
    adc_enable_vref_channel(ADC1);
    sleep(1);
#elif defined(STM32G0) || defined(STM32L4)
    adc_enable_vreg(ADC1);
    adc_enable_vref_channel(ADC1);
    sleep(1);
#endif

    adc_enable(ADC1);
    sleep(1);

    adc_start_conversion_oneshot(ADC1, channel, 0x7);
    sleep(1);

    uint32_t value = adc_get_conversion_oneshot(ADC1);

    adc_disable(ADC1);
    sleep(1);

#if defined(STM32F0) && !defined(GD32)
    adc_disable_vref_channel(ADC1);
#elif defined(STM32G0) || defined(STM32L4)
    adc_disable_vref_channel(ADC1);
    adc_disable_vreg(ADC1);
#endif

    return value;
}

static float readBatteryVoltage(void)
{
#if defined(STM32F0) && !defined(GD32) || defined(STM32G0) || defined(STM32L4)
    return (VREFINT_CAL_VOLTAGE * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX) *
           VREFINT_CAL_VALUE *
           readADC(PWR_BAT_CHANNEL) /
           readADC(ADC_VREF_CHANNEL);
#else
    return ((ADC_VDD * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX)) *
           readADC(PWR_BAT_CHANNEL);
#endif
}

void updateADC(void)
{
    float value;

    value = readBatteryVoltage();
    adc.filteredBatteryVoltage =
        (adc.filteredBatteryVoltage == 0.0F)
            ? value
            : value + BATTERY_VOLTAGE_FILTER_CONSTANT *
                          (adc.filteredBatteryVoltage - value);
}

float getDeviceBatteryVoltage(void)
{
    return adc.filteredBatteryVoltage;
}

#endif
