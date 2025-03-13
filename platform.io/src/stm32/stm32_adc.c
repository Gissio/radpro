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
    bool enabled;
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

    adc.enabled = true;

    updateADC();
}

static void startADC(void)
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
}

static void stopADC(void)
{
    adc_disable(ADC1);
    sleep(1);

#if defined(STM32F0) && !defined(GD32)
    adc_disable_vref_channel(ADC1);
#elif defined(STM32G0) || defined(STM32L4)
    adc_disable_vref_channel(ADC1);
    adc_disable_vreg(ADC1);
#endif
}

static uint32_t readADC(uint8_t channel)
{
    adc_start_conversion_oneshot(ADC1, channel, 0x7);
    sleep(1);

    return adc_get_conversion_oneshot(ADC1);
}

static float readBatteryVoltage(void)
{
    float value;

    startADC();

#if defined(STM32F0) && !defined(GD32) || defined(STM32G0) || defined(STM32L4)
    // +++ TEST
    // value = (VREFINT_CAL_VOLTAGE * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX) *
    //         VREFINT_CAL_VALUE *
    //         readADC(PWR_BAT_CHANNEL) /
    //         readADC(ADC_VREF_CHANNEL);
    value = (ADC_VDD * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX) *
            readADC(PWR_BAT_CHANNEL);
    // +++ TEST
#else
    value = ((ADC_VDD * PWR_BAT_SCALE_FACTOR / ADC_VALUE_MAX)) *
            readADC(PWR_BAT_CHANNEL);
#endif

    stopADC();

    return value;
}

void updateADC(void)
{
    if (!adc.enabled)
        return;

    float value;

    value = readBatteryVoltage();
    // +++ TEST
    // adc.filteredBatteryVoltage =
    //     (adc.filteredBatteryVoltage == 0.0F)
    //         ? value
    //         : value + BATTERY_VOLTAGE_FILTER_CONSTANT *
    //                       (adc.filteredBatteryVoltage - value);
    adc.filteredBatteryVoltage = value;
    // +++ TEST
}

float getDeviceBatteryVoltage(void)
{
    return adc.filteredBatteryVoltage;
}

#endif
