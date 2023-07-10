/*
 * Rad Pro
 * FS2011 battery interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "gd32f1x0_adc.h"

#include "../../battery.h"
#include "../../buzzer.h"
#include "../../display.h"
#include "../../events.h"
#include "../../gm.h"
#include "../../power.h"
#include "../../settings.h"
#include "fs2011.h"

// ADC constants
#define ADC_BATTERY_CHANNEL 4

#define ADC_VDD 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)
#define ADC_BATTERY_NUM 2

#define ADC_BATTERY_VALUE_TO_BATTERY_VOLTAGE (ADC_VDD / ADC_VALUE_MAX / ADC_BATTERY_NUM)

// First order filter (N: time constant in taps): k = e^(-1 / N)
// For N = 100 (seconds):
#define BATTERY_FILTER_CONSTANT 0.99F

#define BATTERY_LOW_VOLTAGE_THRESHOLD 1.0F
#define BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD 0.5F

static struct
{
    uint32_t value;
    float voltage;
} battery;

static uint32_t readBatteryValue(void);

void initBatteryHardware(void)
{
    // GPIO
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(PWR_CHRG_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, PWR_CHRG_PIN);
    gpio_mode_setup(PWR_BAT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, PWR_BAT_PIN);

    // ADC
    rcc_periph_clock_enable(RCC_ADC1);

    if (!isGD32())
    {
        uint8_t adcChannel = ADC_BATTERY_CHANNEL;

        adc_set_regular_sequence(ADC1, 1, &adcChannel);
        adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_007DOT5);

        adc_calibrate(ADC1);
    }
    else
    {
        rcc_osc_on(RCC_HSI14);
        rcc_wait_for_osc_ready(RCC_HSI14);

        adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);
        adc_regular_channel_config(0, ADC_BATTERY_CHANNEL, ADC_SAMPLETIME_7POINT5);
        adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE);
        adc_external_trigger_config(ADC_REGULAR_CHANNEL, ENABLE);

        adc_enable();
        sleep(2);

        adc_calibration_enable();
    }

    battery.voltage = ADC_BATTERY_VALUE_TO_BATTERY_VOLTAGE * readBatteryValue();
}

void updateBattery(void)
{
    float currentVoltage = ADC_BATTERY_VALUE_TO_BATTERY_VOLTAGE * readBatteryValue();

    battery.voltage = BATTERY_FILTER_CONSTANT * battery.voltage +
                              (1.0F - BATTERY_FILTER_CONSTANT) * currentVoltage;

    if (isLowBattery())
        powerOffLowBattery();
}

static uint32_t readBatteryValue(void)
{
    syncTimerThread();

    for (uint32_t i = 0; i < 16; i++)
    {
        if (getBacklight() || getBuzzer())
            sleep(1);
        else
            break;
    }

    if (!isGD32())
    {
        adc_power_on(ADC1);

        syncGMHVPulse();

        adc_start_conversion_regular(ADC1);
        while (!adc_eos(ADC1))
            ;
        battery.value = adc_read_regular(ADC1);

        adc_power_off(ADC1);
    }
    else
    {
        syncGMHVPulse();

        adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
        while (!adc_flag_get(ADC_FLAG_EOC))
            ;
        adc_flag_clear(ADC_FLAG_EOC);
        battery.value = adc_regular_data_read();
    }

    return battery.value;
}

uint32_t getBatteryValue(void)
{
    return battery.value;
}

float getBatteryVoltage(void)
{
    return battery.voltage;
}

bool isBatteryCharging(void)
{
    if (!gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN))
        return true;

    if (battery.voltage < BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD)
        return true;

    return false;
}

bool isLowBattery(void)
{
    return (battery.voltage >= BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD) &&
           (battery.voltage < BATTERY_LOW_VOLTAGE_THRESHOLD);
}

#endif
