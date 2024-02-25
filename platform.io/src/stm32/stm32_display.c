/*
 * Rad Pro
 * Bosean FS-600/FS-1000 specifics
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include "../display.h"
#include "../settings.h"

#include "device.h"

void setDisplayBacklight(bool value)
{
    uint32_t ccr =
        value
            ? displayBrightnessValue[settings.displayBrightness] *
                  (LCD_BACKLIGHT_PWM_PERIOD / 1000)
            : 0;
    LCD_BACKLIGHT_TIMER_CCR(LCD_BACKLIGHT_TIMER) = ccr; // timer_set_oc_value(LCD_BACKLIGHT_TIMER, TIM_OC1, crr);
}

void syncDisplayBacklight(void)
{
    while (TIM_CNT(LCD_BACKLIGHT_TIMER) >= (LCD_BACKLIGHT_PWM_PERIOD / 4))
        ;
}

#endif
