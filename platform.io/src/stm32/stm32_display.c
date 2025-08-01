/*
 * Rad Pro
 * STM32 display
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../display.h"
#include "../settings.h"

#include "device.h"

// Gamma-corrected linear brightness values

// value = 65536 * [0.25, 0.5, 0.75, 1] ^ 2.2
static const uint32_t displayBrightnessValue[] = {
    (uint32_t)(3104.1875F * DISPLAY_BACKLIGHT_BRIGHTNESS_MAX),
    (uint32_t)(14263.100F * DISPLAY_BACKLIGHT_BRIGHTNESS_MAX),
    (uint32_t)(34802.842F * DISPLAY_BACKLIGHT_BRIGHTNESS_MAX),
    (uint32_t)(65536.000F * DISPLAY_BACKLIGHT_BRIGHTNESS_MAX),
};

void initBacklight(void)
{
    // RCC
    rcc_enable_tim(DISPLAY_BACKLIGHT_TIMER);

    // GPIO
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

    // Timer
    tim_setup_pwm(DISPLAY_BACKLIGHT_TIMER,
                  DISPLAY_BACKLIGHT_TIMER_CHANNEL);
    tim_set_period(DISPLAY_BACKLIGHT_TIMER,
                   DISPLAY_BACKLIGHT_TIMER_PERIOD);
    tim_enable(DISPLAY_BACKLIGHT_TIMER);
}

void setBacklight(bool value)
{
    uint32_t ontime = ((displayBrightnessValue[settings.displayBrightness] *
                        DISPLAY_BACKLIGHT_TIMER_PERIOD) >>
                       16);

#if defined(DISPLAY_BACKLIGHT_ACTIVE_LOW)
    tim_set_ontime(DISPLAY_BACKLIGHT_TIMER,
                   DISPLAY_BACKLIGHT_TIMER_CHANNEL,
                   DISPLAY_BACKLIGHT_TIMER_PERIOD - ontime);
#else
    tim_set_ontime(DISPLAY_BACKLIGHT_TIMER,
                   DISPLAY_BACKLIGHT_TIMER_CHANNEL,
                   ontime);
#endif
}

#endif
