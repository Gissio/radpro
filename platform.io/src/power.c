/*
 * Rad Pro
 * Power management
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef SDL_MODE

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#else

#include <stdio.h>

#endif

#include "buzzer.h"
#include "display.h"
#include "events.h"
#include "fs2011.h"
#include "measurements.h"
#include "power.h"
#include "settings.h"
#include "ui.h"

static void setPower(bool value);

void initPower(void)
{
#ifndef SDL_MODE
    // GPIO
    rcc_periph_clock_enable(RCC_GPIOB);

    gpio_mode_setup(PWR_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PWR_EN_PIN);

    setPower(true);
#endif
}

static void setPower(bool value)
{
#ifndef SDL_MODE
    if (value)
        gpio_set(PWR_EN_PORT, PWR_EN_PIN);
    else
        gpio_clear(PWR_EN_PORT, PWR_EN_PIN);
#else
    printf("Set power: %d\n", value);
#endif
}

void powerOff(void)
{
    writeState();

    // Disable devices
    setMeasurements(false);
    stopBacklightTimer();
    stopBuzzerTimer();
    setDisplay(false);
    setPower(false);

    while (getKeyEvent() != KEY_POWER)
        sleep(1);

    // Resume (when using external power supply)
    setPower(true);
    setDisplay(true);
    triggerBacklight();
    setMeasurements(true);
}

void powerOffLowBattery(void)
{
    clearDisplayBuffer();
    drawLowBattery();
    sendDisplayBuffer();

    sleep(5000);

    powerOff();

    updateView();
}
