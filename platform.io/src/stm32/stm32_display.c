/*
 * Rad Pro
 * STM32 display
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../devices/display.h"
#include "../stm32/device.h"
#include "../system/settings.h"

// Gamma-corrected linear brightness values

// value = 65536 * [0.25, 0.5, 0.75, 1] ^ 2.2
static const uint32_t displayBrightnessValue[] = {
    (uint32_t)(3104.1875F),
    (uint32_t)(14263.100F),
    (uint32_t)(34802.842F),
    (uint32_t)(65536.000F),
};

#if defined(GC01)

void initBacklight(void)
{
    // RCC
    rcc_enable_tim(DISPLAY_BACKLIGHT_TIMER);

    // GPIO
    gpio_setup(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL);

    // Timer
    tim_setup_pwm(DISPLAY_BACKLIGHT_TIMER, DISPLAY_BACKLIGHT_TIMER_CHANNEL);
    tim_set_period(DISPLAY_BACKLIGHT_TIMER, DISPLAY_BACKLIGHT_TIMER_PERIOD);
    tim_enable(DISPLAY_BACKLIGHT_TIMER);
}

void setBacklight(bool value)
{
    uint32_t onTime = DISPLAY_BACKLIGHT_TIMER_PERIOD -
                      (value ? ((displayBrightnessValue[settings.displayBrightness] * DISPLAY_BACKLIGHT_TIMER_PERIOD) >> 16) : 0);

    tim_set_ontime(DISPLAY_BACKLIGHT_TIMER, DISPLAY_BACKLIGHT_TIMER_CHANNEL, onTime);
}

#else

void initBacklight(void)
{
}

void setBacklight(bool value)
{
    if (value)
    {
        // RCC
        rcc_enable_tim(DISPLAY_BACKLIGHT_TIMER);

        // GPIO
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
        gpio_setup_af(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING, DISPLAY_BACKLIGHT_AF);
#elif defined(STM32F1)
        gpio_setup(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL);
#endif

        // Timer
        uint32_t period = DISPLAY_BACKLIGHT_TIMER_PERIOD;
        uint32_t onTime = ((displayBrightnessValue[settings.displayBrightness] * DISPLAY_BACKLIGHT_TIMER_PERIOD) >> 16);
        uint32_t prescalerFactor = prescalePWMParameters(&period, &onTime);

#if defined(DISPLAY_BACKLIGHT_ACTIVE_LOW)
        onTime = period - onTime;
#endif

        tim_setup_pwm(DISPLAY_BACKLIGHT_TIMER, DISPLAY_BACKLIGHT_TIMER_CHANNEL);
        tim_set_prescaler_factor(DISPLAY_BACKLIGHT_TIMER, prescalerFactor);
        tim_set_period(DISPLAY_BACKLIGHT_TIMER, period);
        tim_set_ontime(DISPLAY_BACKLIGHT_TIMER, DISPLAY_BACKLIGHT_TIMER_CHANNEL, onTime);
        tim_generate_update(DISPLAY_BACKLIGHT_TIMER);
        tim_enable(DISPLAY_BACKLIGHT_TIMER);
    }
    else
    {
        // GPIO
        gpio_clear(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN);
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
        gpio_setup_output(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
        gpio_setup(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif

        // Timer
        tim_disable(DISPLAY_BACKLIGHT_TIMER);

        // RCC
        rcc_disable_tim(DISPLAY_BACKLIGHT_TIMER);
    }
}

#endif

#endif
