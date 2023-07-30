/*
 * Rad Pro
 * FS600 buzzer interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS600

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../../buzzer.h"
#include "fs600.h"

static struct
{
    bool value;
} buzzer;

void initBuzzer(void)
{
    // GPIO
    gpio_mode_setup(BUZZ_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BUZZ_PIN);
}

void setBuzzer(bool value)
{
    buzzer.value = value;

    if (value)
        gpio_set(BUZZ_PORT, BUZZ_PIN);
    else
        gpio_clear(BUZZ_PORT, BUZZ_PIN);
}

bool getBuzzer(void)
{
    return buzzer.value;
}

#endif
