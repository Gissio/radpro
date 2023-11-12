/*
 * Rad Pro
 * STM32 ADC
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/gpio.h>

#include "../adc.h"
#include "../power.h"

#include "stm32.h"

#define BATTERY_LOW_VOLTAGE_THRESHOLD 1.0F
#define BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD 0.5F

void initPowerHardware(void)
{
    // GPIO

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(PWR_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWR_EN_PIN);
    gpio_mode_setup(PWR_BAT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, PWR_BAT_PIN);
    gpio_mode_setup(PWR_CHRG_PORT, GPIO_MODE_INPUT, PWR_CHRG_PULL, PWR_CHRG_PIN);

#elif defined(STM32F1)

    gpio_set_mode(PWR_EN_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, PWR_EN_PIN);

    gpio_set_mode(PWR_BAT_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, PWR_BAT_PIN);

    gpio_set_mode(PWR_CHRG_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, PWR_CHRG_PIN);
    gpio_set(PWR_CHRG_PORT, PWR_CHRG_PIN); // Pull-up

#endif
}

void setPower(bool value)
{
    if (value)
        gpio_set(PWR_EN_PORT, PWR_EN_PIN);
    else
        gpio_clear(PWR_EN_PORT, PWR_EN_PIN);
}

bool isBatteryCharging(void)
{
#if defined(PWR_CHRG_ACTIVE_LOW)

    if (!gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN))
        return true;

#else

    if (gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN))
        return true;

#endif

// +++ TEST
    if (getBatteryVoltage() < BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD)
        return true;
// +++ TEST

    return false;
}

bool isLowBattery(void)
{
    // +++ TEST
    // return false;
    // +++ TEST

    float batteryVoltage = getBatteryVoltage();

    return (batteryVoltage >= BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD) &&
           (batteryVoltage < BATTERY_LOW_VOLTAGE_THRESHOLD);
}

#endif
