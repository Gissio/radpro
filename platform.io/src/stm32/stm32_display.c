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
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_af(DISPLAY_BACKLIGHT_PORT,
                  DISPLAY_BACKLIGHT_PIN,
                  GPIO_OUTPUTTYPE_PUSHPULL,
                  GPIO_OUTPUTSPEED_2MHZ,
                  GPIO_PULL_FLOATING,
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
    uint32_t ontime =
        value
            ? displayBrightnessValue[settings.displayBrightness] *
                  (DISPLAY_BACKLIGHT_TIMER_PERIOD / 1000)
            : 0;

    tim_set_ontime(DISPLAY_BACKLIGHT_TIMER,
                   DISPLAY_BACKLIGHT_TIMER_CHANNEL,
#if defined(DISPLAY_BACKLIGHT_ACTIVE_LOW)
                   DISPLAY_BACKLIGHT_TIMER_PERIOD - ontime
#else
                   ontime
#endif
    );
}

#endif
