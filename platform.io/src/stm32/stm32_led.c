/*
 * Rad Pro
 * STM32 LED
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../led.h"

#include "device.h"

#if defined(PULSE_LED) || defined(PULSE_LED_EN)

void initPulseLED(void)
{
    // GPIO
    setPulseLED(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(PULSE_LED_PORT,
                      PULSE_LED_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PULSE_LED_PORT,
               PULSE_LED_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setPulseLED(bool value)
{
#if defined(PULSE_LED_ACTIVE_LOW)
    gpio_modify(PULSE_LED_PORT,
                PULSE_LED_PIN,
                !value);
#else
    gpio_modify(PULSE_LED_PORT,
                PULSE_LED_PIN,
                value);
#endif
}

#endif

#if defined(ALERT_LED) || defined(ALERT_LED_EN)

void initAlertLED(void)
{
    // GPIO
    setAlertLED(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(ALERT_LED_PORT,
                      ALERT_LED_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(ALERT_LED_PORT,
               ALERT_LED_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

void setAlertLED(bool value)
{
#if defined(ALERT_LED_ACTIVE_LOW)
    gpio_modify(ALERT_LED_PORT,
                ALERT_LED_PIN,
                !value);
#else
    gpio_modify(ALERT_LED_PORT,
                ALERT_LED_PIN,
                value);
#endif
}

#endif

#endif
