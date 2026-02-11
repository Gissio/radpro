/*
 * Rad Pro
 * STM32 Geiger-Müller tubeHardware
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../devices/tube.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../stm32/device.h"

#define TUBE_HV_LOW_FREQUENCY 1250
#define TUBE_HV_LOW_FREQUENCY_PERIOD (TUBE_HV_FREQUENCY / \
                                      TUBE_HV_LOW_FREQUENCY)
#define TUBE_HV_LOW_DUTYCYCLE_MULTIPLIER ((uint32_t)(TUBE_HVDUTYCYCLE_VALUE_STEP * \
                                                     TUBE_HV_LOW_FREQUENCY_PERIOD))

#define TUBE_BITS_PER_PULSE 2
#define TUBE_BITS_PER_PULSE_MASK ((1 << TUBE_BITS_PER_PULSE) - 1)

struct
{
    bool enabled;

    uint32_t lastTick;
    uint32_t lastTimerCount;
} tubeHardware;

void initTubeHardware(void)
{
    // RCC
#if defined(TUBE_HV_PWM)
    rcc_enable_tim(TUBE_HV_TIMER);
#endif
    rcc_enable_tim(TUBE_DET_TIMER);

    // GPIO
#if defined(TUBE_HV_PWM)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_af(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING, TUBE_HV_AF);
#elif defined(STM32F1)
    gpio_setup(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);
#endif
#endif

#if !defined(TUBE_HV_PWM)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
#endif
#endif

#if defined(TUBE_DET_PULLUP)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_input(TUBE_DET_PORT, TUBE_DET_PIN, GPIO_PULL_PULLUP);
#elif defined(STM32F1)
    gpio_setup(TUBE_DET_PORT, TUBE_DET_PIN, GPIO_MODE_INPUT_PULLUP);
#endif
#elif defined(TUBE_DET_PULLDOWN)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_input(TUBE_DET_PORT, TUBE_DET_PIN, GPIO_PULL_PULLDOWN);
#elif defined(STM32F1)
    gpio_setup(TUBE_DET_PORT, TUBE_DET_PIN, GPIO_MODE_INPUT_PULLDOWN);
#endif
#else
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_input(TUBE_DET_PORT, TUBE_DET_PIN, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(TUBE_DET_PORT, TUBE_DET_PIN, GPIO_MODE_INPUT_FLOATING);
#endif
#endif

    // HV PWM timer
#if defined(TUBE_HV_PWM)
    tim_setup_pwm(TUBE_HV_TIMER, TUBE_HV_TIMER_CHANNEL, false);

    updateTubeHV();

    tim_enable(TUBE_HV_TIMER);
#endif

    // Pulse detection timer
    tim_setup_single(TUBE_DET_TIMER);
    tim_set_prescaler_factor(TUBE_DET_TIMER, TUBE_DET_FREQUENCY / PULSE_MEASUREMENT_FREQUENCY);
    tim_enable(TUBE_DET_TIMER);

    // EXTI
    exti_setup(TUBE_DET_PORT, TUBE_DET_PIN, false, true);
    exti_enable_interrupt(TUBE_DET_PIN);

    NVIC_SetPriority(TUBE_DET_IRQ, 0x0);
    NVIC_EnableIRQ(TUBE_DET_IRQ);
}

void setTubeHVEnabled(bool value)
{
    tubeHardware.enabled = value;

    updateTubeHV();
}

void updateTubeHV(void)
{
#if defined(TUBE_HV_PWM)
    uint32_t period = TUBE_HV_FREQUENCY / getTubeHVFrequency();
    uint32_t onTime = tubeHardware.enabled ? (uint32_t)((float)period * getTubeHVDutyCycle() + 0.5F) : 0;
    uint32_t prescalerFactor = prescalePWMParameters(&period, &onTime);

    tim_set_prescaler_factor(TUBE_HV_TIMER, prescalerFactor);
    tim_set_period(TUBE_HV_TIMER, period);
    tim_set_ontime(TUBE_HV_TIMER, TUBE_HV_TIMER_CHANNEL, onTime);
    tim_generate_update(TUBE_HV_TIMER);
#else
    gpio_modify(TUBE_HV_PORT, TUBE_HV_PIN, tubeHardware.enabled);
#endif
}

void TUBE_DET_IRQ_HANDLER(void)
{
    exti_clear_pending_interrupt(TUBE_DET_PIN);

    tubePulseCount++;

    uint32_t timerCount = TUBE_DET_TIMER->CNT;

    tubeRandomBits = (tubeRandomBits << TUBE_BITS_PER_PULSE) | (timerCount & TUBE_BITS_PER_PULSE_MASK);

    uint32_t tickTime = currentTick - tubeHardware.lastTick;
    if (tickTime < 64)
        tubeDeadTime = timerCount - tubeHardware.lastTimerCount;

    tubeHardware.lastTick = currentTick;
    tubeHardware.lastTimerCount = timerCount;
}

bool readTubeDet(void)
{
    return !gpio_get(TUBE_DET_PORT, TUBE_DET_PIN);
}

#endif
