/*
 * Rad Pro
 * STM32 keyboard
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/gpio.h>

#include "../keyboard.h"

#include "stm32.h"

void initKeyboardHardware(void)
{
    // GPIO

#if defined(FS2011) && defined(STM32F0)

    gpio_mode_setup(KEY_UP_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY_UP_PIN);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    KEY_DOWN_PIN | KEY_ENTER_PIN | KEY_BACK_PIN | KEY_POWER_PIN);

#elif defined(FS2011) && defined(STM32F1)

    gpio_set_mode(KEY_UP_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, KEY_UP_PIN);
    gpio_set(KEY_UP_PORT, KEY_UP_PIN); // Pull-up

    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                  KEY_DOWN_PIN | KEY_ENTER_PIN | KEY_BACK_PIN | KEY_POWER_PIN);
    gpio_set(GPIOB,
             KEY_DOWN_PIN | KEY_ENTER_PIN | KEY_BACK_PIN | KEY_POWER_PIN); // Pull-up

#elif defined(FS600) || defined(FS1000)

    gpio_mode_setup(KEY_DOWN_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, KEY_DOWN_PIN);
    gpio_mode_setup(KEY_UP_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_UP_PIN);
    gpio_mode_setup(KEY_POWER_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_POWER_PIN);
    gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_ENTER_PIN | KEY_BACK_PIN);

#elif defined(GC01)

    gpio_set_mode(KEY_POWER_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                  KEY_POWER_PIN);
    gpio_clear(KEY_POWER_PORT, KEY_POWER_PIN); // Pull-down

    gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                  KEY_ENTER_PIN | KEY_BACK_PIN | KEY_UP_PIN | KEY_DOWN_PIN);
    gpio_clear(KEY_POWER_PORT, KEY_POWER_PIN); // Pull-down

#endif
}

void getKeyboardKeys(bool *isKeyDown)
{
#if defined(FS2011)

    isKeyDown[KEY_POWER] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
    isKeyDown[KEY_UP] = !gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = !gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_ENTER] = !gpio_get(KEY_ENTER_PORT, KEY_ENTER_PIN);
    isKeyDown[KEY_BACK] = !gpio_get(KEY_BACK_PORT, KEY_BACK_PIN);

#elif defined(FS600)

    isKeyDown[KEY_POWER] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
    isKeyDown[KEY_UP] = gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_ENTER] = gpio_get(KEY_ENTER_PORT, KEY_ENTER_PIN);
    isKeyDown[KEY_BACK] = gpio_get(KEY_BACK_PORT, KEY_BACK_PIN);

#elif defined(FS1000)

    isKeyDown[KEY_POWER] = !gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
    isKeyDown[KEY_BACK] = gpio_get(KEY_BACK_PORT, KEY_BACK_PIN);

#elif defined(GC01)

    isKeyDown[KEY_POWER] = gpio_get(KEY_POWER_PORT, KEY_POWER_PIN);
    isKeyDown[KEY_UP] = gpio_get(KEY_UP_PORT, KEY_UP_PIN);
    isKeyDown[KEY_DOWN] = gpio_get(KEY_DOWN_PORT, KEY_DOWN_PIN);
    isKeyDown[KEY_ENTER] = gpio_get(KEY_ENTER_PORT, KEY_ENTER_PIN);
    isKeyDown[KEY_BACK] = gpio_get(KEY_BACK_PORT, KEY_BACK_PIN);

#endif
}

#endif
