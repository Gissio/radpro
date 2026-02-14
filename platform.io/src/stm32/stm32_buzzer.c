/*
 * Rad Pro
 * STM32 buzzer
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#if defined(BUZZER)

#include "../peripherals/buzzer.h"
#include "../system/events.h"
#include "../stm32/device.h"

void initBuzzer(void)
{
    // RCC
#if defined(BUZZER_TIMER)
    rcc_enable_tim(BUZZER_TIMER);
#endif

    // GPIO
    setBuzzer(false);

#if !defined(BUZZER_TIMER)

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(BUZZER_PORT, BUZZER_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#if defined(BUZZER2_PORT)
    gpio_setup_output(BUZZER2_PORT, BUZZER2_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#endif
#elif defined(STM32F1)
    gpio_setup(BUZZER_PORT, BUZZER_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#if defined(BUZZER2_PORT)
    gpio_setup(BUZZER2_PORT, BUZZER2_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
#endif

#else

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_af(BUZZER_PORT, BUZZER_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING, BUZZER_AF);
#elif defined(STM32F1)
    gpio_setup(BUZZER_PORT, BUZZER_PIN, GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL);
#endif

    // Timer
    tim_setup_pwm(BUZZER_TIMER, BUZZER_TIMER_CHANNEL);
    tim_set_period(BUZZER_TIMER, BUZZER_TIMER_PERIOD);
    setBuzzer(false);
    tim_enable(BUZZER_TIMER);

#endif
}

void setBuzzer(bool value)
{
#if !defined(BUZZER_TIMER)

#if defined(BUZZER_ACTIVE_LOW)
    gpio_modify(BUZZER_PORT, BUZZER_PIN, !value);
#else
    gpio_modify(BUZZER_PORT, BUZZER_PIN, value);
#endif

#if defined(BUZZER2_PORT)
#if defined(BUZZER_ACTIVE_LOW)
    gpio_modify(BUZZER2_PORT, BUZZER2_PIN, !value);
#else
    gpio_modify(BUZZER2_PORT, BUZZER2_PIN, value);
#endif
#endif

#else

    tim_set_ontime(BUZZER_TIMER, BUZZER_TIMER_CHANNEL, value ? BUZZER_TIMER_PERIOD / 2 : 0);

#endif
}

#endif

#endif
