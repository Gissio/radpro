/*
 * Rad Pro
 * FS600 keyboard interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS600

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../../keyboard.h"
#include "fs600.h"

void initKeyboardKeyDown(void)
{
    // GPIO
    gpio_mode_setup(KEY_UP_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    KEY_UP_PIN);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    KEY_DOWN_PIN | KEY_ENTER_PIN | KEY_BACK_PIN | KEY_POWER_PIN);
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
