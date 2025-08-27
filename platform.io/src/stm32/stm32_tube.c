/*
 * Rad Pro
 * STM32 Geiger-Müller tubeHardware
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../cmath.h"
#include "../events.h"
#include "../settings.h"
#include "../tube.h"

#include "device.h"

#define TUBE_HV_LOW_FREQUENCY 1250
#define TUBE_HV_LOW_FREQUENCY_PERIOD (TUBE_HV_FREQUENCY / \
                                      TUBE_HV_LOW_FREQUENCY)
#define TUBE_HV_LOW_DUTYCYCLE_MULTIPLIER ((uint32_t)(TUBE_HVDUTYCYCLE_VALUE_STEP * \
                                                     TUBE_HV_LOW_FREQUENCY_PERIOD))

#define TUBE_PULSE_QUEUE_SIZE 64
#define TUBE_PULSE_QUEUE_MASK (TUBE_PULSE_QUEUE_SIZE - 1)

static struct
{
    bool enabled;

    volatile uint32_t pulseTimeQueueHead;
    volatile uint32_t pulseTimeQueueTail;
    volatile uint32_t pulseTimeQueue[TUBE_PULSE_QUEUE_SIZE];
} tubeHardware;

void initTubeHardware(void)
{
    // RCC
#if defined(TUBE_HV_PWM)
    rcc_enable_tim(TUBE_HV_TIMER);
#endif
    rcc_enable_tim(TUBE_DET_TIMER_MASTER);
    rcc_enable_tim(TUBE_DET_TIMER_SLAVE);

    // GPIO
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

#if defined(TUBE_HV_PWM)
    gpio_setup_af(TUBE_HV_PORT,
                  TUBE_HV_PIN,
                  GPIO_OUTPUTTYPE_PUSHPULL,
                  GPIO_OUTPUTSPEED_50MHZ,
                  GPIO_PULL_FLOATING,
                  TUBE_HV_AF);
#else
    gpio_setup_output(TUBE_HV_PORT,
                      TUBE_HV_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#endif
    gpio_setup_input(TUBE_DET_PORT,
                     TUBE_DET_PIN,
#if defined(TUBE_DET_PULLUP)
                     GPIO_PULL_UP
#elif defined(TUBE_DET_PULLDOWN)
                     GPIO_PULL_DOWN
#else
                     GPIO_PULL_FLOATING
#endif
    );

#elif defined(STM32F1)

#if defined(TUBE_HV_PWM)
    gpio_setup(TUBE_HV_PORT,
               TUBE_HV_PIN,
               GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);
#else
    gpio_setup(TUBE_HV_PORT,
               TUBE_HV_PIN,
               GPIO_MODE_OUTPUT_50MHZ_PUSHPULL);
#endif
    gpio_setup(TUBE_DET_PORT,
               TUBE_DET_PIN,
#if defined(TUBE_DET_PULLUP)
               GPIO_MODE_INPUT_PULLUP
#elif defined(TUBE_DET_PULLDOWN)
               GPIO_MODE_INPUT_PULLDOWN
#else
               GPIO_MODE_INPUT_FLOATING
#endif
    );

#endif

    // HV PWM timer
#if defined(TUBE_HV_PWM)
    tim_setup_pwm(TUBE_HV_TIMER,
                  TUBE_HV_TIMER_CHANNEL);

    updateTubeHV();

    tim_enable(TUBE_HV_TIMER);
#endif

    // Pulse detection timer
    tim_setup_linked(TUBE_DET_TIMER_MASTER,
                     TUBE_DET_TIMER_SLAVE,
                     TUBE_DET_TIMER_TRIGGER_CONNECTION);
    tim_set_prescaler_factor(TUBE_DET_TIMER_MASTER,
                             TUBE_DET_FREQUENCY / PULSE_MEASUREMENT_FREQUENCY);

    tim_enable(TUBE_DET_TIMER_SLAVE);
    tim_enable(TUBE_DET_TIMER_MASTER);

    // EXTI
    exti_setup(TUBE_DET_PORT,
               TUBE_DET_PIN,
               false,
               true);

    NVIC_SetPriority(TUBE_DET_IRQ, 0x0);
    NVIC_EnableIRQ(TUBE_DET_IRQ);

    exti_enable_interrupt(TUBE_DET_PIN);
}

void setTubeHV(bool value)
{
    tubeHardware.enabled = value;

    updateTubeHV();
}

void updateTubeHV(void)
{
#if defined(TUBE_HV_PWM)
    uint32_t period = TUBE_HV_FREQUENCY / getTubeHVFrequency();
    uint32_t onTime = tubeHardware.enabled
                          ? period * getTubeHVDutyCycle() + 0.5F
                          : 0;

    // Get presacler factor
    uint32_t prescalerFactor = getGCD(period, onTime);
    period /= prescalerFactor;
    onTime /= prescalerFactor;

    // Scale prescaler factor
    while (prescalerFactor >= 0x10000)
    {
        period <<= 1;
        onTime <<= 1;
        prescalerFactor >>= 1;
    }

    // Scale period
    while (period >= 0x10000)
    {
        period >>= 1;
        onTime >>= 1;
        prescalerFactor <<= 1;
    }

    tim_set_prescaler_factor(TUBE_HV_TIMER,
                             prescalerFactor);

    tim_set_period(TUBE_HV_TIMER,
                   period);

    tim_set_ontime(TUBE_HV_TIMER,
                   TUBE_HV_TIMER_CHANNEL,
                   onTime);

    tim_generate_update(TUBE_HV_TIMER);
#else
    gpio_modify(TUBE_HV_PORT,
                TUBE_HV_PIN,
                tubeHardware.enabled);
#endif
}

void TUBE_DET_IRQ_HANDLER(void)
{
    exti_clear_pending_interrupt(TUBE_DET_PIN);

    // Pulse timer
    uint32_t countHigh1 = TUBE_DET_TIMER_SLAVE->CNT;
    uint32_t count = TUBE_DET_TIMER_MASTER->CNT;
    uint32_t countHigh2 = TUBE_DET_TIMER_SLAVE->CNT;

    if (countHigh1 == countHigh2)
        count |= countHigh1 << 16;
    else
    {
        if (count & 0x8000)
            count |= countHigh1 << 16;
        else
            count |= countHigh2 << 16;
    }

    tubeHardware.pulseTimeQueue[tubeHardware.pulseTimeQueueHead] = count;
    tubeHardware.pulseTimeQueueHead = (tubeHardware.pulseTimeQueueHead + 1) & TUBE_PULSE_QUEUE_MASK;
}

bool getTubePulseTime(uint32_t *pulseTime)
{
    if (tubeHardware.pulseTimeQueueHead == tubeHardware.pulseTimeQueueTail)
        return false;

    *pulseTime = tubeHardware.pulseTimeQueue[tubeHardware.pulseTimeQueueTail];
    tubeHardware.pulseTimeQueueTail = (tubeHardware.pulseTimeQueueTail + 1) & TUBE_PULSE_QUEUE_MASK;

    return true;
}

bool getTubeDet(void)
{
    return !gpio_get(TUBE_DET_PORT,
                     TUBE_DET_PIN);
}

#endif
