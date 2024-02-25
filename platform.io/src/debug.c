/*
 * Rad Pro
 * Debugging
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "adc.h"
#include "buzzer.h"
#include "cstring.h"
#include "debug.h"
#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "power.h"
#include "pulseled.h"
#include "settings.h"

#define DEBUG_BIT_LENGTH 100
#define DEBUG_BIT0_LENGTH 1
#define DEBUG_BIT1_LENGTH 15

void debugBeep(void)
{
    setBuzzer(true);
#if defined(PULSE_LED)
    setPulseLED(true);
#endif

    sleep(100);

#if defined(PULSE_LED)
    setPulseLED(false);
#endif
    setBuzzer(false);

    sleep(400);
}

void debugWait(uint32_t ms)
{
    uint32_t n = 9 * 200 * ms;

    for (uint32_t i = 0; i < n; i++)
        sleep(0);
}

void debugBit(bool value)
{
    uint32_t onTime;
    uint32_t offTime;

    switch (value)
    {
    case 0:
        onTime = DEBUG_BIT0_LENGTH;
        offTime = DEBUG_BIT_LENGTH - DEBUG_BIT0_LENGTH;

        break;

    case 1:
        onTime = DEBUG_BIT1_LENGTH;
        offTime = DEBUG_BIT_LENGTH - DEBUG_BIT1_LENGTH;

        break;
    }

#if defined(PULSE_LED)
    setPulseLED(true);
#endif
    setBuzzer(true);
    debugWait(onTime);
#if defined(PULSE_LED)
    setPulseLED(false);
#endif
    setBuzzer(false);
    debugWait(offTime);
}

void debugUInt32(uint32_t value)
{
    debugWait(500);

    for (uint32_t j = 0; j < 32; j++)
    {
        debugBit(value >> 31);
        value <<= 1;
    }
}

#if defined(STM32)

#include <libopencm3/stm32/gpio.h>

#elif defined(SDLSIM)

uint32_t GPIOA_IDR = 0x01234567;
uint32_t GPIOB_IDR = 0x89abcdef;
uint32_t GPIOC_IDR = 0x02468ace;
uint32_t GPIOD_IDR = 0x13579bdf;
#define GPIOA 0
#define GPIOB 0
#define GPIO0 0
#define GPIO11 0

void gpio_set(uint32_t a, uint32_t b)
{
}

void gpio_clear(uint32_t a, uint32_t b)
{
}

#endif

extern const uint8_t font_tiny5[];

void runTestMode(void)
{
    sleep(500);

    while (true)
    {
        char lines[8][32];

        strcpy(lines[0], "GPIOA: ");
        strcatUInt32Hex(lines[0], GPIOA_IDR);

        strcpy(lines[1], "GPIOB: ");
        strcatUInt32Hex(lines[1], GPIOB_IDR);

        strcpy(lines[2], "GPIOC: ");
        strcatUInt32Hex(lines[2], GPIOC_IDR);

        strcpy(lines[3], "GPIOD: ");
        strcatUInt32Hex(lines[3], GPIOD_IDR);

        strcpy(lines[4], "ADC: ");
        strcatUInt32Hex(lines[4], readADC(4, 7));

        strcpy(lines[5], "TIM1: ");
        strcatUInt32Hex(lines[5], 0);

        strcpy(lines[6], "TIM2: ");
        strcatUInt32Hex(lines[6], 0);

        strcpy(lines[7], "TIM3: ");
        strcatUInt32Hex(lines[7], 0);

        debugBeep();
        drawTestMode(lines);
        debugBeep();
        refreshDisplay();
        debugBeep();
        triggerDisplay();
#if defined(DISPLAY_MONOCHROME)
        setDisplay(true);
#endif

        debugBeep();
    }
}
