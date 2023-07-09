/*
 * Rad Pro
 * FS2011 keyboard interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../../keyboard.h"
#include "fs2011.h"

void initKeyboardKeyDown(void)
{
    // GPIO
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    KEY_UP_PIN);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    KEY_POWER_PIN | KEY_ENTER_PIN | KEY_BACK_PIN | KEY_DOWN_PIN);
}

void getKeyboardKeyDown(bool *isKeyDown)
{
    isKeyDown[KEY_POWER] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_ENTER] = !gpio_get(KEY_ENTER_PORT, KEY_ENTER_PIN);
    isKeyDown[KEY_BACK] = !gpio_get(KEY_BACK_PORT, KEY_BACK_PIN);
}

#endif
