/*
 * Rad Pro
 * Battery management
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef SDL_MODE

#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "gd32f1x0_adc.h"

#endif

#include "battery.h"
#include "buzzer.h"
#include "display.h"
#include "events.h"
#include "fs2011.h"
#include "gm.h"
#include "power.h"
#include "settings.h"

// ADC constants
#define ADC_VDD 3.3F
#define ADC_VALUE_MAX ((1 << 12) - 1)
#define ADC_BATTERY_NUM 2

#define ADC_BATTERY_VALUE_TO_BATTERY_VOLTAGE (ADC_VDD / ADC_VALUE_MAX / ADC_BATTERY_NUM)

#define ADC_BATTERY_CHANNEL 4

// First order filter (N: time constant in taps): k = e^(-1 / N)
// For N = 100 (seconds):
#define BATTERY_FILTER_CONSTANT 0.99F

#define BATTERY_LOW_VOLTAGE_THRESHOLD 1.0F
#define BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD 0.5F

struct
{
    uint32_t value;
    float filteredVoltage;
} battery;

static const float batteryLevelVoltageThresholds[2][9] = {
    {1.159F, 1.198F, 1.223F, 1.244F, 1.257F, 1.268F, 1.277F, 1.297F, 1.333F},
    {1.017F, 1.149F, 1.245F, 1.297F, 1.326F, 1.351F, 1.388F, 1.429F, 1.470F},
};

static float readBatteryVoltage(void);

void initBattery(void)
{
#ifndef SDL_MODE
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
        adc_channel_length_config(ADC_REGULAR_CHANNEL, 1);
        adc_regular_channel_config(0, ADC_BATTERY_CHANNEL, ADC_SAMPLETIME_7POINT5);

        adc_enable();
        sleep(2);

        adc_calibration_enable();
    }
#endif

    battery.filteredVoltage = readBatteryVoltage();
}

static float readBatteryVoltage(void)
{
#ifndef SDL_MODE
    syncThreads();

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

        // Sync to GM pulse
        syncGMHVPulse();

        adc_start_conversion_regular(ADC1);
        while (!adc_eos(ADC1))
            ;
        battery.value = adc_read_regular(ADC1);

        adc_power_off(ADC1);
    }
    else
    {
        // Sync to GM pulse
        syncGMHVPulse();

        adc_software_trigger_enable(ADC_REGULAR_CHANNEL);
        while (!adc_flag_get(ADC_FLAG_EOC))
            ;
        adc_flag_clear(ADC_FLAG_EOC);
        battery.value = adc_regular_data_read();
    }

    return ADC_BATTERY_VALUE_TO_BATTERY_VOLTAGE * battery.value;

#else
    return 1.27F;
#endif
}

void updateBattery(void)
{
    battery.filteredVoltage = BATTERY_FILTER_CONSTANT * battery.filteredVoltage +
                              (1.0F - BATTERY_FILTER_CONSTANT) * readBatteryVoltage();

    if (isLowBattery())
        powerOffLowBattery();
}

uint8_t getBatteryLevel(void)
{
#ifndef SDL_MODE
    if (!gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN))
        return BATTERY_LEVEL_CHARGING;
#endif

    if (battery.filteredVoltage < BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD)
        return BATTERY_LEVEL_CHARGING;

    const float *batteryLevelThreshold = batteryLevelVoltageThresholds[settings.batteryType];
    uint8_t level = BATTERY_LEVEL_MAX;
    for (uint8_t i = 0; i < BATTERY_LEVEL_MAX; i++)
    {
        if (battery.filteredVoltage < batteryLevelThreshold[i])
        {
            level = i;
            break;
        }
    }

    return level;
}

bool isLowBattery(void)
{
    return (battery.filteredVoltage >= BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD) &&
           (battery.filteredVoltage < BATTERY_LOW_VOLTAGE_THRESHOLD);
}
