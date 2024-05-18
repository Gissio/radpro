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

#define ADC_VALUE_TO_VOLTAGE (ADC_VDD / ADC_VALUE_MAX)

// Battery voltage

// First order filter (n: time constant in taps): k = exp(-1 / n)
// For n = 100 (seconds):
#define BATTERY_VOLTAGE_FILTER_CONSTANT 0.99F

static struct
{
    float filteredBatteryVoltageValue;
} adc;

void initADC(void)
{
    syncTimerThread();

    rcc_enable_adc(ADC1);

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

#if defined(STM32G0) || defined(STM32L4)
    adc_enable_vreg(ADC1);
    sleep(1);
#endif

    if (channel == ADC_VBAT_CHANNEL)
        adc_enable_vbat_channel(ADC1);

    adc_enable(ADC1);
    sleep(1);

    adc_start_conversion_oneshot(ADC1, channel, 0x7);
    sleep(1);

    uint32_t value = adc_get_conversion_oneshot(ADC1);

    adc_disable(ADC1);
    sleep(1);

    if (channel == ADC_VBAT_CHANNEL)
        adc_disable_vbat_channel(ADC1);

#if defined(STM32G0) || defined(STM32L4)
    adc_disable_vreg(ADC1);
#endif

    return value;
}

static float readBatteryVoltageValue(void)
{
    return readADC(PWR_BAT_CHANNEL);
}

void updateADC(void)
{
    float value;

    value = readBatteryVoltageValue();
    adc.filteredBatteryVoltageValue =
        (adc.filteredBatteryVoltageValue == 0.0F)
            ? value
            : value + BATTERY_VOLTAGE_FILTER_CONSTANT *
                          (adc.filteredBatteryVoltageValue - value);
}

float getDeviceBatteryVoltage(void)
{
    return (ADC_VALUE_TO_VOLTAGE * PWR_BAT_SCALE_FACTOR) *
           adc.filteredBatteryVoltageValue;
}

#endif
