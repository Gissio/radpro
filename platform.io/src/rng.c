/*
 * FS2011 Pro
 * Random number generator
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "display.h"
#include "main.h"
#include "rng.h"
#include "ui.h"

#define RNG_BITQUEUE_SIZE 256
#define RNG_BITQUEUE_MASK (RNG_BITQUEUE_SIZE - 1)
#define RNG_BITQUEUE_MAX (RNG_BITQUEUE_MASK - 1)
#define RNG_BITQUEUE_BYTENUM (RNG_BITQUEUE_SIZE >> 3)

#define RNG_TEXT_SIZE 40

struct RNG
{
    uint8_t initPulseNum;

    uint32_t lastPulseTime;
    uint32_t lastPeriod;

    uint8_t bitQueue[RNG_BITQUEUE_BYTENUM];
    volatile uint32_t bitQueueHead;
    volatile uint32_t bitQueueTail;

    bool bitFlip;

    enum RNGMode mode;

    uint32_t fdrN;
    uint32_t fdrV;
    uint32_t fdrC;

    char text[RNG_TEXT_SIZE + 1];
    uint8_t activityIndicator;
} rng;

void onRNGPulse(uint32_t pulseTime)
{
    uint32_t period = pulseTime - rng.lastPulseTime;

    if (rng.initPulseNum <= 1)
        rng.initPulseNum++;
    else if (period != rng.lastPeriod)
    {
        uint32_t queueSize = (rng.bitQueueHead - rng.bitQueueTail) & RNG_BITQUEUE_MASK;

        if ((queueSize < RNG_BITQUEUE_MAX) &&
            (period != rng.lastPeriod))
        {
            uint32_t headByteIndex = rng.bitQueueHead >> 3;
            uint8_t headBitIndex = (rng.bitQueueHead & 0x7);

            bool bit = (period > rng.lastPeriod);

            // Flip every second bit (bias filtering)
            if (rng.bitFlip)
                bit = !bit;
            rng.bitFlip = !rng.bitFlip;

            rng.bitQueue[headByteIndex] = (rng.bitQueue[headByteIndex] &
                                           ~(1 << headBitIndex)) |
                                          (bit << headBitIndex);
            rng.bitQueueHead = (rng.bitQueueHead + 1) & RNG_BITQUEUE_MASK;
        }
    }

    rng.lastPeriod = (pulseTime - rng.lastPulseTime);
    rng.lastPulseTime = pulseTime;
}

static int8_t getRNGBit(void)
{
    if (rng.bitQueueHead == rng.bitQueueTail)
        return -1;

    uint32_t tailByteIndex = (rng.bitQueueTail >> 3);
    uint32_t tailBitIndex = (rng.bitQueueTail & 0x7);

    bool bit = rng.bitQueue[tailByteIndex] & (1 << tailBitIndex);
    rng.bitQueueTail = (rng.bitQueueTail + 1) & RNG_BITQUEUE_MASK;

    return bit;
}

void resetRNG(enum RNGMode mode)
{
    rng.mode = mode;
    switch (mode)
    {
    case RNG_MODE_ALPHANUMERIC:
        rng.fdrN = 62;
        break;

    case RNG_MODE_FULL_ASCII:
        rng.fdrN = 94;
        break;

    case RNG_MODE_HEXADECIMAL:
        rng.fdrN = 16;
        break;

    case RNG_MODE_DECIMAL:
        rng.fdrN = 10;
        break;

    case RNG_MODE_6DICE:
        rng.fdrN = 6;
        break;

    case RNG_MODE_COIN_TOSS:
        rng.fdrN = 2;
        break;
    }

    rng.fdrV = 1;
    rng.fdrC = 0;

    rng.text[0] = '\0';
    rng.activityIndicator = 1;
}

// Fast Dice Roller algorithm: https://arxiv.org/abs/1304.1916
static int32_t getRNGInt(void)
{
    while (true)
    {
        int8_t bit = getRNGBit();
        if (bit == -1)
            return -1;

        rng.fdrV = rng.fdrV << 1;
        rng.fdrC = (rng.fdrC << 1) + bit;

        if (rng.fdrV >= rng.fdrN)
        {
            if (rng.fdrC < rng.fdrN)
            {
                uint32_t c = rng.fdrC;
                rng.fdrV = 1;
                rng.fdrC = 0;
                return c;
            }
            else
            {
                rng.fdrV = rng.fdrV - rng.fdrN;
                rng.fdrC = rng.fdrC - rng.fdrN;
            }
        }
    }
}

static void updateRNGText(void)
{
    uint32_t index = (uint32_t)strlen(rng.text);

    while (true)
    {
        if (index >= RNG_TEXT_SIZE)
        {
            rng.activityIndicator = 0;
            return;
        }

        int32_t digit = getRNGInt();
        if (digit == -1)
            return;

        char c;
        switch (rng.mode)
        {
        case RNG_MODE_ALPHANUMERIC:
            if (digit < 10)
                c = '0' + digit;
            else if (digit < (10 + 26))
                c = 'A' + digit - 10;
            else
                c = 'a' + digit - (10 + 26);
            break;

        case RNG_MODE_FULL_ASCII:
            c = '!' + digit;
            break;

        case RNG_MODE_HEXADECIMAL:
            if (digit < 10)
                c = '0' + digit;
            else
                c = 'a' + digit - 10;
            break;

        case RNG_MODE_DECIMAL:
            c = '0' + digit;
            break;

        case RNG_MODE_6DICE:
            c = '1' + digit;
            break;

        case RNG_MODE_COIN_TOSS:
            c = '0' + digit;
            break;

        default:
            c = ' ';
            break;
        }

        rng.text[index] = c;
        rng.text[index + 1] = '\0';

        index++;
    }
}

void drawRNGView(void)
{
    updateRNGText();

    switch (rng.mode)
    {
    case RNG_MODE_ALPHANUMERIC:
        drawTitle("Letters & numbers");
        break;

    case RNG_MODE_FULL_ASCII:
        drawTitle("Full ASCII");
        break;

    case RNG_MODE_HEXADECIMAL:
        drawTitle("Hexadecimal");
        break;

    case RNG_MODE_DECIMAL:
        drawTitle("Decimal");
        break;

    case RNG_MODE_6DICE:
        drawTitle("6-sided dice");
        break;

    case RNG_MODE_COIN_TOSS:
        drawTitle("Coin toss");
        break;
    }

    drawRNGText(rng.text);

    switch (rng.activityIndicator)
    {
    case 1:
        drawSubtitle(".");
        rng.activityIndicator = 2;
        break;

    case 2:
        drawSubtitle("..");
        rng.activityIndicator = 3;
        break;

    case 3:
        drawSubtitle("...");
        rng.activityIndicator = 1;
        break;
    }
}

void onRNGViewKey(KeyEvent keyEvent)
{
    if (keyEvent == KEY_BACK)
        setView(VIEW_MENU);
    else if (keyEvent == KEY_UP)
        rng.bitQueueTail = (rng.bitQueueTail + 1) & RNG_BITQUEUE_MASK;
}
