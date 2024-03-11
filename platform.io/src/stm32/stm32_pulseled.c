/*
 * Rad Pro
 * STM32 pulse LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(PULSELED)

#include "../pulseled.h"

#include "device.h"

void initPulseLEDController(void)
{
    setPulseLED(false);

#if defined(STM32F0) || defined(STM32G0)
    gpio_setup_output(PULSELED_PORT,
               PULSELED_PIN,
               GPIO_OUTPUTTYPE_PUSHPULL,
               GPIO_OUTPUTSPEED_2MHZ,
               GPIO_PULL_NONE);
#elif defined(STM32F1)
    gpio_setup(PULSELED_PORT,
               PULSELED_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setPulseLED(bool value)
{
    gpio_modify(PULSELED_PORT,
                PULSELED_PIN,
#if PULSELED_ACTIVE_LOW
                !
#endif
                value);
}

#endif
