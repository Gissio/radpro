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
#include "../stm32/device.h"

#if defined(BUZZER_TIMER)

#if defined(BUZZER_ACTIVE_LOW) || defined(BUZZER2_PORT)
#error "BUZZER_ACTIVE_LOW and BUZZER2_PORT are not supported with BUZZER_TIMER"
#endif

#if defined(BUZZER_VOLUME)
static uint16_t buzzerOnTime = BUZZER_TIMER_PERIOD / 2;
#else
#define buzzerOnTime (BUZZER_TIMER_PERIOD / 2)
#endif

void initBuzzer(void)
{
    // RCC
    rcc_enable_tim(BUZZER_TIMER);

    // GPIO
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
}

#if defined(BUZZER_VOLUME)
void setBuzzerVolume(uint8_t volume)
{
    // Volume 0-3: 1/128, 1/32, 1/8, 1/2 of the period
    buzzerOnTime = BUZZER_TIMER_PERIOD >> (7 - 2 * volume);
}
#endif

void setBuzzer(bool value)
{
    tim_set_ontime(BUZZER_TIMER, BUZZER_TIMER_CHANNEL, value ? buzzerOnTime : 0);
}

#else

#if defined(BUZZER_VOLUME)
#error "BUZZER_VOLUME requires BUZZER_TIMER"
#endif

#if defined(BUZZER_ACTIVE_LOW)
#define BUZZER_LEVEL(value) (!(value))
#else
#define BUZZER_LEVEL(value) (value)
#endif

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
#define setupBuzzerOutput(port, pin) \
    gpio_setup_output(port, pin, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING)
#elif defined(STM32F1)
#define setupBuzzerOutput(port, pin) \
    gpio_setup(port, pin, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL)
#endif

void initBuzzer(void)
{
    // GPIO (output level is set before enabling the output)
    setBuzzer(false);

    setupBuzzerOutput(BUZZER_PORT, BUZZER_PIN);
#if defined(BUZZER2_PORT)
    setupBuzzerOutput(BUZZER2_PORT, BUZZER2_PIN);
#endif
}

void setBuzzer(bool value)
{
    gpio_modify(BUZZER_PORT, BUZZER_PIN, BUZZER_LEVEL(value));
#if defined(BUZZER2_PORT)
    gpio_modify(BUZZER2_PORT, BUZZER2_PIN, BUZZER_LEVEL(value));
#endif
}

#endif

#endif

#endif
