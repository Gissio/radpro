/*
 * Rad Pro
 * STM32 power
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/adc.h"
#include "../peripherals/keyboard.h"
#include "../system/power.h"
#include "../stm32/device.h"

void initPower(void)
{
    // GPIO
    setPowerEnabled(false);

#if defined(PWR_VCC)
    gpio_set(PWR_VCC_PORT, PWR_VCC_PIN);
#endif

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(PWR_EN_PORT, PWR_EN_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PWR_EN_PORT, PWR_EN_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif

#if defined(PWR_VCC_PORT)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_output(PWR_VCC_PORT, PWR_VCC_PIN, GPIO_OUTPUTTYPE_PUSHPULL, GPIO_OUTPUTSPEED_2MHZ, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PWR_VCC_PORT, PWR_VCC_PIN, GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif
#endif

#if defined(PWR_USB_PORT)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
#if defined(PWR_USB_PULLUP)
    gpio_setup_input(PWR_USB_PORT, PWR_USB_PIN, GPIO_PULL_PULLUP);
#else
    gpio_setup_input(PWR_USB_PORT, PWR_USB_PIN, GPIO_PULL_FLOATING);
#endif
#elif defined(STM32F1)
#if defined(PWR_USB_PULLUP)
    gpio_setup(PWR_USB_PORT, PWR_USB_PIN, GPIO_MODE_INPUT_PULLUP);
#else
    gpio_setup(PWR_USB_PORT, PWR_USB_PIN, GPIO_MODE_INPUT_FLOATING);
#endif
#endif
#endif

#if defined(PWR_CHRG_PORT)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
#if defined(PWR_CHRG_PULLUP)
    gpio_setup_input(PWR_CHRG_PORT, PWR_CHRG_PIN, GPIO_PULL_PULLUP);
#else
    gpio_setup_input(PWR_CHRG_PORT, PWR_CHRG_PIN, GPIO_PULL_FLOATING);
#endif
#elif defined(STM32F1)
#if defined(PWR_CHRG_PULLUP)
    gpio_setup(PWR_CHRG_PORT, PWR_CHRG_PIN, GPIO_MODE_INPUT_PULLUP);
#else
    gpio_setup(PWR_CHRG_PORT, PWR_CHRG_PIN, GPIO_MODE_INPUT_FLOATING);
#endif
#endif
#endif

#if defined(PWR_STDBY_PORT)
#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)
    gpio_setup_input(PWR_STDBY_PORT, PWR_STDBY_PIN, GPIO_PULL_FLOATING);
#elif defined(STM32F1)
    gpio_setup(PWR_STDBY_PORT, PWR_STDBY_PIN, GPIO_MODE_INPUT_FLOATING);
#endif
#endif
}

void setPowerEnabled(bool value)
{
#if defined(PWR_EN_ACTIVE_LOW)
    gpio_modify(PWR_EN_PORT, PWR_EN_PIN, !value);
#else
    gpio_modify(PWR_EN_PORT, PWR_EN_PIN, value);
#endif
}

#if !defined(PWR_CHRG_CHANNEL)

bool isUSBPowered(void)
{
    bool value = false;
#if defined(PWR_CHRG_PORT)
    value |= isBatteryCharging();
#endif
#if defined(PWR_USB_PORT)
#if defined(PWR_USB_ACTIVE_LOW)
    return !gpio_get(PWR_USB_PORT, PWR_USB_PIN);
#else
    return gpio_get(PWR_USB_PORT, PWR_USB_PIN);
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

uint32_t getBatteryNum(void)
{
    return PWR_BAT_NUM;
}

#endif
