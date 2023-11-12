/*
 * Rad Pro
 * STM32 Tube
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "../events.h"
#include "../settings.h"
#include "../tube.h"

#include "stm32.h"

#define TUBE_HV_PWM_FREQUENCY 1250
#define TUBE_HV_PWM_PERIOD (SYS_FREQUENCY / TUBE_HV_PWM_FREQUENCY)
#define TUBE_HV_PWM_PERIOD_2 (TUBE_HV_PWM_PERIOD / 2)

#define TUBE_PULSE_QUEUE_SIZE 64
#define TUBE_PULSE_QUEUE_MASK (TUBE_PULSE_QUEUE_SIZE - 1)

static struct
{
    volatile uint32_t pulseQueueHead;
    volatile uint32_t pulseQueueTail;
    volatile uint32_t pulseQueue[TUBE_PULSE_QUEUE_SIZE];
} tube;

void initTubeHardware(void)
{
    // High voltage generator

    rcc_periph_clock_enable(TUBE_HV_TIMER_RCC);

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(TUBE_HV_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TUBE_HV_PIN);
    gpio_set_output_options(TUBE_HV_PORT, GPIO_OTYPE_PP, TUBE_HV_PORT_SPEED, TUBE_HV_PIN);
    gpio_set_af(TUBE_HV_PORT, TUBE_HV_AF, TUBE_HV_PIN);

#elif defined(STM32F1)

    gpio_set_mode(TUBE_HV_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, TUBE_HV_PIN);

#endif

    TIM_CCMR1(TUBE_HV_TIMER) |= TIM_CCMR1_OC1M_PWM1; // timer_set_oc_mode(TUBE_HV_TIMER, TIM_OC1, TIM_OCM_PWM1);
    TIM_CCER(TUBE_HV_TIMER) |= TIM_CCER_CC1E;        // timer_enable_oc_output(TUBE_HV_TIMER, TIM_OC1);

    updateTubeHV();

    TIM_CR1(TUBE_HV_TIMER) |= TIM_CR1_CEN; // timer_enable_counter(TUBE_HV_TIMER);

    // Pulse detection

#if defined(TUBE_DET_TIMER_HIGH)

    // Link TUBE_DET_TIMER (16-bit LSB) with TUBE_DET_TIMER_HIGH (16-bit MSB)

    rcc_periph_clock_enable(TUBE_DET_TIMER_RCC);
    rcc_periph_clock_enable(TUBE_DET_TIMER_HIGH_RCC);

    TIM_CR2(TUBE_DET_TIMER) |= TIM_CR2_MMS_UPDATE; // timer_set_master_mode(TUBE_DET_TIMER, TIM_CR2_MMS_UPDATE);

    TIM_SMCR(TUBE_DET_TIMER_HIGH) |= TIM_SMCR_TS_ITR0;  // timer_slave_set_trigger(TUBE_DET_TIMER_HIGH, TIM_SMCR_TS_ITR0);
    TIM_SMCR(TUBE_DET_TIMER_HIGH) |= TIM_SMCR_SMS_ECM1; // timer_slave_set_mode(TUBE_DET_TIMER_HIGH, TIM_SMCR_SMS_ECM1);

    TIM_EGR(TUBE_DET_TIMER) |= TIM_EGR_UG; // timer_generate_event(TUBE_DET_TIMER, TIM_EGR_UG);

    TIM_CR1(TUBE_DET_TIMER_HIGH) |= TIM_CR1_CEN; // timer_enable_counter(TUBE_DET_TIMER_HIGH);
    TIM_CR1(TUBE_DET_TIMER) |= TIM_CR1_CEN;      // timer_enable_counter(TUBE_DET_TIMER);

#else

    // Single TUBE_DET_TIMER (32-bit)

    rcc_periph_clock_enable(TUBE_DET_TIMER_RCC);

    TIM_EGR(TUBE_DET_TIMER) |= TIM_EGR_UG;  // timer_generate_event(TUBE_DET_TIMER, TIM_EGR_UG);
    TIM_CR1(TUBE_DET_TIMER) |= TIM_CR1_CEN; // timer_enable_counter(TUBE_DET_TIMER);

#endif

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(TUBE_DET_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, TUBE_DET_PIN);

#elif defined(STM32F1)

    gpio_set_mode(TUBE_DET_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, TUBE_DET_PIN);
    gpio_set(TUBE_DET_PORT, TUBE_DET_PIN); // Pull-up

#endif

    exti_select_source(TUBE_DET_EXTI, GPIOA);
    exti_set_trigger(TUBE_DET_EXTI, EXTI_TRIGGER_FALLING);
    exti_enable_request(TUBE_DET_EXTI);

    nvic_enable_irq(TUBE_DET_EXTI_IRQ);

#if defined(DEBUG_TEST_PWM)

    rcc_periph_clock_enable(RCC_TIM14);

    timer_set_period(TIM14, SYS_FREQUENCY / 25000);
    timer_enable_update_event(TIM14);
    timer_enable_irq(TIM14, TIM_DIER_UIE);
    timer_enable_counter(TIM14);

    nvic_enable_irq(NVIC_TIM14_IRQ);

#endif
}

void updateTubeHV(void)
{
    // Sanity check

    if (settings.tubeHVDutyCycle >= TUBE_HV_DUTY_CYCLE_NUM)
        settings.tubeHVDutyCycle = TUBE_HV_DUTY_CYCLE_NUM - 1;

    // Frequency

    uint32_t arr = (TUBE_HV_PWM_PERIOD >> settings.tubeHVFrequency) - 1;
    TIM_ARR(TUBE_HV_TIMER) = arr; // timer_set_period(TUBE_HV_TIMER, arr);

    // Duty cycle

    uint32_t ccr = (TUBE_HV_PWM_PERIOD_2 -
                    (settings.tubeHVDutyCycle << 5)) >>
                   settings.tubeHVFrequency;
    TIM_CCR1(TUBE_HV_TIMER) = ccr; // timer_set_oc_value(TUBE_HV_TIMER, TIM_OC1, crr);
}

#if defined(DEBUG_TEST_PWM)

void tim14_isr(void)
{
    TIM_SR(TIM14) &= ~TIM_SR_UIF;

#else

void TUBE_DET_IRQ_HANDLER(void)
{
#if defined(EXTI_FPR1)

    EXTI_FPR1 = TUBE_DET_EXTI; // exti_reset_request();

#else

    EXTI_PR = TUBE_DET_EXTI; // exti_reset_request();

#endif

#endif

#if defined(TUBE_DET_TIMER_HIGH)

    // Link TUBE_DET_TIMER (16-bit) as LSB with TUBE_DET_TIMER_HIGH (16-bit) as MSB

    uint32_t countHigh1 = TIM_CNT(TUBE_DET_TIMER_HIGH); // timer_get_counter(TUBE_DET_TIMER_HIGH)
    uint32_t count = TIM_CNT(TUBE_DET_TIMER);           // timer_get_counter(TUBE_DET_TIMER)
    uint32_t countHigh2 = TIM_CNT(TUBE_DET_TIMER_HIGH); // timer_get_counter(TUBE_DET_TIMER_HIGH)

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

#else

    tube.pulseQueue[tube.pulseQueueHead] = TIM_CNT(TUBE_DET_TIMER); // timer_get_counter(TUBE_DET_TIMER)

#endif

    tube.pulseQueueHead = (tube.pulseQueueHead + 1) & TUBE_PULSE_QUEUE_MASK;
}

void syncHVPulse(void)
{
    // Assume 'while' takes <= 4 cycles, mask with 0b11

    while ((TIM_CNT(TUBE_HV_TIMER) & (~0b11)) != (20 & (~0b11)))
        ;
}

bool getTubePulse(uint32_t *pulseTime)
{
    if (tube.pulseQueueHead == tube.pulseQueueTail)
        return false;

    *pulseTime = tube.pulseQueue[tube.pulseQueueTail];
    tube.pulseQueueTail = (tube.pulseQueueTail + 1) & TUBE_PULSE_QUEUE_MASK;

    return true;
}

#endif
