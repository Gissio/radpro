/*
 * Rad Pro
 * STM32 vibrator
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../vibrator.h"

#include "device.h"

void initVibratorController(void)
{
    setVibrator(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

#if defined(VIBRATOR)
    gpio_setup_output(VIBRATOR_PORT,
               VIBRATOR_PIN,
               GPIO_OUTPUTTYPE_PUSHPULL,
               GPIO_OUTPUTSPEED_2MHZ,
               GPIO_PULL_FLOATING);
#endif

#elif defined(STM32F1)

#if defined(VIBRATOR)
    gpio_setup(VIBRATOR_PORT,
               VIBRATOR_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif

#endif
}

void setVibrator(bool value)
{
#if defined(VIBRATOR)
    gpio_modify(VIBRATOR_PORT,
                VIBRATOR_PIN,
#if defined(VIBRATOR_ACTIVE_LOW)
                !
#endif
                value);
#endif
}

#endif
