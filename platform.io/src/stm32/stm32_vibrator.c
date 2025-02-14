/*
 * Rad Pro
 * STM32 vibration
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../vibration.h"

#include "device.h"

void initVibration(void)
{
    setVibration(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

#if defined(VIBRATION)
    gpio_setup_output(VIBRATION_PORT,
                      VIBRATION_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#endif

#elif defined(STM32F1)

#if defined(VIBRATION)
    gpio_setup(VIBRATION_PORT,
               VIBRATION_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif

#endif
}

void setVibration(bool value)
{
#if defined(VIBRATION)
    gpio_modify(VIBRATION_PORT,
                VIBRATION_PIN,
#if defined(VIBRATION_ACTIVE_LOW)
                !
#endif
                value);
#endif
}

#endif
