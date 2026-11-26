/*
 * Rad Pro
 * STM32 display
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/display.h"
#include "../stm32/device.h"
#include "../system/cmath.h"
#include "../system/settings.h"

// Fixed prescaler, so the PWM period fits in a 16-bit timer
#define BACKLIGHT_PRESCALER (DISPLAY_BACKLIGHT_TIMER_PERIOD / 65536 + 1)
#define BACKLIGHT_PERIOD (DISPLAY_BACKLIGHT_TIMER_PERIOD / BACKLIGHT_PRESCALER)

#if defined(GC01)
#define BACKLIGHT_ONTIME(duty) ((uint16_t)((1.0F - (duty)) * BACKLIGHT_PERIOD))
#else
#define BACKLIGHT_ONTIME(duty) ((uint16_t)((duty) * BACKLIGHT_PERIOD))
#endif

// PWM on-time for off, then gamma-corrected brightness levels:
// [0.25, 0.5, 0.75, 1] ^ 2.2
static const uint16_t backlightOnTime[] = {
    BACKLIGHT_ONTIME(0.0F),
    BACKLIGHT_ONTIME(0.0473661F),
    BACKLIGHT_ONTIME(0.2176376F),
    BACKLIGHT_ONTIME(0.5310492F),
    BACKLIGHT_ONTIME(1.0F),
};

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
#define setupBacklightAF() \
    gpio_setup_af(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING, DISPLAY_BACKLIGHT_AF)
#define setupBacklightOutput() \
    gpio_setup_output(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING)
#elif defined(STM32F1)
#define setupBacklightAF() \
    gpio_setup(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL)
#define setupBacklightOutput() \
    gpio_setup(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL)
#endif

#if defined(GC01)

// TIM1 is shared with the buzzer: it keeps running, and its prescaler and
// period must match the buzzer's setup

_Static_assert(BACKLIGHT_PRESCALER == 1, "GC-01 backlight shares TIM1 with the buzzer (no prescaler)");

void initBacklight(void)
{
    // RCC
    rcc_enable_tim(DISPLAY_BACKLIGHT_TIMER);

    // GPIO
    setupBacklightAF();

    // Timer (backlight off before enabling: CCR resets to 0, which is full brightness)
    tim_setup_pwm(DISPLAY_BACKLIGHT_TIMER, DISPLAY_BACKLIGHT_TIMER_CHANNEL);
    tim_set_period(DISPLAY_BACKLIGHT_TIMER, BACKLIGHT_PERIOD);
    setBacklight(false);
    tim_enable(DISPLAY_BACKLIGHT_TIMER);
}

void setBacklight(bool value)
{
    tim_set_ontime(DISPLAY_BACKLIGHT_TIMER,
                   DISPLAY_BACKLIGHT_TIMER_CHANNEL,
                   backlightOnTime[value ? (settings.displayBrightness + 1) : 0]);
}

#else

// The timer is configured once; its registers are retained while its
// clock is gated off, so switching on only needs clock, on-time and pin

void initBacklight(void)
{
    rcc_enable_tim(DISPLAY_BACKLIGHT_TIMER);

    tim_setup_pwm(DISPLAY_BACKLIGHT_TIMER, DISPLAY_BACKLIGHT_TIMER_CHANNEL);
#if BACKLIGHT_PRESCALER > 1
    tim_set_prescaler_factor(DISPLAY_BACKLIGHT_TIMER, BACKLIGHT_PRESCALER);
    tim_generate_update(DISPLAY_BACKLIGHT_TIMER);
#endif
    tim_set_period(DISPLAY_BACKLIGHT_TIMER, BACKLIGHT_PERIOD);
    tim_enable(DISPLAY_BACKLIGHT_TIMER);

    rcc_disable_tim(DISPLAY_BACKLIGHT_TIMER);
}

NOINLINE void setBacklight(bool value)
{
    // Called from the main thread, SysTick and the pulse IRQ: the RCC and
    // GPIO read-modify-writes below must not interleave with each other or
    // with other writers of the same registers (never called with IRQs masked)
    __disable_irq();

    if (value)
    {
        rcc_enable_tim(DISPLAY_BACKLIGHT_TIMER);
        tim_set_ontime(DISPLAY_BACKLIGHT_TIMER,
                       DISPLAY_BACKLIGHT_TIMER_CHANNEL,
                       backlightOnTime[settings.displayBrightness + 1]);
        setupBacklightAF();
    }
    else
    {
        // Drive the pin low before gating the timer clock, which freezes its output
        gpio_modify(DISPLAY_BACKLIGHT_PORT, DISPLAY_BACKLIGHT_PIN, false);
        setupBacklightOutput();
        rcc_disable_tim(DISPLAY_BACKLIGHT_TIMER);
    }

    __enable_irq();
}

#endif

#endif
