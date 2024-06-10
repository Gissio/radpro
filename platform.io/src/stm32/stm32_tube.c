/*
 * Rad Pro
 * STM32 Geiger-Müller tube
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../events.h"
#include "../settings.h"
#include "../tube.h"

#include "device.h"

#define TUBE_HV_LOW_FREQUENCY 1250
#define TUBE_HV_LOW_FREQUENCY_PERIOD (TIM_FREQUENCY / \
                                      TUBE_HV_LOW_FREQUENCY)
#define TUBE_HV_LOW_DUTYCYCLE_MULTIPLIER ((uint32_t)(TUBE_HVDUTYCYCLE_VALUE_STEP * \
                                                     TUBE_HV_LOW_FREQUENCY_PERIOD))

#define TUBE_PULSE_QUEUE_SIZE 64
#define TUBE_PULSE_QUEUE_MASK (TUBE_PULSE_QUEUE_SIZE - 1)

static struct
{
    bool enabled;

    volatile uint32_t pulseQueueHead;
    volatile uint32_t pulseQueueTail;
    volatile uint32_t pulseQueue[TUBE_PULSE_QUEUE_SIZE];
} tube;

void initTubeController(void)
{
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

    // Pulse timer
    tim_setup_linked(TUBE_DET_TIMER_MASTER,
                     TUBE_DET_TIMER_SLAVE,
                     TUBE_DET_TIMER_TRIGGER_CONNECTION);
    tim_set_prescaler_factor(TUBE_DET_TIMER_MASTER,
                             TIM_FREQUENCY / PULSE_MEASUREMENT_FREQUENCY);

    tim_enable(TUBE_DET_TIMER_SLAVE);
    tim_enable(TUBE_DET_TIMER_MASTER);

    // EXTI
    exti_setup(TUBE_DET_PORT,
               TUBE_DET_PIN,
               false,
               true);

    NVIC_SetPriority(TUBE_DET_IRQ, 0x00);
    NVIC_EnableIRQ(TUBE_DET_IRQ);

    exti_enable_interrupt(TUBE_DET_PIN);
}

void setTubeHV(bool value)
{
    tube.enabled = value;

    updateTubeHV();
}

void updateTubeHV(void)
{
#if defined(TUBE_HV_PWM)
    uuint32_t hvPeriod = TIM_FREQUENCY / getTubeHVFrequency();
    uuint32_t hvOnTime = tube.enabled
                             ? tube.hvPeriod * getTubeHVDutyCycle()
                             : 0;

    tim_set_period(TUBE_HV_TIMER,
                   hvPeriod);

    tim_set_ontime(TUBE_HV_TIMER,
                   TUBE_HV_TIMER_CHANNEL,
                   hvOnTime);
#else
    gpio_modify(TUBE_HV_PORT,
                TUBE_HV_PIN,
                tube.enabled);
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

    tube.pulseQueue[tube.pulseQueueHead] = count;
    tube.pulseQueueHead = (tube.pulseQueueHead + 1) & TUBE_PULSE_QUEUE_MASK;
}

bool getTubePulse(uint32_t *pulseTime)
{
    if (tube.pulseQueueHead == tube.pulseQueueTail)
        return false;

    *pulseTime = tube.pulseQueue[tube.pulseQueueTail];
    tube.pulseQueueTail = (tube.pulseQueueTail + 1) & TUBE_PULSE_QUEUE_MASK;

    return true;
}

bool getTubeDet(void)
{
    return !gpio_get(TUBE_DET_PORT,
                     TUBE_DET_PIN);
}

#endif
