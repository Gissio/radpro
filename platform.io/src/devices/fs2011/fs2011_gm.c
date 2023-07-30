/*
 * Rad Pro
 * FS2011 Geiger-Müller tube interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <libopencm3/cm3/nvic.h>

#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "../../gm.h"
#include "fs2011.h"

#define GM_PULSE_PERIOD 106
#define GM_PULSE_PERIOD_HIGH 53

#define GM_PULSE_TIMES_NUM 16
#define GM_PULSE_TIMES_MASK (GM_PULSE_TIMES_NUM - 1)

static struct
{
    volatile uint8_t pulsesQueueHead;
    volatile uint8_t pulsesQueueTail;
    volatile uint32_t pulsesQueue[GM_PULSE_TIMES_NUM];
} gm;

void initGM(void)
{
    // Pulse timer
#ifdef STM32F0
    rcc_periph_clock_enable(RCC_TIM2);

    // Single TIM2 (32-bit)
    TIM_EGR(TIM2) |= TIM_EGR_UG;  // timer_generate_event(TIM2, TIM_EGR_UG);
    TIM_CR1(TIM2) |= TIM_CR1_CEN; // timer_enable_counter(TIM2);
#elif STM32F1
    rcc_periph_clock_enable(RCC_TIM1);
    rcc_periph_clock_enable(RCC_TIM2);

    // Link TIM1 (16-bit) as lsb with TIM2 (16-bit) as msb
    TIM_CR2(TIM1) |= TIM_CR2_MMS_UPDATE; // timer_set_master_mode(TIM1, TIM_CR2_MMS_UPDATE);

    TIM_SMCR(TIM2) |= TIM_SMCR_TS_ITR0; // timer_slave_set_trigger(TIM2, TIM_SMCR_TS_ITR0);
   	TIM_SMCR(TIM2) |= TIM_SMCR_SMS_ECM1; // timer_slave_set_mode(TIM2, TIM_SMCR_SMS_ECM1);

	TIM_EGR(TIM1) |= TIM_EGR_UG; // timer_generate_event(TIM1, TIM_EGR_UG);

    TIM_CR1(TIM2) |= TIM_CR1_CEN; // timer_enable_counter(TIM2);
    TIM_CR1(TIM1) |= TIM_CR1_CEN; // timer_enable_counter(TIM1);
#endif

    // High voltage generation
    rcc_periph_clock_enable(RCC_TIM3);

#ifdef STM32F0
    gpio_mode_setup(GM_HV_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GM_HV_PIN);
    gpio_set_output_options(GM_HV_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, GM_HV_PIN);
    gpio_set_af(GM_HV_PORT, GPIO_AF1, GM_HV_PIN);
#elif STM32F1
    gpio_set_mode(GM_HV_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GM_HV_PIN);
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_FULL_SWJ, AFIO_MAPR_TIM3_REMAP_PARTIAL_REMAP);
#endif

    TIM_ARR(TIM3) = GM_PULSE_PERIOD - 1;    // timer_set_period(TIM3, GM_PULSE_PERIOD - 1);
    TIM_CCMR1(TIM3) |= TIM_CCMR1_OC1M_PWM1; // timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
    TIM_CCR1(TIM3) = GM_PULSE_PERIOD_HIGH;  // timer_set_oc_value(TIM3, TIM_OC1, GM_PULSE_PERIOD_HIGH);
    TIM_CCER(TIM3) |= TIM_CCER_CC1E;        // timer_enable_oc_output(TIM3, TIM_OC1);
    TIM_CR1(TIM3) |= TIM_CR1_CEN;           // timer_enable_counter(TIM3);

    // GM detection
#ifdef STM32F0
    gpio_mode_setup(GM_DET_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GM_DET_PIN /*| GM_DET2_PIN*/);
#elif STM32F1
    gpio_set_mode(GM_DET_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GM_DET_PIN /*| GM_DET2_PIN*/);
    gpio_set(GM_DET_PORT, GM_DET_PIN); // Pull-up
#endif

    exti_select_source(EXTI6, GPIOA);
    exti_set_trigger(EXTI6, EXTI_TRIGGER_FALLING);
    exti_enable_request(EXTI6);

#ifdef STM32F0
    nvic_enable_irq(NVIC_EXTI4_15_IRQ);
#elif STM32F1
    nvic_enable_irq(NVIC_EXTI9_5_IRQ);
#endif
}

#ifdef STM32F0
void exti4_15_isr(void)
#elif STM32F1
void exti9_5_isr(void)
#endif
{
    EXTI_PR = EXTI6; // exti_reset_request(EXTI6);

#ifdef STM32F0
    gm.pulsesQueue[gm.pulsesQueueHead] = TIM_CNT(TIM2); // timer_get_counter(TIM2)
#elif STM32F1
    // Link TIM1 (16-bit) as lsb with TIM2 (16-bit) as msb
    uint32_t countH1 = TIM_CNT(TIM2); // timer_get_counter(TIM2)
    uint32_t count = TIM_CNT(TIM1);   // timer_get_counter(TIM1)
    uint32_t countH2 = TIM_CNT(TIM2); // timer_get_counter(TIM2)

    if (countH1 == countH2)
        count |= countH1 << 16;
    else
    {
        if (count >= 0x8000)
            count |= countH1 << 16;
        else
            count |= countH2 << 16;
    }

    gm.pulsesQueue[gm.pulsesQueueHead] = count;
#endif
    gm.pulsesQueueHead = (gm.pulsesQueueHead + 1) & GM_PULSE_TIMES_MASK;
}

void syncGMHVPulse(void)
{
    // Assumes 'while' takes <= 4 cycles:
    while ((TIM_CNT(TIM3) & (~3)) != (20 & (~3)))
        ;
}

bool getGMPulse(uint32_t *pulseTime)
{
    if (gm.pulsesQueueHead == gm.pulsesQueueTail)
        return false;

    *pulseTime = gm.pulsesQueue[gm.pulsesQueueTail];
    gm.pulsesQueueTail = (gm.pulsesQueueTail + 1) & GM_PULSE_TIMES_MASK;

    return true;
}

#endif
