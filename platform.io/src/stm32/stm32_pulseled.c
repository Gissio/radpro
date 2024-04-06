/*
 * Rad Pro
 * STM32 pulse LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(PULSE_LED)

#include "../pulseled.h"

#include "device.h"

void initPulseLEDController(void)
{
    setPulseLED(false);

#if defined(STM32F0) || defined(STM32G0)
    gpio_setup_output(PULSE_LED_PORT,
               PULSE_LED_PIN,
               GPIO_OUTPUTTYPE_PUSHPULL,
               GPIO_OUTPUTSPEED_2MHZ,
               GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PULSE_LED_PORT,
               PULSE_LED_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setPulseLED(bool value)
{
    gpio_modify(PULSE_LED_PORT,
                PULSE_LED_PIN,
#if defined(PULSE_LED_ACTIVE_LOW)
                !
#endif
                value);
}

#endif
