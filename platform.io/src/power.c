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

#include "gd32f1x0_adc.h"

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

struct
{
    float batteryValue;
} power;

static uint32_t readBatteryValue(void)
{
    uint32_t value;

#ifndef SDL_MODE
    if (isGD32())
    {
        adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
        while (!adc_flag_get(ADC_FLAG_EOC))
            ;
        adc_flag_clear(ADC_FLAG_EOC);
        value = adc_regular_data_read();
    }
    else
    {
        adc_power_on(ADC1);

        adc_start_conversion_regular(ADC1);
        while (!adc_eoc(ADC1))
            waitSysTicks(1);
        value = adc_read_regular(ADC1);

        adc_power_off(ADC1);
    }
#else
    value = (uint32_t)(ADC_FACTOR * 1.27F);
#endif

    return value;
}

void initPower(void)
{
#ifndef SDL_MODE
    // GPIO
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(PWR_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWR_EN_PIN);
    gpio_mode_setup(PWR_CHRG_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, PWR_CHRG_PIN);
    gpio_mode_setup(PWR_BAT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, PWR_BAT_PIN);

    setPower(true);

    // ADC
    rcc_periph_clock_enable(RCC_ADC1);

    uint8_t adcChannel = 4;
    if (isGD32())
    {
        adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);
        adc_regular_channel_config(0, adcChannel, ADC_SAMPLETIME_55POINT5);

        adc_enable();
        waitSysTicks(2);

        adc_calibration_enable();
    }
    else
    {
        adc_set_regular_sequence(ADC1, 1, (uint8_t *)&adcChannel);
        adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_055DOT5);

        adc_calibrate(ADC1);
    }
#endif

    power.batteryValue = (float)readBatteryValue();
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

void updateBattery(void)
{
    power.batteryValue = (BATTERY_FILTER_CONSTANT * power.batteryValue +
                          (1.0F - BATTERY_FILTER_CONSTANT) * readBatteryValue());
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
