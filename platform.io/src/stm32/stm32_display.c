/*
 * Rad Pro
 * STM32 display
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../display.h"
#include "../settings.h"

#include "device.h"

void initDisplayBacklight(void)
{
#if defined(STM32F0) || defined(STM32G0)
    gpio_setup_af(DISPLAY_BACKLIGHT_PORT,
                  DISPLAY_BACKLIGHT_PIN,
                  GPIO_OUTPUTTYPE_PUSHPULL,
                  GPIO_OUTPUTSPEED_2MHZ,
                  GPIO_PULL_NONE,
                  DISPLAY_BACKLIGHT_AF);
#elif defined(STM32F1)
    gpio_setup(DISPLAY_BACKLIGHT_PORT,
               DISPLAY_BACKLIGHT_PIN,
               GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL);
#endif

    tim_setup_pwm(DISPLAY_BACKLIGHT_TIMER,
                  DISPLAY_BACKLIGHT_TIMER_CHANNEL);
    setDisplayBacklight(false);
    tim_set_period(DISPLAY_BACKLIGHT_TIMER,
                   DISPLAY_BACKLIGHT_TIMER_PERIOD);
    tim_enable(DISPLAY_BACKLIGHT_TIMER);
}

void setDisplayBacklight(bool value)
{
    tim_set_ontime(DISPLAY_BACKLIGHT_TIMER,
                   DISPLAY_BACKLIGHT_TIMER_CHANNEL,
                   value
                       ? displayBrightnessValue[settings.displayBrightness] *
                             (DISPLAY_BACKLIGHT_TIMER_PERIOD / 1000)
                       : 0);
}

#endif
