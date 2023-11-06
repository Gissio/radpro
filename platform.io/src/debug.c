/*
 * Rad Pro
 * Debugging
 *
 * (C) 2022-2023 Gissio
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
    uint32_t n = 200 * ms;

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

    setBuzzer(true);
    debugWait(onTime);
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

void debugTestMode(void)
{
    setDisplay(true);

    uint32_t state = 0;

    while (true)
    {
        triggerDisplay();

        clearDisplayBuffer();

        char line[128];
        uint32_t value;

        value = GPIOA_IDR;
        strcpy(line, "GPIOA: ");
        strcatUInt32Hex(line, value);
        drawTextLeft(line, font_tiny5, 2, 8);

        value = GPIOB_IDR;
        strcpy(line, "GPIOB: ");
        strcatUInt32Hex(line, value);
        drawTextLeft(line, font_tiny5, 66, 8);

        value = GPIOC_IDR;
        strcpy(line, "GPIOC: ");
        strcatUInt32Hex(line, value);
        drawTextLeft(line, font_tiny5, 2, 16);

        value = GPIOD_IDR;
        strcpy(line, "GPIOD: ");
        strcatUInt32Hex(line, value);
        drawTextLeft(line, font_tiny5, 66, 16);

        value = readADC(1, 0x7);
        strcpy(line, "ADC1: ");
        strcatUInt16Hex(line, value);
        drawTextLeft(line, font_tiny5, 2, 24);

        value = readADC(2, 0x7);
        strcpy(line, "ADC2: ");
        strcatUInt16Hex(line, value);
        drawTextLeft(line, font_tiny5, 66, 24);

        value = readADC(4, 0x7);
        strcpy(line, "ADC4: ");
        strcatUInt16Hex(line, value);
        drawTextLeft(line, font_tiny5, 2, 32);

        value = readADC(6, 0x7);
        strcpy(line, "ADC6: ");
        strcatUInt16Hex(line, value);
        drawTextLeft(line, font_tiny5, 66, 32);

        value = readADC(7, 0x7);
        strcpy(line, "ADC7: ");
        strcatUInt16Hex(line, value);
        drawTextLeft(line, font_tiny5, 2, 40);

        value = readADC(8, 0x7);
        strcpy(line, "ADC8: ");
        strcatUInt16Hex(line, value);
        drawTextLeft(line, font_tiny5, 66, 40);

        enum Event event = getKeyboardEvent();
        if (event == EVENT_UP)
        {
            state++;
            state &= 0b11;
        }

        strcpy(line, "PA11: ");
        if ((state & 0b10) == 0)
        {
            strcat(line, "0");

            gpio_clear(GPIOA, GPIO11);
        }
        else
        {
            strcat(line, "1");

            gpio_set(GPIOA, GPIO11);
        }
        drawTextLeft(line, font_tiny5, 2, 48);

        strcpy(line, "PB0: ");
        if ((state & 0b01) == 0)
        {
            strcat(line, "0");

            gpio_clear(GPIOB, GPIO0);
        }
        else
        {
            strcat(line, "1");

            gpio_set(GPIOB, GPIO0);
        }
        drawTextLeft(line, font_tiny5, 66, 48);

        sendDisplayBuffer();

        sleep(200);
    }
}
