/*
 * Rad Pro
 * SDLSim vibrator
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(VIBRATOR)

#include <libopencm3/stm32/gpio.h>

#include "../vibrator.h"

#include "stm32.h"

void initVibrator(void)
{
#if defined(STM32F0) || defined(STM32G0)
    gpio_mode_setup(VIBRATOR_PORT,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    VIBRATOR_PIN);
#elif defined(STM32F1)
    gpio_set_mode(VIBRATOR_PORT,
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  VIBRATOR_PIN);
#endif
}

void setVibrator(bool value)
{
    if (value)
        gpio_set(VIBRATOR_PORT, VIBRATOR_PIN);
    else
        gpio_clear(VIBRATOR_PORT, VIBRATOR_PIN);
}

#endif
