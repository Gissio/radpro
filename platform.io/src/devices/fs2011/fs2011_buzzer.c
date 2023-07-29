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

static struct
{
    bool value;
} buzzer;

void initBuzzer(void)
{
    // GPIO
#ifdef STM32F0
    gpio_mode_setup(BUZZ_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BUZZ_PIN);
#elif STM32F1
    gpio_set_mode(BUZZ_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, BUZZ_PIN);
#endif
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
