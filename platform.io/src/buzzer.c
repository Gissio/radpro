/*
 * FS2011 Pro
 * Sound
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "main.h"

void initBuzzer(void)
{
#ifndef SDL_MODE
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(BUZZ_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BUZZ_PIN);
#endif
}

void setBuzzer(bool value)
{
#ifndef SDL_MODE
    if (value)
        gpio_set(BUZZ_PORT, BUZZ_PIN);
    else
        gpio_clear(BUZZ_PORT, BUZZ_PIN);
#endif
}
