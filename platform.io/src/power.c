/*
 * FS2011 Pro
 * Power management
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>

#include "events.h"
#include "main.h"
#include "power.h"
#include "settings.h"

#define BATTERY_NUM 2

#define BATTERY_NI_MH_VOLTAGE_MAX 1.3F
#define BATTERY_NI_MH_VOLTAGE_MIN 1.2F
#define BATTERY_ALKALINE_VOLTAGE_MAX 1.4F
#define BATTERY_ALKALINE_VOLTAGE_MIN 1.1F

#define ADC_VOLTAGE_MAX 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)
#define ADC_FACTOR (BATTERY_NUM * ADC_VALUE_MAX / ADC_VOLTAGE_MAX)

#define BATTERY_NI_MH_VALUE_MIN (ADC_FACTOR * BATTERY_NI_MH_VOLTAGE_MIN)
#define BATTERY_NI_MH_VALUE_RANGE (ADC_FACTOR * (BATTERY_NI_MH_VOLTAGE_MAX - BATTERY_NI_MH_VOLTAGE_MIN))
#define BATTERY_ALKALINE_VALUE_MIN (ADC_FACTOR * BATTERY_ALKALINE_VOLTAGE_MIN)
#define BATTERY_ALKALINE_VALUE_RANGE (ADC_FACTOR * (BATTERY_ALKALINE_VOLTAGE_MAX - BATTERY_ALKALINE_VOLTAGE_MIN))

// First order filter (N: time constant in taps): k = e^(-1 / N)
// For N = 60 (seconds):
#define BATTERY_FILTER_CONSTANT 0.98347F

const uint8_t powerADCChannels = {4};

struct Power
{
    float batteryValue;
} power;

void initPower(void)
{
#ifndef SDL_MODE
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(PWR_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWR_EN_PIN);
    gpio_mode_setup(PWR_CHRG_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, PWR_CHRG_PIN);
    gpio_mode_setup(PWR_BAT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, PWR_BAT_PIN);

    setPower(true);

    rcc_periph_clock_enable(RCC_ADC1);

    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_055DOT5);
    adc_set_regular_sequence(ADC1, 1, (uint8_t *)&powerADCChannels);

    adc_calibrate_async(ADC1);
    uint32_t calibrationTimeout = 100;
    while (adc_is_calibrating(ADC1) && (calibrationTimeout > 0))
    {
        waitSysTicks(1);
        calibrationTimeout--;
    }

    // +++
    adc_power_on(ADC1);
    // ---

    waitSysTicks(10);
#endif

    power.batteryValue = (float)getBatteryValue();
}

void setPower(bool value)
{
#ifndef SDL_MODE
    if (value)
        gpio_set(PWR_EN_PORT, PWR_EN_PIN);
    else
        gpio_clear(PWR_EN_PORT, PWR_EN_PIN);
#else
    printf("Set power: %d\n", value);
#endif
}

uint32_t getBatteryValue(void)
{
#ifndef SDL_MODE
    // +++
    // adc_power_on(ADC1);
    // ---
    adc_start_conversion_regular(ADC1);

    uint32_t timeout = 100;
    while (!adc_eoc(ADC1) && (timeout > 0))
    {
        waitSysTicks(1);
        timeout--;
    }

    uint32_t value = adc_read_regular(ADC1);
    // +++
    // adc_power_off(ADC1);
    // ---

    return value;

#else
    return (uint32_t)(ADC_FACTOR * 1.27F);
#endif
}

void updateBattery(void)
{
    power.batteryValue = (BATTERY_FILTER_CONSTANT * power.batteryValue +
                          (1.0F - BATTERY_FILTER_CONSTANT) * getBatteryValue());
}

uint8_t getBatteryLevel(void)
{
#ifndef SDL_MODE
    if (!gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN))
        return BATTERY_LEVEL_CHARGING;
#endif

    int32_t value;
    switch (settings.batteryType)
    {
    case BATTERY_NI_MH:
        value = (int32_t)(BATTERY_LEVEL_MAX * (power.batteryValue - BATTERY_NI_MH_VALUE_MIN) /
                          BATTERY_NI_MH_VALUE_RANGE);
        break;

    case BATTERY_ALKALINE:
        value = (int32_t)(BATTERY_LEVEL_MAX * (power.batteryValue - BATTERY_ALKALINE_VALUE_MIN) /
                          BATTERY_ALKALINE_VALUE_RANGE);
        break;

    default:
        value = 0;
        break;
    }

    return (value < 0) ? 0 : ((value > BATTERY_LEVEL_MAX) ? BATTERY_LEVEL_MAX : value);
}
