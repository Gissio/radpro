/*
 * Rad Pro
 * STM32 power
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../system/power.h"
#include "../stm32/device.h"

// pull: FLOATING or PULLUP
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
#define setupOutput(port, pin) \
    gpio_setup_output(port, pin, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING)
#define setupInput(port, pin, pull) \
    gpio_setup_input(port, pin, GPIO_PULL_##pull)
#elif defined(STM32F1)
#define setupOutput(port, pin) \
    gpio_setup(port, pin, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL)
#define setupInput(port, pin, pull) \
    gpio_setup(port, pin, GPIO_MODE_INPUT_##pull)
#else
#error "Unsupported STM32 family"
#endif

void initPower(bool value)
{
    setPowerEnabled(value);
    setupOutput(PWR_EN_PORT, PWR_EN_PIN);

#if defined(PWR_VCC_PORT)
    gpio_set(PWR_VCC_PORT, PWR_VCC_PIN);
    setupOutput(PWR_VCC_PORT, PWR_VCC_PIN);
#endif

#if defined(PWR_USB_PORT)
#if defined(PWR_USB_PULLUP)
    setupInput(PWR_USB_PORT, PWR_USB_PIN, PULLUP);
#else
    setupInput(PWR_USB_PORT, PWR_USB_PIN, FLOATING);
#endif
#endif

#if defined(PWR_CHRG_PORT)
    // With PWR_CHRG_CHANNEL, the pin is sampled by the ADC but must stay in
    // input mode: analog mode would disconnect the pull-up, which creates
    // the intermediate (battery-powered) level
#if defined(PWR_CHRG_PULLUP)
    setupInput(PWR_CHRG_PORT, PWR_CHRG_PIN, PULLUP);
#else
    setupInput(PWR_CHRG_PORT, PWR_CHRG_PIN, FLOATING);
#endif
#endif
}

void setPowerEnabled(bool value)
{
#if defined(PWR_EN_ACTIVE_LOW)
    value = !value;
#endif
    gpio_modify(PWR_EN_PORT, PWR_EN_PIN, value);
}

#if !defined(PWR_CHRG_CHANNEL)

bool isUSBPowered(void)
{
    bool value = isBatteryCharging();
#if defined(PWR_USB_PORT)
#if defined(PWR_USB_ACTIVE_LOW)
    value |= !gpio_get(PWR_USB_PORT, PWR_USB_PIN);
#else
    value |= gpio_get(PWR_USB_PORT, PWR_USB_PIN);
#endif
#endif
    return value;
}

bool isBatteryCharging(void)
{
#if defined(PWR_CHRG_PORT)
#if defined(PWR_CHRG_ACTIVE_LOW)
    return !gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN);
#else
    return gpio_get(PWR_CHRG_PORT, PWR_CHRG_PIN);
#endif
#else
    return false;
#endif
}

#endif

bool wasResetByWatchdog(void)
{
    return get_bits(RCC->CSR, RCC_CSR_IWDGRSTF);
}

void clearResetFlags(void)
{
    set_bits(RCC->CSR, RCC_CSR_RMVF);
}

#endif
