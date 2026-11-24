/*
 * Rad Pro
 * H240-VER2.2 board services
 *
 * License: MIT
 */

#if defined(CA32) && defined(H240)

#include "ca32_gpio.h"

#include "../peripherals/adc.h"
#include "../peripherals/keyboard.h"
#include "../peripherals/tube.h"
#include "../system/power.h"

#define REG16(address) (*(volatile uint16_t *)(uintptr_t)(address))
#define REG32(address) (*(volatile uint32_t *)(uintptr_t)(address))

#define RCC_APB1ENR REG32(0x4002101c)
#define RCC_APB2ENR REG32(0x40021018)
#define RCC_TIM3_ENABLE (1u << 1)
#define RCC_TIM14_ENABLE (1u << 8)
#define RCC_ADC0_ENABLE (1u << 9)

#define TIM3_BASE 0x40000400u
#define TIM14_BASE 0x40002000u
#define TIM_CR1(base) REG16((base) + 0x00u)
#define TIM_DIER(base) REG16((base) + 0x0cu)
#define TIM_SR(base) REG16((base) + 0x10u)
#define TIM_EGR(base) REG16((base) + 0x14u)
#define TIM_CCMR2(base) REG16((base) + 0x1cu)
#define TIM_CCER(base) REG16((base) + 0x20u)
#define TIM_CNT(base) REG32((base) + 0x24u)
#define TIM_PSC(base) REG16((base) + 0x28u)
#define TIM_ARR(base) REG32((base) + 0x2cu)
#define TIM_CCR3(base) REG32((base) + 0x3cu)
#define TIM_BDTR(base) REG16((base) + 0x44u)
#define TIM_COUNTER_ENABLE 1u
#define TIM_UPDATE 1u
#define TIM_OC3_PWM1 (6u << 4)
#define TIM_CC3_ENABLE (1u << 8)
#define TIM_MAIN_OUTPUT_ENABLE (1u << 15)

#define ADC0_CONTROL REG32(0x40021418)
#define ADC0_DATA REG16(0x40021428)
#define ADC0_SAMPLE_CONTROL REG16(0x4002142c)
#define ADC0_CONFIGURATION 0x1009u
#define ADC0_CHANNEL_SHIFT 8
#define ADC0_START (1u << 5)
#define ADC0_COMPLETE (1u << 7)

#define NVIC_ISER REG32(0xe000e100)
#define NVIC_ICPR REG32(0xe000e280)
#define TIM3_IRQ (1u << 16)

enum
{
    PowerHoldPin = 0, // PB0, driven high by the factory startup
    ChargerStatusPin = 9, // PB9, active low
    TubeDetectorPin = 10, // PA10, active low
    TubeConverterPin = 15, // PA15, TIM14_CH3
    ADCBatteryChannel = 3, // PB5
    ADCUSBChannel = 4, // PB4
};

// Factory TIM3 setup at 0x08005648 uses an input divider of four and 15 kHz sampling.
#define SAMPLE_PRESCALER 4u
#define SAMPLE_FREQUENCY 15000u
#define SAMPLE_CLOCK (CA32_CORE_CLOCK_HZ / SAMPLE_PRESCALER)
#define SAMPLE_PERIOD ((SAMPLE_CLOCK + SAMPLE_FREQUENCY / 2u) / SAMPLE_FREQUENCY)
#define PULSE_CONFIRM_TICKS (SAMPLE_CLOCK / 100000u)
#define PULSE_REARM_SAMPLES 3u

#define CONVERTER_FREQUENCY 2000u
#define CONVERTER_PERIOD (CA32_CORE_CLOCK_HZ / CONVERTER_FREQUENCY)
#define CONVERTER_DUTY_DENOMINATOR 5u

static volatile bool pulseArmed;
static volatile uint8_t pulseCooldown;

static const uint8_t keyboardPins[KEY_NUM] = {
    [KEY_LEFT] = 14,
    [KEY_RIGHT] = 12,
    [KEY_UP] = 11,
    [KEY_DOWN] = 13,
    [KEY_OK] = 15,
};

void initKeyboardHardware(void)
{
    for (unsigned i = 0; i < KEY_NUM; i++)
        ca32GpioInput(CA32_GPIO_B, keyboardPins[i], true);
}

void onKeyboardTick(void)
{
    for (unsigned i = 0; i < KEY_NUM; i++)
        keyboardKeyDown[i] = (keyboardKeyDown[i] << 1) |
                             !ca32GpioRead(CA32_GPIO_B, keyboardPins[i]);
}

void updateKeyboardState(void)
{
}

void initPower(bool wasWatchdogReset)
{
    (void)wasWatchdogReset;
    ca32GpioOutput(CA32_GPIO_B, PowerHoldPin, true);
    ca32GpioInput(CA32_GPIO_B, ChargerStatusPin, true);
}

void setPowerEnabled(bool enabled)
{
    // shortcut: leave PB0 high during standby until its power routing is verified.
    if (enabled)
        ca32GpioWrite(CA32_GPIO_B, PowerHoldPin, true);
}

void initADC(void)
{
    RCC_APB2ENR |= RCC_ADC0_ENABLE;
    ca32GpioAnalog(CA32_GPIO_B, 5, 5);
    ca32GpioAnalog(CA32_GPIO_B, 4, 5);
    ADC0_SAMPLE_CONTROL = 0x3f0u;
}

static bool adcSample(uint16_t *sample)
{
    ADC0_CONTROL |= ADC0_START;
    for (unsigned wait = CA32_CORE_CLOCK_HZ / 500u; wait != 0; wait--)
    {
        if (ADC0_CONTROL & ADC0_COMPLETE)
        {
            ADC0_CONTROL |= ADC0_COMPLETE;
            *sample = ADC0_DATA >> 4;
            return true;
        }
    }
    return false;
}

static uint16_t readADC(unsigned channel)
{
    uint16_t sample;
    ADC0_CONTROL = ADC0_CONFIGURATION | (channel << ADC0_CHANNEL_SHIFT);
    // The factory code discards the first sample after selecting a channel.
    if (adcSample(&sample) && adcSample(&sample))
        return sample;
    return 0;
}

float readBatteryVoltage(void)
{
    uint16_t value = readADC(ADCBatteryChannel);
    if (!value)
        return 0.0f;

    // shortcut: map the factory gauge thresholds to nominal volts until the divider is measured.
    return 3.0f + ((float)value - 2208.0f) * (1.2f / 560.0f);
}

bool isUSBPowered(void)
{
    return readADC(ADCUSBChannel) > 2048;
}

bool isBatteryCharging(void)
{
    return !ca32GpioRead(CA32_GPIO_B, ChargerStatusPin) && isUSBPowered();
}

bool readTubeDet(void)
{
    return !ca32GpioRead(CA32_GPIO_A, TubeDetectorPin);
}

void initTubeHardware(void)
{
    ca32GpioOutput(CA32_GPIO_A, TubeConverterPin, false);
    ca32GpioInput(CA32_GPIO_A, TubeDetectorPin, true);

    RCC_APB1ENR |= RCC_TIM3_ENABLE;
    TIM_CR1(TIM3_BASE) = 0;
    TIM_PSC(TIM3_BASE) = SAMPLE_PRESCALER - 1u;
    TIM_ARR(TIM3_BASE) = SAMPLE_PERIOD - 1u;
    TIM_EGR(TIM3_BASE) = TIM_UPDATE;
    TIM_SR(TIM3_BASE) = (uint16_t)~TIM_UPDATE;
    TIM_DIER(TIM3_BASE) = TIM_UPDATE;
    NVIC_ICPR = TIM3_IRQ;
    NVIC_ISER = TIM3_IRQ;
}

static void setConverter(bool enabled)
{
#if defined(H240_ENABLE_HV)
    if (enabled)
    {
        // Factory 0x0800766c selects PA15 AF6 and 2 kHz at 20% duty.
        RCC_APB1ENR |= RCC_TIM14_ENABLE;
        TIM_CR1(TIM14_BASE) = 0;
        TIM_PSC(TIM14_BASE) = 0;
        TIM_ARR(TIM14_BASE) = CONVERTER_PERIOD - 1u;
        TIM_CCR3(TIM14_BASE) = CONVERTER_PERIOD / CONVERTER_DUTY_DENOMINATOR;
        TIM_CCMR2(TIM14_BASE) = TIM_OC3_PWM1;
        TIM_CCER(TIM14_BASE) = TIM_CC3_ENABLE;
        TIM_BDTR(TIM14_BASE) = TIM_MAIN_OUTPUT_ENABLE;
        TIM_EGR(TIM14_BASE) = TIM_UPDATE;
        TIM_CR1(TIM14_BASE) = TIM_COUNTER_ENABLE;
        ca32GpioAlternate(CA32_GPIO_A, TubeConverterPin, 6);
        return;
    }
#else
    (void)enabled;
#endif
    ca32GpioOutput(CA32_GPIO_A, TubeConverterPin, false);
#if defined(H240_ENABLE_HV)
    RCC_APB1ENR |= RCC_TIM14_ENABLE;
    TIM_CR1(TIM14_BASE) = 0;
    TIM_CCER(TIM14_BASE) = 0;
    TIM_BDTR(TIM14_BASE) = 0;
    RCC_APB1ENR &= ~RCC_TIM14_ENABLE;
#endif
}

void setTubeHVEnabled(bool enabled)
{
    TIM_CR1(TIM3_BASE) = 0;
    TIM_SR(TIM3_BASE) = (uint16_t)~TIM_UPDATE;
    NVIC_ICPR = TIM3_IRQ;
    pulseCooldown = 0;
    pulseArmed = !readTubeDet();

    setConverter(enabled);
    if (enabled)
        TIM_CR1(TIM3_BASE) = TIM_COUNTER_ENABLE;
}

static uint32_t sampleTicksSince(uint32_t start, uint32_t now)
{
    return now >= start ? now - start : SAMPLE_PERIOD - start + now;
}

void TIM3_IRQHandler(void)
{
    if (!(TIM_SR(TIM3_BASE) & TIM_UPDATE))
        return;
    TIM_SR(TIM3_BASE) = (uint16_t)~TIM_UPDATE;

    if (pulseCooldown)
        pulseCooldown--;
    if (!readTubeDet())
    {
        if (!pulseCooldown)
            pulseArmed = true;
        return;
    }
    if (!pulseArmed || pulseCooldown)
        return;

    uint32_t start = TIM_CNT(TIM3_BASE);
    for (unsigned wait = CA32_CORE_CLOCK_HZ / 100000u; wait != 0 && readTubeDet(); wait--)
    {
        if (sampleTicksSince(start, TIM_CNT(TIM3_BASE)) >= PULSE_CONFIRM_TICKS)
        {
            pulseArmed = false;
            pulseCooldown = PULSE_REARM_SAMPLES;
            tubePulseCount++;
            return;
        }
    }
}

#endif
