/*
 * Rad Pro
 * FS2011 buzzer interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../../buzzer.h"
#include "fs2011.h"

static struct {
    bool value;
} buzzer;

void initBuzzer(void)
{
    // GPIO
    rcc_periph_clock_enable(RCC_GPIOB);

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
