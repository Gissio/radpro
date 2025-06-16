/*
 * Rad Pro
 * STM32 LED
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(PULSE_LED)

#include "../led.h"

#include "device.h"

void initLED(void)
{
#if defined(PULSE_LED)
    setPulseLED(false);
#endif
#if defined(ALERT_LED)
    setAlertLED(false);
#endif

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

#if defined(PULSE_LED)
    gpio_setup_output(PULSE_LED_PORT,
                      PULSE_LED_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#endif
#if defined(ALERT_LED)
    gpio_setup_output(ALERT_LED_PORT,
                      ALERT_LED_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#endif

#elif defined(STM32F1)
#if defined(PULSE_LED)
    gpio_setup(PULSE_LED_PORT,
               PULSE_LED_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
#if defined(ALERT_LED)
    gpio_setup(ALERT_LED_PORT,
               ALERT_LED_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
#endif
}

#if defined(PULSE_LED)

void setPulseLED(bool value)
{
    gpio_modify(PULSE_LED_PORT,
                PULSE_LED_PIN,
#if defined(PULSE_LED_ACTIVE_LOW)
                !
#endif
                value);
}

#endif

#if defined(ALERT_LED)

void setAlertLED(bool value)
{
    gpio_modify(ALERT_LED_PORT,
                ALERT_LED_PIN,
#if defined(ALERT_LED_ACTIVE_LOW)
                !
#endif
                value);
}

#endif

#endif
