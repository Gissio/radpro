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
#include "../power.h"

#include "device.h"

void initPower(void)
{
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

#if defined(PWR_EXTERNAL_PORT)
    gpio_setup_input(PWR_EXTERNAL_PORT,
                     PWR_EXTERNAL_PIN,
#if defined(PWR_EXTERNAL_PULLUP)
                     GPIO_PULL_UP
#else
                     GPIO_PULL_FLOATING
#endif
    );
#endif

#if defined(PWR_CHRG_PORT)
    gpio_setup_input(PWR_CHRG_PORT,
                     PWR_CHRG_PIN,
#if defined(PWR_CHRG_PULLUP)
                     GPIO_PULL_UP
#else
                     GPIO_PULL_FLOATING
#endif
    );
#endif

#if defined(PWR_STDBY_PORT)
    gpio_setup_input(PWR_STDBY_PORT,
                     PWR_STDBY_PIN,
#if defined(PWR_STDBY_PULLUP)
                     GPIO_PULL_UP
#else
                     GPIO_PULL_FLOATING
#endif
    );
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

#if defined(PWR_EXTERNAL_PORT)
    gpio_setup(PWR_EXTERNAL_PORT,
               PWR_EXTERNAL_PIN,
#if defined(PWR_EXTERNAL_PULLUP)
               GPIO_MODE_INPUT_PULLUP
#else
               GPIO_MODE_INPUT_FLOATING
#endif
    );
#endif

#if defined(PWR_CHRG_PORT)
    gpio_setup(PWR_CHRG_PORT,
               PWR_CHRG_PIN,
#if defined(PWR_CHRG_PULLUP)
               GPIO_MODE_INPUT_PULLUP
#else
               GPIO_MODE_INPUT_FLOATING
#endif
    );
#endif

#if defined(PWR_STDBY_PORT)
    gpio_setup(PWR_STDBY_PORT,
               PWR_STDBY_PIN,
#if defined(PWR_STDBY_PULLUP)
               GPIO_MODE_INPUT_PULLUP
#else
               GPIO_MODE_INPUT_FLOATING
#endif
    );
#endif

#endif
}

void setPower(bool value)
{
    gpio_modify(PWR_EN_PORT,
                PWR_EN_PIN,
#if defined(PWR_EN_ACTIVE_LOW)
                !value
#else
                value
#endif
    );

#if defined(PWR_VCC_PORT)
    gpio_modify(PWR_VCC_PORT,
                PWR_VCC_PIN,
#if defined(PWR_VCC_ACTIVE_LOW)
                !value
#else
                value
#endif
    );
#endif
}

bool isDevicePowered(void)
{
#if defined(PWR_EXTERNAL_PORT)
    return
#if defined(PWR_EXTERNAL_ACTIVE_LOW)
        !
#endif
        gpio_get(PWR_EXTERNAL_PORT,
                 PWR_EXTERNAL_PIN);
#else
    return isBatteryCharging();
#endif
}

bool isBatteryCharging(void)
{
#if defined(PWR_CHRG_PORT)
    return
#if defined(PWR_CHRG_ACTIVE_LOW)
        !
#endif
        gpio_get(PWR_CHRG_PORT,
                 PWR_CHRG_PIN);
#else
    return false;
#endif
}

#endif
