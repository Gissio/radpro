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

// MCU temperature

// First order filter (N: time constant in taps): k = exp(-1 / n)
// For n = 30 (seconds):
#define MCU_TEMPERATURE_FILTER_CONSTANT 0.967216F

static struct
{
    float filteredBatteryVoltageValue;
    float filteredMCUTemperatureValue;
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

    adc_start_calibration(ADC1);
    sleep(1);

    adc_disable(ADC1);
    sleep(1);
#elif defined(STM32F0)
    adc_start_calibration(ADC1);
    sleep(1);
#elif defined(STM32G0)
    adc_enable_vreg(ADC1);
    sleep(1);

    adc_start_calibration(ADC1);
    sleep(1);

    adc_disable_vreg(ADC1);
#endif

    updateADC();
}

static uint32_t readADC(uint8_t channel)
{
    syncTimerThread();

#if defined(STM32G0)
    adc_enable_vreg(ADC1);
    sleep(1);
#endif

    adc_enable(ADC1);
    sleep(1);

    adc_start_conversion_oneshot(ADC1, channel, 0x7);
    sleep(1);

    uint32_t value = adc_get_conversion_oneshot(ADC1);

    adc_disable(ADC1);
    sleep(1);

#if defined(STM32G0)
    adc_disable_vreg(ADC1);
#endif

    return value;
}

static float readBatteryVoltageValue(void)
{
    return readADC(PWR_BAT_CHANNEL);
}

static float readMCUTemperatureValue(void)
{
    adc_enable_temperature_sensor(ADC1);

    float value = readADC(ADC_TEMP_CHANNEL);

    adc_disable_temperature_sensor(ADC1);

    return value;
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

    value = readMCUTemperatureValue();
    adc.filteredMCUTemperatureValue =
        (adc.filteredMCUTemperatureValue == 0.0F)
            ? value
            : value + MCU_TEMPERATURE_FILTER_CONSTANT *
                          (adc.filteredMCUTemperatureValue - value);
}

float getDeviceBatteryVoltage(void)
{
    return (ADC_VALUE_TO_VOLTAGE * PWR_BAT_SCALE_FACTOR) *
           adc.filteredBatteryVoltageValue;
}

float getDeviceTemperature(void)
{
    float value = adc.filteredMCUTemperatureValue;

#if defined(STM32F0) && defined(GD32)
    return 25.0F + (1.45F - value * ADC_VALUE_TO_VOLTAGE) / 0.0041F;
#elif defined(STM32F0)
    float valueLow = TS_CAL1;
    float valueHigh = TS_CAL2;
    float valueSlope = (valueHigh - valueLow) / (110.0F - 30.0F);
    return 30.0F + (value - valueLow) / valueSlope;
#elif defined(STM32F1) && defined(GD32)
    return 25.0F + (1.45F - value * ADC_VALUE_TO_VOLTAGE) / 0.0041F;
#elif defined(STM32F1) && defined(CH32)
    return 25.0F + (1.34F - value * ADC_VALUE_TO_VOLTAGE) / 0.0043F;
#elif defined(STM32F1) && defined(APM32)
    return 25.0F + (1.41F - value * ADC_VALUE_TO_VOLTAGE) / 0.0042F;
#elif defined(STM32F1)
    return 25.0F + (1.43F - value * ADC_VALUE_TO_VOLTAGE) / 0.0043F;
#elif defined(STM32G0)
    float valueLow = TS_CAL1;
    return 30.0F + (value - valueLow) * ADC_VALUE_TO_VOLTAGE / 0.0025F;
#endif
}

#endif
