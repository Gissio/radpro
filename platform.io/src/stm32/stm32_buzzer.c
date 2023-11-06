/*
 * Rad Pro
 * STM32 buzzer
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/gpio.h>

#include "../buzzer.h"

#include "stm32.h"

static struct
{
    bool value;
} buzzer;

void initBuzzerHardware(void)
{
    // GPIO

#if defined(BUZZ_ACTIVE_LOW)

    setBuzzer(false);

#endif

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(BUZZ_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BUZZ_PIN);

#if defined(BUZZ_PORT2)

    gpio_mode_setup(BUZZ_PORT2, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BUZZ_PIN2);

#endif

#elif defined(STM32F1)

    gpio_set_mode(BUZZ_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, BUZZ_PIN);

#endif
}

void setBuzzer(bool value)
{
    buzzer.value = value;

#if defined(BUZZ_ACTIVE_LOW)

    if (value)
    {
        gpio_clear(BUZZ_PORT, BUZZ_PIN);

#if defined(BUZZ_PORT2)

        gpio_clear(BUZZ_PORT2, BUZZ_PIN2);

#endif
    }
    else
    {
        gpio_set(BUZZ_PORT, BUZZ_PIN);

#if defined(BUZZ_PORT2)

        gpio_set(BUZZ_PORT2, BUZZ_PIN2);

#endif
    }

#else

    if (value)
        gpio_set(BUZZ_PORT, BUZZ_PIN);
    else
        gpio_clear(BUZZ_PORT, BUZZ_PIN);

#endif
}

bool getBuzzer(void)
{
    return buzzer.value;
}

#endif
