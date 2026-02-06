/*
 * Rad Pro
 * STM32 vibrator
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#if defined(VIBRATOR)

#include "../devices/vibrator.h"
#include "../stm32/device.h"

void initVibrator(void)
{
    // GPIO
    setVibrator(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(VIBRATOR_PORT, VIBRATOR_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);

#elif defined(STM32F1)
    gpio_setup(VIBRATOR_PORT, VIBRATOR_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setVibrator(bool value)
{
#if defined(VIBRATOR_ACTIVE_LOW)
    gpio_modify(VIBRATOR_PORT, VIBRATOR_PIN, !value);
#else
    gpio_modify(VIBRATOR_PORT, VIBRATOR_PIN, value);
#endif
}

#endif

#endif
