/*
 * Rad Pro
 * STM32 buzzer
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "../buzzer.h"

#include "stm32.h"

void initBuzzerHardware(void)
{
    // GPIO

#if defined(BUZZ_ACTIVE_LOW)
    setBuzzer(false);
#endif

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(BUZZ_PORT,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    BUZZ_PIN);

#if defined(BUZZ_PORT2)
    gpio_mode_setup(BUZZ_PORT2,
                    GPIO_MODE_OUTPUT,
                    GPIO_PUPD_NONE,
                    BUZZ_PIN2);
#endif

#elif defined(STM32F1)

#if defined(BUZZ_TIMER)
    gpio_set_mode(BUZZ_PORT,
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  BUZZ_PIN);
#else
    gpio_set_mode(BUZZ_PORT,
                  GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL,
                  BUZZ_PIN);
#endif

#endif

    // Buzzer timer

#if defined(BUZZ_TIMER)

    rcc_periph_clock_enable(BUZZ_TIMER_RCC);

    BUZZ_TIMER_CCMR(BUZZ_TIMER) |= BUZZ_TIMER_CCMR_MODE; // timer_set_oc_mode(TUBE_HV_TIMER, TIM_OC1, TIM_OCM_PWM1);
    TIM_ARR(BUZZ_TIMER) = BUZZ_PWM_PERIOD;               // timer_set_period(BUZZ_TIMER, BUZZ_PWM_PERIOD);

    setBuzzer(false);

    TIM_CCER(BUZZ_TIMER) |= BUZZ_TIMER_CCER_CC; // timer_enable_oc_output(TUBE_HV_TIMER, TIM_OC1);
    TIM_CR1(BUZZ_TIMER) |= TIM_CR1_CEN;         // timer_enable_counter(TUBE_HV_TIMER);

#endif
}

void setBuzzer(bool value)
{
#if !defined(BUZZ_TIMER)

#if defined(BUZZ_ACTIVE_LOW)

    if (value)
    {
        gpio_clear(BUZZ_PORT, BUZZ_PIN);
#if defined(BUZZ_PORT2)
        gpio_clear(BUZZ_PORT2, BUZZ_PIN2);
#endif
    }
    else
    {
        gpio_set(BUZZ_PORT, BUZZ_PIN);
#if defined(BUZZ_PORT2)
        gpio_set(BUZZ_PORT2, BUZZ_PIN2);
#endif
    }

#else

    if (value)
        gpio_set(BUZZ_PORT, BUZZ_PIN);
    else
        gpio_clear(BUZZ_PORT, BUZZ_PIN);

#endif

#else

    // Set PWM duty cycle

    uint32_t ccr = value
                       ? BUZZ_PWM_PERIOD / 2
                       : 0;
    BUZZ_TIMER_CCR(BUZZ_TIMER) = ccr; // timer_set_oc_value(BUZZ_TIMER, xxx, crr);

#endif
}

void syncBuzzer(void)
{
#if !defined(BUZZ_TIMER)
    for (uint32_t i = 0; i < 10; i++)
    {
        if ((GPIO_ODR(BUZZ_PORT) &
             BUZZ_PIN) == 0)
             break;

        sleep(1);
    }
#endif
}

#endif
