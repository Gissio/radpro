/*
 * Rad Pro
 * STM32 power
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../adc.h"
#include "../keyboard.h"
#include "../power.h"

#include "device.h"

void initPower(void)
{
    // GPIO
    setPower(false);

#if defined(STM32F0) || defined(STM32G0) || defined(STM32L4)

    gpio_setup_output(PWR_EN_PORT,
                      PWR_EN_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);

#if defined(PWR_VCC_PORT)
    gpio_setup_output(PWR_VCC_PORT,
                      PWR_VCC_PIN,
                      GPIO_OUTPUTTYPE_PUSHPULL,
                      GPIO_OUTPUTSPEED_2MHZ,
                      GPIO_PULL_FLOATING);
#endif

#if defined(PWR_BAT_PORT)
    gpio_setup_analog(PWR_BAT_PORT,
                      PWR_BAT_PIN,
                      GPIO_PULL_FLOATING);
#endif

#if defined(PWR_USB_PORT)
    gpio_setup_input(PWR_USB_PORT,
                     PWR_USB_PIN,
                     GPIO_PULL_FLOATING);
#endif

#if defined(PWR_CHRG_PORT)

#if defined(PWR_CHRG_PULLUP)
    gpio_setup_input(PWR_CHRG_PORT,
                     PWR_CHRG_PIN,
                     GPIO_PULL_PULLUP);
#else
    gpio_setup_input(PWR_CHRG_PORT,
                     PWR_CHRG_PIN,
                     GPIO_PULL_FLOATING);
#endif

#endif

#if defined(PWR_STDBY_PORT)
    gpio_setup_input(PWR_STDBY_PORT,
                     PWR_STDBY_PIN,
                     GPIO_PULL_FLOATING);
#endif

#elif defined(STM32F1)

    gpio_setup(PWR_EN_PORT,
               PWR_EN_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);

#if defined(PWR_VCC_PORT)
    gpio_setup(PWR_VCC_PORT,
               PWR_VCC_PIN,
               GPIO_MODE_OUTPUT_2MHZ_PUSHPULL);
#endif

#if defined(PWR_BAT_PORT)
    gpio_setup(PWR_BAT_PORT,
               PWR_BAT_PIN,
               GPIO_MODE_INPUT_ANALOG);
#endif

#if defined(PWR_USB_PORT)
    gpio_setup(PWR_USB_PORT,
               PWR_USB_PIN,
               GPIO_MODE_INPUT_FLOATING);
#endif

#if defined(PWR_CHRG_PORT)

#if defined(PWR_CHRG_PULLUP)
    gpio_setup(PWR_CHRG_PORT,
               PWR_CHRG_PIN,
               GPIO_MODE_INPUT_PULLUP);
#else
    gpio_setup(PWR_CHRG_PORT,
               PWR_CHRG_PIN,
               GPIO_MODE_INPUT_FLOATING);
#endif

#endif

#if defined(PWR_STDBY_PORT)

#if defined(PWR_STDBY_PULLUP)
    gpio_setup(PWR_STDBY_PORT,
               PWR_STDBY_PIN,
               GPIO_MODE_INPUT_PULLUP);
#else
    gpio_setup(PWR_STDBY_PORT,
               PWR_STDBY_PIN,
               GPIO_MODE_INPUT_FLOATING);
#endif

#endif

#endif
}

void setPower(bool value)
{
#if defined(PWR_EN_ACTIVE_LOW)
    gpio_modify(PWR_EN_PORT,
                PWR_EN_PIN,
                !value);
#else
    gpio_modify(PWR_EN_PORT,
                PWR_EN_PIN,
                value);
#endif

#if defined(PWR_VCC_PORT)
    gpio_modify(PWR_VCC_PORT,
                PWR_VCC_PIN,
                value);
#endif
}

bool isPowerOnReset(void)
{
#if defined(STM32F0) || defined(STM32F1)
    bool isPowerOnReset = get_bits(RCC->CSR, RCC_CSR_PORRSTF_Msk);
#elif defined(STM32G0)
    bool isPowerOnReset = get_bits(RCC->CSR, RCC_CSR_PWRRSTF_Msk);
#elif defined(STM32L4)
    bool isPowerOnReset = get_bits(RCC->CSR, RCC_CSR_BORRSTF_Msk);
#endif
    set_bits(RCC->CSR, RCC_CSR_RMVF_Msk);

    return isPowerOnReset;
}

bool isBatteryCharging(void)
{
#if defined(PWR_CHRG_PORT)

#if defined(PWR_CHRG_ACTIVE_LOW)
    return !gpio_get(PWR_CHRG_PORT,
                     PWR_CHRG_PIN);
#else
    return gpio_get(PWR_CHRG_PORT,
                    PWR_CHRG_PIN);
#endif

#else
    return false;
#endif
}

bool isUSBPowered(void)
{
#if defined(PWR_USB_PORT)
    return gpio_get(PWR_USB_PORT,
                    PWR_USB_PIN);
#else
    return false;
#endif
}

uint32_t getBatteryNum(void)
{
    return PWR_BAT_NUM;
}

#endif
