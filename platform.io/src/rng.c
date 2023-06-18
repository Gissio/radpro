/*
 * Rad Pro
 * Random number generator
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "display.h"
#include "rng.h"
#include "ui.h"

#define RNG_BITQUEUE_SIZE 512
#define RNG_BITQUEUE_MASK (RNG_BITQUEUE_SIZE - 1)
#define RNG_BITQUEUE_MAX (RNG_BITQUEUE_MASK - 1)
#define RNG_BITQUEUE_BYTENUM (RNG_BITQUEUE_SIZE >> 3)

#define RNG_TEXT_SIZE 16

struct
{
    volatile uint32_t bitQueueHead;
    volatile uint32_t bitQueueTail;
    uint8_t bitQueue[RNG_BITQUEUE_BYTENUM];

    uint8_t pulseIndex;
    uint32_t pulseTime1;
    uint32_t pulseTime2;
    bool bitFlip;

    enum RNGMode mode;
    uint32_t fdrN;
    uint32_t fdrV;
    uint32_t fdrC;

    char text[RNG_TEXT_SIZE + 1];
    uint8_t activityIndicator;
} rng;

const uint8_t rngModeRanges[] = {62, 94, 16, 10, 2, 6, 4, 8, 12, 20};
const char *const rngModeTitles[] = {
    "Letters & numbers",
    "Full ASCII",
    "Hexadecimal",
    "Decimal",
    "Coin toss",
    "6-sided dice",
    "4-sided dice",
    "8-sided dice",
    "12-sided dice",
    "20-sided dice",
};

const char *const rngActivitySubtitles[] = {
    ".",
    "..",
    "...",
};

static void enqueueBit(bool bit)
{
    uint32_t queueSize = (rng.bitQueueHead - rng.bitQueueTail) & RNG_BITQUEUE_MASK;

    if (queueSize >= RNG_BITQUEUE_MAX)
        return;

    uint32_t headByteIndex = rng.bitQueueHead >> 3;
    uint8_t headBitIndex = (rng.bitQueueHead & 0x7);

    rng.bitQueue[headByteIndex] = (rng.bitQueue[headByteIndex] &
                                   ~(1 << headBitIndex)) |
                                  (bit << headBitIndex);
    rng.bitQueueHead = (rng.bitQueueHead + 1) & RNG_BITQUEUE_MASK;
}

static int8_t dequeueBit(void)
{
    if (rng.bitQueueHead == rng.bitQueueTail)
        return -1;

    uint32_t tailByteIndex = (rng.bitQueueTail >> 3);
    uint32_t tailBitIndex = (rng.bitQueueTail & 0x7);

    bool bit = rng.bitQueue[tailByteIndex] & (1 << tailBitIndex);
    rng.bitQueueTail = (rng.bitQueueTail + 1) & RNG_BITQUEUE_MASK;

    return bit;
}

void onRNGPulse(uint32_t pulseTime)
{
    switch (rng.pulseIndex)
    {
    case 0:
        // Store first pulse
        rng.pulseTime1 = pulseTime;
        rng.pulseIndex++;

        break;

    case 1:
        // Store second pulse
        rng.pulseTime2 = pulseTime;
        rng.pulseIndex++;

        break;

    case 2:
    {
        // Calculate intervals between pulses
        uint32_t pulseInterval1 = rng.pulseTime2 - rng.pulseTime1;
        uint32_t pulseInterval2 = pulseTime - rng.pulseTime2;

        if (pulseInterval1 == pulseInterval2)
        {
            rng.pulseTime1 = rng.pulseTime2;
            break;
        }

        // Prepare bit
        bool bit;
        if (rng.bitFlip)
            bit = pulseInterval1 > pulseInterval2;
        else
            bit = pulseInterval1 < pulseInterval2;
        enqueueBit(bit);
        rng.bitFlip = !rng.bitFlip;

        // Prepare next cycle
        rng.pulseIndex = 1;
        rng.pulseTime1 = pulseTime;
        break;
    }
    }
}

// Fast Dice Roller algorithm: https://arxiv.org/abs/1304.1916
static int32_t getRNGInt(void)
{
    while (true)
    {
        int8_t bit = dequeueBit();
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

void resetRNG(enum RNGMode mode)
{
    rng.mode = mode;
    rng.fdrN = rngModeRanges[mode];
    rng.fdrV = 1;
    rng.fdrC = 0;

    rng.text[0] = '\0';
    rng.activityIndicator = 1;
}

static char getRNGChar(int32_t digit)
{
    if (digit < 10)
        return '0' + digit;
    else if (digit < (10 + 26))
        return 'a' + digit - 10;
    else
        return 'A' + digit - (10 + 26);
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
        case RNG_MODE_HEXADECIMAL:
        case RNG_MODE_DECIMAL:
        case RNG_MODE_COIN_TOSS:
            c = getRNGChar(digit);
            break;

        case RNG_MODE_FULL_ASCII:
            c = '!' + digit;
            break;

        case RNG_MODE_6SIDED_DICE:
        case RNG_MODE_4SIDED_DICE:
        case RNG_MODE_8SIDED_DICE:
        case RNG_MODE_12SIDED_DICE:
        case RNG_MODE_20SIDED_DICE:
            c = getRNGChar(digit + 1);
            break;

        default:
            c = '?';
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

    drawTitle(rngModeTitles[rng.mode]);

    if (rng.activityIndicator)
    {
        drawSubtitle(rngActivitySubtitles[rng.activityIndicator - 1]);
        rng.activityIndicator = (rng.activityIndicator % 3) + 1;
    }

    drawRNGText(rng.text);
}

void onRNGViewKey(KeyEvent keyEvent)
{
    if (keyEvent == KEY_BACK)
        setView(VIEW_MENU);
}
