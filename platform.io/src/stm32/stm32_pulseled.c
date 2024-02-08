/*
 * Rad Pro
 * SDLSim pulse LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(PULSE_LED)

#include <libopencm3/stm32/gpio.h>

#include "../pulseled.h"

#include "stm32.h"

void initPulseLEDHardware(void)
{
    // GPIO

#if defined(PULSE_LED_ACTIVE_LOW)
    setPulseLED(false);
#endif

#if defined(STM32F0) || defined(STM32G0)
    gpio_mode_setup(PULSE_LED_PORT,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    PULSE_LED_PIN);
#elif defined(STM32F1)
    gpio_set_mode(PULSE_LED_PORT,
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  PULSE_LED_PIN);
#endif
}

void setPulseLED(bool value)
{
#if defined(PULSE_LED_ACTIVE_LOW)
    if (value)
        gpio_clear(PULSE_LED_PORT, PULSE_LED_PIN);
    else
        gpio_set(PULSE_LED_PORT, PULSE_LED_PIN);
#else
    if (value)
        gpio_set(PULSE_LED_PORT, PULSE_LED_PIN);
    else
        gpio_clear(PULSE_LED_PORT, PULSE_LED_PIN);
#endif
}

#endif
