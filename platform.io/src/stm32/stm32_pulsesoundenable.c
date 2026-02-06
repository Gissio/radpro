/*
 * Rad Pro
 * STM32 pulse sound enable
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#if defined(PULSESOUND_ENABLE)

#include "../devices/pulsesoundenable.h"
#include "../system/events.h"
#include "../stm32/device.h"

void initPulseSoundEnable(void)
{
    gpio_set(PULSE_SOUND_EN_PORT, PULSE_SOUND_EN_PIN);
}

void setPulseSoundEnable(bool value)
{
    gpio_setup(PULSE_SOUND_EN_PORT, PULSE_SOUND_EN_PIN, value ? GPIO_MODE_INPUT_PULLUP : GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
}

#endif

#endif
