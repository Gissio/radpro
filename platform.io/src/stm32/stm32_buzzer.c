/*
 * Rad Pro
 * STM32 buzzer
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../buzzer.h"
#include "../events.h"

#include "device.h"

void initBuzzerController(void)
{
    // GPIO
    setBuzzer(false);

#if !defined(BUZZ_TIMER)

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

    gpio_setup_output(BUZZ_PORT,
                      BUZZ_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);

#if defined(BUZZ2_PORT)
    gpio_setup_output(BUZZ2_PORT,
                      BUZZ2_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#endif

#elif defined(STM32F1)

    gpio_setup(BUZZ_PORT,
               BUZZ_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);

#if defined(BUZZ2_PORT)
    gpio_setup(BUZZ2_PORT,
               BUZZ2_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif

#endif

#else

    gpio_setup(BUZZ_PORT,
               BUZZ_PIN,
               GPIO_MODE_OUTPUT_2MHZ_AF_PUSHPULL);

    tim_setup_pwm(BUZZ_TIMER,
                  BUZZ_TIMER_CHANNEL);
    tim_set_period(BUZZ_TIMER,
                   BUZZ_TIMER_PERIOD);
    setBuzzer(false);
    tim_enable(BUZZ_TIMER);

#endif
}

void setBuzzer(bool value)
{
#if !defined(BUZZ_TIMER)

    gpio_modify(BUZZ_PORT,
                BUZZ_PIN,
#if defined(BUZZ_ACTIVE_LOW)
                !
#endif
                value);

#if defined(BUZZ2_PORT)
    gpio_modify(BUZZ2_PORT,
                BUZZ2_PIN,
#if defined(BUZZ_ACTIVE_LOW)
                !
#endif
                value);
#endif

#else

    tim_set_ontime(BUZZ_TIMER,
                   BUZZ_TIMER_CHANNEL,
                   value
                       ? BUZZ_TIMER_PERIOD / 2
                       : 0);

#endif
}

#endif
