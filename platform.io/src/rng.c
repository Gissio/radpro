/*
 * Rad Pro
 * Random number generator
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "cstring.h"
#include "display.h"
#include "menu.h"
#include "rng.h"
#include "settings.h"

#define RNG_BITQUEUE_SIZE 128
#define RNG_BITQUEUE_MASK (RNG_BITQUEUE_SIZE - 1)
#define RNG_BITQUEUE_MAX_SIZE (RNG_BITQUEUE_SIZE - 1)
#define RNG_BITQUEUE_BYTE_NUM (RNG_BITQUEUE_SIZE / 8)

#define RNG_TEXT_MAX_SIZE 16

typedef enum
{
    RNG_MODE_ALPHANUMERIC,
    RNG_MODE_FULL_ASCII,
    RNG_MODE_HEXADECIMAL,
    RNG_MODE_DECIMAL,
    RNG_MODE_20SIDED_DICE,
    RNG_MODE_12SIDED_DICE,
    RNG_MODE_8SIDED_DICE,
    RNG_MODE_6SIDED_DICE,
    RNG_MODE_4SIDED_DICE,
    RNG_MODE_COIN_FLIP,
} RNGMode;

static const uint8_t rngModeRanges[] = {62, 94, 16, 10, 20, 12, 8, 6, 4, 2};

static const char *const rngModeMenuOptions[] = {
    "Alphanumeric",
    "Full ASCII",
    "Hexadecimal",
    "Decimal",
    "20-sided dice",
    "12-sided dice",
    "8-sided dice",
    "6-sided dice",
    "4-sided dice",
    "Coin flip",
    NULL,
};

static const char *const rngActivitySubtitles[] = {
    ".",
    "..",
    "...",
};

static struct
{
    volatile uint32_t bitQueueHead;
    volatile uint32_t bitQueueTail;
    uint8_t bitQueue[RNG_BITQUEUE_BYTE_NUM];

    uint32_t pulseIndex;
    uint32_t pulseTimeSecondToLast;
    uint32_t lastPulseTimeCount;
    bool bitFlip;

    RNGMode mode;
    uint32_t fdrN;
    uint32_t fdrV;
    uint32_t fdrC;

    char text[RNG_TEXT_MAX_SIZE + 1];
    uint8_t activityIndicator;
} rng;

extern const View rngView;

static void enqueueBit(bool bit)
{
    uint32_t queueSize = (rng.bitQueueHead - rng.bitQueueTail) &
                         RNG_BITQUEUE_MASK;

    if (queueSize >= RNG_BITQUEUE_MAX_SIZE)
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

static uint32_t getQueueSize(void)
{
    return (rng.bitQueueHead - rng.bitQueueTail) & RNG_BITQUEUE_MASK;
}

void onRNGPulse(uint32_t pulseTime)
{
    switch (rng.pulseIndex)
    {
    case 0:
        rng.pulseTimeSecondToLast = pulseTime;
        rng.pulseIndex++;

        break;

    case 1:
        rng.lastPulseTimeCount = pulseTime;
        rng.pulseIndex++;

        break;

    case 2:
    {
        uint32_t duration1 = rng.lastPulseTimeCount - rng.pulseTimeSecondToLast;
        uint32_t duration2 = pulseTime - rng.lastPulseTimeCount;

        if (duration1 != duration2)
        {
            bool bit = rng.bitFlip ? duration1 > duration2 : duration1 < duration2;
            rng.bitFlip = !rng.bitFlip;

            enqueueBit(bit);
        }

        rng.pulseIndex = 1;
        rng.pulseTimeSecondToLast = pulseTime;

        break;
    }
    }
}

// Random data interface

int32_t getRandomData(void)
{
    if (getQueueSize() < 8)
        return -1;

    int32_t value = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        value <<= 1;
        value |= dequeueBit();
    }

    return value;
}

// Fast Dice Roller algorithm: https://arxiv.org/abs/1304.1916

static int32_t getFastDiceRollerInt(void)
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

static void initFastDiceRoller(RNGMode mode)
{
    rng.mode = mode;
    rng.fdrN = rngModeRanges[mode];
    rng.fdrV = 1;
    rng.fdrC = 0;

    strclr(rng.text);
    rng.activityIndicator = 1;
}

static char getFastDiceRollerChar(int32_t digit)
{
    if (digit < 10)
        return '0' + digit;
    else if (digit < (10 + 26))
        return 'a' + digit - 10;
    else
        return 'A' + digit - (10 + 26);
}

static void updateFastDiceRollerText(void)
{
    uint32_t index = (uint32_t)strlen(rng.text);

    while (true)
    {
        if (index >= RNG_TEXT_MAX_SIZE)
        {
            rng.activityIndicator = 0;
            return;
        }

        int32_t digit = getFastDiceRollerInt();
        if (digit == -1)
            return;

        char c;
        switch (rng.mode)
        {
        case RNG_MODE_ALPHANUMERIC:
        case RNG_MODE_HEXADECIMAL:
        case RNG_MODE_DECIMAL:
        case RNG_MODE_COIN_FLIP:
            c = getFastDiceRollerChar(digit);

            break;

        case RNG_MODE_FULL_ASCII:
            c = '!' + digit;

            break;

        case RNG_MODE_4SIDED_DICE:
        case RNG_MODE_6SIDED_DICE:
        case RNG_MODE_8SIDED_DICE:
        case RNG_MODE_12SIDED_DICE:
        case RNG_MODE_20SIDED_DICE:
            c = getFastDiceRollerChar(digit + 1);

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

// RNG menu

static const char *onRNGMenuGetOption(const Menu *menu,
                                      uint32_t index,
                                      MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return rngModeMenuOptions[index];
}

static void onRNGMenuSelect(const Menu *menu)
{
    initFastDiceRoller(menu->state->selectedIndex);

    setView(&rngView);
}

static MenuState rngMenuState;

static const Menu rngMenu = {
    "Random generator",
    &rngMenuState,
    onRNGMenuGetOption,
    onRNGMenuSelect,
    onSettingsSubMenuBack,
};

const View rngMenuView = {
    onMenuEvent,
    &rngMenu,
};

// RNG view

static void onRNGEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setView(&rngMenuView);

        break;

    case EVENT_DRAW:
    {
        updateFastDiceRollerText();

        const char *stateString;
        if (rng.activityIndicator)
        {
            stateString = rngActivitySubtitles[rng.activityIndicator - 1];
            rng.activityIndicator++;
            if (rng.activityIndicator > 3)
                rng.activityIndicator = 1;
        }
        else
            stateString = "";

        drawRNG(rngModeMenuOptions[rng.mode],
                rng.text,
                stateString);

        break;
    }

    default:
        break;
    }
}

const View rngView = {
    onRNGEvent,
    NULL,
};
