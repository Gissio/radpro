/*
 * Rad Pro
 * STM32 LED
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/led.h"
#include "../stm32/device.h"

#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)

// Output level is set by updateLED() right after init
static void setupLEDOutput(GPIO_TypeDef *port, uint8_t pin)
{
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(port, pin, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(port, pin, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
}

#endif

#if defined(PULSE_LED)

void initPulseLED(void)
{
    setupLEDOutput(PULSE_LED_PORT, PULSE_LED_PIN);
}

void setPulseLED(bool value)
{
#if defined(PULSE_LED_ACTIVE_LOW)
    value = !value;
#endif
    gpio_modify(PULSE_LED_PORT, PULSE_LED_PIN, value);
}

#endif

#if defined(ALERT_LED)

void initAlertLED(void)
{
    setupLEDOutput(ALERT_LED_PORT, ALERT_LED_PIN);
}

void setAlertLED(bool value)
{
#if defined(ALERT_LED_ACTIVE_LOW)
    value = !value;
#endif
    gpio_modify(ALERT_LED_PORT, ALERT_LED_PIN, value);
}

#endif

#if defined(PULSE_LED_EN)

void initPulseLEDEnable(void)
{
    setupLEDOutput(PULSE_LED_EN_PORT, PULSE_LED_EN_PIN);
}

void setPulseLEDEnable(bool value)
{
#if defined(PULSE_LED_EN_ACTIVE_LOW)
    value = !value;
#endif
    gpio_modify(PULSE_LED_EN_PORT, PULSE_LED_EN_PIN, value);
}

#endif

#if defined(ALERT_LED_EN)

void initAlertLEDEnable(void)
{
    setupLEDOutput(ALERT_LED_EN_PORT, ALERT_LED_EN_PIN);
}

void setAlertLEDEnable(bool value)
{
#if defined(ALERT_LED_EN_ACTIVE_LOW)
    value = !value;
#endif
    gpio_modify(ALERT_LED_EN_PORT, ALERT_LED_EN_PIN, value);
}

#endif

#endif
