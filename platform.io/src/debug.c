/*
 * Rad Pro
 * Debugging
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "adc.h"
#include "buzzer.h"
#include "cstring.h"
#include "debug.h"
#include "display.h"
#include "events.h"
#include "keyboard.h"
#include "led.h"
#include "power.h"
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

    setBuzzer(false);
#if defined(PULSE_LED)
    setPulseLED(false);
#endif
    sleep(400);
}

void debugBit(bool value)
{
    setBuzzer(true);
#if defined(PULSE_LED)
    setPulseLED(true);
#endif
    if (value)
        debugWait(DEBUG_BIT0_LENGTH);
    else
        debugWait(DEBUG_BIT1_LENGTH);

    setBuzzer(false);
#if defined(PULSE_LED)
    setPulseLED(false);
#endif
    if (value)
        debugWait(DEBUG_BIT_LENGTH - DEBUG_BIT0_LENGTH);
    else
        debugWait(DEBUG_BIT_LENGTH - DEBUG_BIT1_LENGTH);
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

extern const uint8_t font_tiny5[];

void runTestMode(void)
{
    sleep(500);

    while (true)
    {
        char lines[8][32];

        strcpy(lines[0], "GPIOA: ");
        strcatUInt32Hex(lines[0], getGPIO(0));

        strcpy(lines[1], "GPIOB: ");
        strcatUInt32Hex(lines[1], getGPIO(1));

        strcpy(lines[2], "GPIOC: ");
        strcatUInt32Hex(lines[2], getGPIO(2));

        strcpy(lines[3], "GPIOD: ");
        strcatUInt32Hex(lines[3], getGPIO(3));

        strcpy(lines[4], "ADC: ");
        strcatUInt32Hex(lines[4], 0);

        strcpy(lines[5], "TIM1: ");
        strcatUInt32Hex(lines[5], 0);

        strcpy(lines[6], "TIM2: ");
        strcatUInt32Hex(lines[6], 0);

        strcpy(lines[7], "TIM3: ");
        strcatUInt32Hex(lines[7], 0);

        drawTestMode(lines);
        refreshDisplay();

        enableDisplay(true);
        setDisplayBacklight(true);
    }
}
