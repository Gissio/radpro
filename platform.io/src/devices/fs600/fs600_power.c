/*
 * Rad Pro
 * FS600 power interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS600

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../../power.h"
#include "fs600.h"

void initPower(void)
{
    // GPIO
    gpio_mode_setup(PWR_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWR_EN_PIN);

    setPower(true);
}

void setPower(bool value)
{
    if (value)
        gpio_set(PWR_EN_PORT, PWR_EN_PIN);
    else
        gpio_clear(PWR_EN_PORT, PWR_EN_PIN);
}

#endif
