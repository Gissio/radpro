/*
 * Rad Pro
 * STM32 power
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../adc.h"
#include "../power.h"

#include "device.h"

#define BATTERY_LOW_VOLTAGE_THRESHOLD 1.0F
#define BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD 0.5F

void initPowerController(void)
{
#if defined(STM32F0) || defined(STM32G0)
    gpio_setup_output(PWR_EN_PORT,
                      PWR_EN_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_NONE);

    gpio_setup_analog(PWR_BAT_PORT,
                      PWR_BAT_PIN,
                      GPIO_PULL_NONE);

    gpio_setup_input(PWR_CHRG_PORT,
                     PWR_CHRG_PIN,
#if defined(PWR_CHRG_PULLUP)
                     GPIO_PULL_UP
#else
                     GPIO_PULL_NONE
#endif
    );
#elif defined(STM32F1)
    gpio_setup(PWR_EN_PORT,
               PWR_EN_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);

    gpio_setup(PWR_BAT_PORT,
               PWR_BAT_PIN,
               GPIO_MODE_INPUT_ANALOG);

    gpio_setup(PWR_CHRG_PORT,
               PWR_EN_PIN,
#if defined(PWR_CHRG_PULLUP)
               GPIO_MODE_INPUT_PULLUP
#else
               GPIO_MODE_INPUT_FLOATING
#endif
    );
#endif
}

void setPower(bool value)
{
    gpio_modify(PWR_EN_PORT,
                PWR_EN_PIN,
#if defined(PWR_EN_ACTIVE_LOW)
                !value
#else
                value
#endif
    );
}

bool isBatteryCharging(void)
{
    if (
#if defined(PWR_CHRG_ACTIVE_LOW)
        !
#endif
        gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN))
        return true;

    if (getDeviceBatteryVoltage() < BATTERY_EXTERNAL_POWER_SUPPLY_VOLTAGE_THRESHOLD)
        return true;

    return false;
}

#endif
