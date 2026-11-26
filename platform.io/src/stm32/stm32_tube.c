/*
 * Rad Pro
 * STM32 Geiger-Müller tube hardware
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/tube.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../stm32/device.h"

// The detection pin must be an input: G0/L4 GPIOs reset to analog mode,
// which disables EXTI
#if defined(TUBE_DET_PULLUP)
#define TUBE_DET_PULL GPIO_PULL_PULLUP
#define TUBE_DET_MODE GPIO_MODE_INPUT_PULLUP
#else
#define TUBE_DET_PULL GPIO_PULL_FLOATING
#define TUBE_DET_MODE GPIO_MODE_INPUT_FLOATING
#endif

static struct
{
    bool enabled;

    uint16_t previousTimerCount;
} tubeHardware;

void initTubeHardware(void)
{
    // RCC
#if defined(TUBE_HV_PWM)
    rcc_enable_tim(TUBE_HV_TIMER);
#endif
    rcc_enable_tim(TUBE_DET_TIMER);

    // GPIO
#if defined(STM32F1)
#if defined(TUBE_HV_PWM)
    gpio_setup(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_MODE_OUTPUT_50MHZ_AF_PUSHPULL);
#else
    gpio_setup(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
    gpio_setup(TUBE_DET_PORT, TUBE_DET_PIN, TUBE_DET_MODE);
#else
#if defined(TUBE_HV_PWM)
    gpio_setup_af(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_50MHZ, GPIO_PULL_FLOATING, TUBE_HV_AF);
#else
    gpio_setup_output(TUBE_HV_PORT, TUBE_HV_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#endif
    gpio_setup_input(TUBE_DET_PORT, TUBE_DET_PIN, TUBE_DET_PULL);
#endif

    // HV PWM timer
#if defined(TUBE_HV_PWM)
    tim_setup_pwm(TUBE_HV_TIMER, TUBE_HV_TIMER_CHANNEL);

    updateTubeHV();

    tim_enable(TUBE_HV_TIMER);
#endif

    // Pulse detection timer (PSC is preloaded: set it before the update event)
    tim_set_prescaler_factor(TUBE_DET_TIMER, TUBE_DET_FREQUENCY / PULSE_MEASUREMENT_FREQUENCY);
    tim_setup_freerunning(TUBE_DET_TIMER);
    tim_enable(TUBE_DET_TIMER);

    // EXTI
    exti_setup(TUBE_DET_PORT, TUBE_DET_PIN, false, true);
    exti_enable_interrupt(TUBE_DET_PIN);

    nvic_set_priority(TUBE_DET_IRQ, 0x00);
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
    float periodFloat = TUBE_HV_FREQUENCY / getTubeHVFrequency();
    uint32_t period = (uint32_t)(periodFloat + 0.5F);
    uint32_t onTime = tubeHardware.enabled ? (uint32_t)(periodFloat * getTubeHVDutyCycle() + 0.5F) : 0;
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

    // Read and clear the overflow flag before sampling the counter, so a
    // wrap in between is caught by the next pulse
    uint32_t timerStatus = TUBE_DET_TIMER->SR;
    TUBE_DET_TIMER->SR = 0;
    uint32_t timerCount = TUBE_DET_TIMER->CNT;

    tubePulseCount++;

    tubeRandomBits = (tubeRandomBits << TUBE_BITS_PER_PULSE) | (timerCount & TUBE_BITS_PER_PULSE_MASK);

    // Without a 16-bit timer wrap since the previous pulse, the interval is
    // valid. Flash operations also set UIF (see stm32_flash.c), as they delay
    // this interrupt and its timestamp.
    if (!(timerStatus & TIM_SR_UIF))
    {
        uint16_t pulseInterval = timerCount - tubeHardware.previousTimerCount;
        if (pulseInterval < tubeDeadTime)
            tubeDeadTime = pulseInterval;
    }

    tubeHardware.previousTimerCount = timerCount;
}

bool readTubeDet(void)
{
    return !gpio_get(TUBE_DET_PORT, TUBE_DET_PIN);
}

#endif
