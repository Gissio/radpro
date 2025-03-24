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
#include "events.h"
#include "menu.h"
#include "rng.h"
#include "settings.h"

#define RNG_BITQUEUE_SIZE 128
#define RNG_BITQUEUE_MASK (RNG_BITQUEUE_SIZE - 1)
#define RNG_BITQUEUE_MAX_SIZE (RNG_BITQUEUE_SIZE - 1)
#define RNG_BITQUEUE_BYTE_NUM (RNG_BITQUEUE_SIZE / 8)

#define RNG_SYMBOLS_MAX 16

typedef enum
{
    RNG_MODE_FULL_ASCII,
    RNG_MODE_ALPHANUMERIC,
    RNG_MODE_HEXADECIMAL,
    RNG_MODE_DECIMAL,
    RNG_MODE_BINARY,

    RNG_MODE_100SIDED_DIE,
    RNG_MODE_20SIDED_DIE,
    RNG_MODE_12SIDED_DIE,
    RNG_MODE_10SIDED_DIE,
    RNG_MODE_8SIDED_DIE,
    RNG_MODE_6SIDED_DIE,
    RNG_MODE_4SIDED_DIE,

    RNG_MODE_COIN_FLIP,

    RNG_MODE_THROW = RNG_MODE_100SIDED_DIE,
} RNGMode;

static const uint8_t rngModeRanges[] = {94, 62, 16, 10, 2, 100, 20, 12, 10, 8, 6, 4, 2};

static const char *const rngModeMenuOptions[] = {
    "Full ASCII",
    "Alphanumeric",
    "Hexadecimal",
    "Decimal",
    "Binary",
    "100-sided die (0-99)",
    "20-sided die",
    "12-sided die",
    "10-sided die (0-9)",
    "8-sided die",
    "6-sided die",
    "4-sided die",
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
    uint32_t fastDiceRollerN;
    uint32_t fastDiceRollerV;
    uint32_t fastDiceRollerC;

    char text[RNG_SYMBOLS_MAX + 1];
    uint8_t activityIndicator;
} rng;

extern const View rngView;

extern const Menu rngMenu;

void resetRNG(void)
{
    selectMenuItem(&rngMenu,
                   0,
                   0);
}

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

        rng.fastDiceRollerV = rng.fastDiceRollerV << 1;
        rng.fastDiceRollerC = (rng.fastDiceRollerC << 1) + bit;

        if (rng.fastDiceRollerV >= rng.fastDiceRollerN)
        {
            if (rng.fastDiceRollerC < rng.fastDiceRollerN)
            {
                uint32_t c = rng.fastDiceRollerC;
                rng.fastDiceRollerV = 1;
                rng.fastDiceRollerC = 0;
                return c;
            }
            else
            {
                rng.fastDiceRollerV = rng.fastDiceRollerV - rng.fastDiceRollerN;
                rng.fastDiceRollerC = rng.fastDiceRollerC - rng.fastDiceRollerN;
            }
        }
    }
}

static void initFastDiceRoller(RNGMode mode)
{
    rng.mode = mode;
    rng.fastDiceRollerN = rngModeRanges[mode];
    rng.fastDiceRollerV = 1;
    rng.fastDiceRollerC = 0;

    strclr(rng.text);
    rng.activityIndicator = 1;
}

static void updateFastDiceRollerText(void)
{
    while (rng.activityIndicator)
    {
        int32_t digit = getFastDiceRollerInt();
        if (digit < 0)
            return;

        if (rng.mode == RNG_MODE_FULL_ASCII)
            strcatChar(rng.text, '!' + digit);
        else if (rng.mode < RNG_MODE_THROW)
        {
            char c;
            if (digit < 10)
                c = '0' + digit;
            else if (digit < (10 + 26))
                c = 'a' + digit - 10;
            else
                c = 'A' + digit - (10 + 26);

            strcatChar(rng.text, c);
        }
        else if (rng.mode != RNG_MODE_COIN_FLIP)
        {
            if ((rng.mode != RNG_MODE_100SIDED_DIE) &&
                (rng.mode != RNG_MODE_10SIDED_DIE))
                digit++;

            strcatUInt32(rng.text, digit, 0);
        }
        else
            strcpy(rng.text, digit ? "Head" : "Tail");

        if ((rng.mode >= RNG_MODE_THROW) ||
            (strlen(rng.text) >= RNG_SYMBOLS_MAX))
        {
            rng.activityIndicator = 0;
            triggerAlarm();
        }
    }
}

// RNG view

static void onRNGEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setView(&rngMenuView);

        break;

    case EVENT_PERIOD:
        if (rng.activityIndicator)
        {
            rng.activityIndicator++;
            if (rng.activityIndicator > 3)
                rng.activityIndicator = 1;
        }

        break;

    case EVENT_DRAW:
    {
        updateFastDiceRollerText();

        const char *stateString;
        if (rng.activityIndicator)
            stateString = rngActivitySubtitles[rng.activityIndicator - 1];
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

const Menu rngMenu = {
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
