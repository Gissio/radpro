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

#include "../peripherals/pulsesoundenable.h"
#include "../stm32/device.h"

// The output data bit stays set, so only the mode changes:
// enabled: input pull-up, disabled: output driven high
_Static_assert(PULSE_SOUND_EN_PIN < 8, "PULSE_SOUND_EN must be on CRL");

#define PULSE_SOUND_EN_SHIFT (4 * PULSE_SOUND_EN_PIN)
#define PULSE_SOUND_EN_MODE_INPUT_PULLUP (GPIO_MODE_INPUT_PULLUP & GPIO_MODE_Msk)

void setPulseSoundEnable(bool value)
{
    // CRL is shared with the backlight pin, reconfigured from the
    // higher-priority pulse IRQ (never called with IRQs masked)
    __disable_irq();

    modify_bits(PULSE_SOUND_EN_PORT->CRL,
                GPIO_MODE_Msk << PULSE_SOUND_EN_SHIFT,
                (value ? PULSE_SOUND_EN_MODE_INPUT_PULLUP : GPIO_MODE_OUTPUT_50MHZ_PUSHPULL) << PULSE_SOUND_EN_SHIFT);

    __enable_irq();
}

#endif

#endif
