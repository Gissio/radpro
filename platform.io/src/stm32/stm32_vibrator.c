/*
 * Rad Pro
 * STM32 vibrator
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(VIBRATOR)

#include "../vibrator.h"

#include "device.h"

void initVibrator(void)
{
    setVibrator(false);

#if defined(STM32F0) || defined(STM32G0)
    gpio_setup_output(VIBRATOR_PORT,
               VIBRATOR_PIN,
               GPIO_OUTPUTTYPE_PUSHPULL,
               GPIO_OUTPUTSPEED_2MHZ,
               GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(VIBRATOR_PORT,
               VIBRATOR_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setVibrator(bool value)
{
    gpio_modify(VIBRATOR_PORT,
                VIBRATOR_PIN,
#if defined(VIBRATOR_ACTIVE_LOW)
                !
#endif
                value);
}

#endif
