/*
 * FS2011 Pro
 * Geiger-Müller tube
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "gm.h"
#include "main.h"

struct GM
{
    uint32_t pulseCount;
} gm;

void initGM(void)
{
#ifndef SDL_MODE
    // High voltage
    rcc_periph_clock_enable(RCC_TIM3);

    gpio_mode_setup(GM_HV_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, GM_HV_PIN);
    gpio_set_output_options(GM_HV_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, GM_HV_PIN);
    gpio_set_af(GM_HV_PORT, GPIO_AF1, GM_HV_PIN);

    timer_set_period(TIM3, 105);
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_value(TIM3, TIM_OC1, 53);
    timer_enable_oc_output(TIM3, TIM_OC1);

    timer_enable_counter(TIM3);

    // GM detection
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GM_DET_PIN /*| GM_DET2_PIN*/);

    exti_select_source(EXTI6, GPIOA);
    exti_set_trigger(EXTI6, EXTI_TRIGGER_FALLING);
    exti_enable_request(EXTI6);

    nvic_set_priority(NVIC_EXTI4_15_IRQ, 255);
    nvic_enable_irq(NVIC_EXTI4_15_IRQ);
#endif
}

void exti4_15_isr(void)
{
#ifndef SDL_MODE
    exti_reset_request(EXTI6);
#endif

    gm.pulseCount++;
}

uint32_t getPulseCount(void)
{
    return gm.pulseCount;
}
