/*
 * Rad Pro
 * STM32 Tube
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#if defined(STM32F0)
#include <libopencm3/stm32/syscfg.h>
#endif

#include "../events.h"
#include "../settings.h"
#include "../tube.h"

#include "stm32.h"

#define TUBE_PWM_LOW_FREQUENCY 1250
#define TUBE_PWM_LOW_FREQUENCY_PERIOD (TIM_FREQUENCY / TUBE_PWM_LOW_FREQUENCY)
#define TUBE_PWM_LOW_DUTYCYCLE_MULTIPLIER ((TUBE_PWM_LOW_FREQUENCY_PERIOD / 2) / TUBE_PWMDUTYCYCLE_NUM)

#define TUBE_PULSE_QUEUE_SIZE 64
#define TUBE_PULSE_QUEUE_MASK (TUBE_PULSE_QUEUE_SIZE - 1)

static struct
{
    bool enabled;

    volatile uint32_t pulseQueueHead;
    volatile uint32_t pulseQueueTail;
    volatile uint32_t pulseQueue[TUBE_PULSE_QUEUE_SIZE];
} tube;

void initTubeHardware(void)
{
    // GPIO

#if defined(STM32F0) || defined(STM32G0)

    gpio_mode_setup(TUBE_HV_PORT,
                    GPIO_MODE_AF,
                    GPIO_PUPD_NONE,
                    TUBE_HV_PIN);
    gpio_set_output_options(TUBE_HV_PORT,
                            GPIO_OTYPE_PP,
#if defined(STM32F0)
                            GPIO_OSPEED_HIGH,
#elif defined(STM32G0)
                            GPIO_OSPEED_VERYHIGH,
#endif
                            TUBE_HV_PIN);
    gpio_set_af(TUBE_HV_PORT,
                TUBE_HV_AF,
                TUBE_HV_PIN);

    gpio_mode_setup(TUBE_DET_PORT,
                    GPIO_MODE_INPUT,
                    GPIO_PUPD_PULLUP,
                    TUBE_DET_PIN);

#elif defined(STM32F1)

    gpio_set_mode(TUBE_HV_PORT,
                  GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                  TUBE_HV_PIN);

#if defined(TUBE_DET_PULLUP)
    gpio_set(TUBE_DET_PORT,
             TUBE_DET_PIN); // Pull-up
#endif
    gpio_set_mode(TUBE_DET_PORT,
                  GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_PULL_UPDOWN,
                  TUBE_DET_PIN);

#endif

    // PWM timer

    rcc_periph_clock_enable(TUBE_HV_TIMER_RCC);

    TUBE_HV_TIMER_CCMR(TUBE_HV_TIMER) |= TUBE_HV_TIMER_CCMR_MODE; // timer_set_oc_mode(TUBE_HV_TIMER, TIM_OC1, TIM_OCM_PWM1);

    updateTubeHV();

    TIM_CCER(TUBE_HV_TIMER) |= TUBE_HV_TIMER_CCER_CC; // timer_enable_oc_output(TUBE_HV_TIMER, TIM_OC1);
    TIM_CR1(TUBE_HV_TIMER) |= TIM_CR1_CEN;            // timer_enable_counter(TUBE_HV_TIMER);

    // Pulse timer (link TUBE_DET_TIMER_LOW with TUBE_DET_TIMER_HIGH)

    rcc_periph_clock_enable(TUBE_DET_TIMER_LOW_RCC);
    rcc_periph_clock_enable(TUBE_DET_TIMER_HIGH_RCC);

    TIM_CR2(TUBE_DET_TIMER_LOW) |= TIM_CR2_MMS_UPDATE; // timer_set_master_mode(TUBE_DET_TIMER, TIM_CR2_MMS_UPDATE);
    TIM_PSC(TUBE_DET_TIMER_LOW) = TIM_FREQUENCY / 8000000 - 1; // timer_set_prescaler(TUBE_DET_TIMER_LOW, TIM_FREQUENCY / 8000000 - 1);

    TIM_SMCR(TUBE_DET_TIMER_HIGH) |= TIM_SMCR_TS_ITR0;  // timer_slave_set_trigger(TUBE_DET_TIMER_HIGH, TIM_SMCR_TS_ITR0);
    TIM_SMCR(TUBE_DET_TIMER_HIGH) |= TIM_SMCR_SMS_ECM1; // timer_slave_set_mode(TUBE_DET_TIMER_HIGH, TIM_SMCR_SMS_ECM1);

    TIM_EGR(TUBE_DET_TIMER_LOW) |= TIM_EGR_UG; // timer_generate_event(TUBE_DET_TIMER, TIM_EGR_UG);

    TIM_CR1(TUBE_DET_TIMER_HIGH) |= TIM_CR1_CEN; // timer_enable_counter(TUBE_DET_TIMER_HIGH);
    TIM_CR1(TUBE_DET_TIMER_LOW) |= TIM_CR1_CEN;  // timer_enable_counter(TUBE_DET_TIMER);

    // EXTI

    TUBE_DET_EXTICR_REG = TUBE_DET_EXTICR_VALUE; // exti_select_source(TUBE_DET_EXTI, GPIOA);
    EXTI_FTSR |= TUBE_DET_EXTI;                  // exti_set_trigger(TUBE_DET_EXTI, EXTI_TRIGGER_FALLING);
    EXTI_IMR |= TUBE_DET_EXTI;                   // exti_enable_request(TUBE_DET_EXTI);

    nvic_enable_irq(TUBE_DET_EXTI_IRQ);

    // PWM debug test

#if defined(DEBUG_TEST_PWM)

    rcc_periph_clock_enable(RCC_TIM14);

    timer_set_period(TIM14, SYSTEM_FREQUENCY / 25000);
    timer_enable_update_event(TIM14);
    timer_enable_irq(TIM14, TIM_DIER_UIE);
    timer_enable_counter(TIM14);

    nvic_enable_irq(NVIC_TIM14_IRQ);

#endif
}

void setTubeHV(bool value)
{
    tube.enabled = value;

    updateTubeHV();
}

void updateTubeHV(void)
{
    uint32_t frequencyIndex;
    uint32_t dutyCycleIndex;

    switch (settings.tubeHVProfile)
    {
    case TUBE_HVPROFILE_FACTORY_DEFAULT:
        frequencyIndex = TUBE_FACTORYDEFAULT_HVFREQUENCY;
        dutyCycleIndex = TUBE_FACTORYDEFAULT_HVDUTYCYCLE;

        break;

    case TUBE_HVPROFILE_OPTIMIZED:
        frequencyIndex = TUBE_OPTIMIZED_HVFREQUENCY;
        dutyCycleIndex = TUBE_OPTIMIZED_HVDUTYCYCLE;

        break;

    case TUBE_HVPROFILE_ENERGY_SAVING:
        frequencyIndex = TUBE_ENERGYSAVING_HVFREQUENCY;
        dutyCycleIndex = TUBE_ENERGYSAVING_HVDUTYCYCLE;

        break;

    default:
        frequencyIndex = settings.tubePWMFrequency;
        dutyCycleIndex = settings.tubePWMDutyCycle;

        break;
    }

    // Sanity check

    if (frequencyIndex >= TUBE_PWMFREQUENCY_NUM)
        frequencyIndex = TUBE_PWMFREQUENCY_NUM - 1;
    if (dutyCycleIndex >= TUBE_PWMDUTYCYCLE_NUM)
        dutyCycleIndex = TUBE_PWMDUTYCYCLE_NUM - 1;

    // Set PWM frequency

    uint32_t arr = (TUBE_PWM_LOW_FREQUENCY_PERIOD >> frequencyIndex) - 1;
    TIM_ARR(TUBE_HV_TIMER) = arr; // timer_set_period(TUBE_HV_TIMER, arr);

    // Set PWM duty cycle

    uint32_t ccr;
    if (tube.enabled)
        ccr = (TUBE_PWM_LOW_FREQUENCY_PERIOD / 2 -
               TUBE_PWM_LOW_DUTYCYCLE_MULTIPLIER * dutyCycleIndex) >>
              frequencyIndex;
    else
        ccr = 0;
    TUBE_HV_TIMER_CCR(TUBE_HV_TIMER) = ccr; // timer_set_oc_value(TUBE_HV_TIMER, xxx, crr);
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

    // Links TUBE_DET_TIMER (16-bit) as LSB with TUBE_DET_TIMER_HIGH (16-bit) as MSB

    uint32_t countHigh1 = TIM_CNT(TUBE_DET_TIMER_HIGH); // timer_get_counter(TUBE_DET_TIMER_HIGH)
    uint32_t count = TIM_CNT(TUBE_DET_TIMER_LOW);       // timer_get_counter(TUBE_DET_TIMER)
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
    tube.pulseQueueHead = (tube.pulseQueueHead + 1) & TUBE_PULSE_QUEUE_MASK;
}

void syncTubeHV(void)
{

#if TIM_FREQUENCY == 8000000
    // while ((TIM_CNT(TUBE_HV_TIMER) & (~0b11)) != (20 & (~0b11)))
    //     ;
#elif TIM_FREQUENCY == 72000000
    // while ((TIM_CNT(TUBE_HV_TIMER) & (~0b11)) != (20 & (~0b11)))
    //     ;
#endif
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
