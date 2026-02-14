/*
 * Rad Pro
 * Random number generator
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>

#include "../extras/rng.h"
#include "../peripherals/tube.h"
#include "../peripherals/voice.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/menu.h"
#include "../ui/rng.h"

#define RNG_STACK_BITNUM 128
#define RNG_STACK_UINT32NUM (RNG_STACK_BITNUM / 32)

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

static Menu rngMenu;

static void pushRNGBit(bool bit);
static bool popRNGBit(void);
static uint32_t getRNGBitCount(void);

static const uint8_t rngModeRanges[] = {
    94,
    62,
    16,
    10,
    2,
    100,
    20,
    12,
    10,
    8,
    6,
    4,
    2,
};

static cstring rngModeMenuOptions[] = {
    STRING_ASCII,
    STRING_ALPHANUMERIC,
    STRING_HEXADECIMAL,
    STRING_DECIMAL,
    STRING_BINARY,
    STRING_100_SIDED_DIE,
    STRING_20_SIDED_DIE,
    STRING_12_SIDED_DIE,
    STRING_10_SIDED_DIE,
    STRING_8_SIDED_DIE,
    STRING_6_SIDED_DIE,
    STRING_4_SIDED_DIE,
    STRING_COIN_FLIP,
    NULL,
};

static struct
{
    uint32_t lastPulseCount;

    uint32_t stack[RNG_STACK_UINT32NUM];
    uint32_t stackIndex;

    RNGMode mode;
    uint32_t fastDiceRollerN;
    uint32_t fastDiceRollerV;
    uint32_t fastDiceRollerC;

    char text[RNG_SYMBOLS_MAX + 1];
    uint8_t activityState;
} rng;

void setupRNG(void)
{
    selectMenuItem(&rngMenu, 0, 0);

    rng.stackIndex = 0;
}

void updateRNG(void)
{
    uint32_t pulseCount = tubePulseCount;
    uint32_t randomBits = tubeRandomBits;

    uint32_t bitCount = 2 * (pulseCount - rng.lastPulseCount);
    rng.lastPulseCount = pulseCount;

    if (bitCount > 32)
        bitCount = 32;

    for (uint32_t i = 0; i < bitCount; i++)
    {
        pushRNGBit(randomBits & 0b1);
        randomBits >>= 1;
    }
}

// Stack

static void pushRNGBit(bool bit)
{
    if (rng.stackIndex >= RNG_STACK_BITNUM)
        return;

    uint32_t stackUint32Index = rng.stackIndex / 32;
    uint32_t stackBitIndex = rng.stackIndex % 32;

    rng.stack[stackUint32Index] = (rng.stack[stackUint32Index] &
                                   ~(1 << stackBitIndex)) |
                                  (bit << stackBitIndex);
    rng.stackIndex++;
}

static bool popRNGBit(void)
{
    rng.stackIndex--;
    uint32_t stackUint32Index = rng.stackIndex / 32;
    uint32_t stackBitIndex = rng.stackIndex % 32;
    return (rng.stack[stackUint32Index] >> stackBitIndex) & 0x1;
}

static uint32_t getRNGBitCount(void)
{
    return rng.stackIndex;
}

// Random data interface

int32_t getRNGByte(void)
{
    if (getRNGBitCount() < 8)
        return -1;

    int32_t value = 0;
    for (uint32_t i = 0; i < 8; i++)
    {
        value <<= 1;
        value |= popRNGBit();
    }

    return value;
}

// Fast Dice Roller algorithm: https://arxiv.org/abs/1304.1916

static int32_t getFastDiceRollerValue(void)
{
    while (true)
    {
        if (getRNGBitCount() == 0)
            return -1;

        bool bit = popRNGBit();

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
    rng.activityState = 1;
}

static void updateFastDiceRollerText(void)
{
    while (rng.activityState)
    {
        int32_t value = getFastDiceRollerValue();
        if (value < 0)
            return;

        if (rng.mode == RNG_MODE_FULL_ASCII)
            strcatChar(rng.text, '!' + value);
        else if (rng.mode < RNG_MODE_THROW)
        {
            char c;
            if (value < 10)
                c = '0' + value;
            else if (value < (10 + 26))
                c = 'a' + value - 10;
            else
                c = 'A' + value - (10 + 26);

            strcatChar(rng.text, c);
        }
        else
        {
            if ((rng.mode != RNG_MODE_100SIDED_DIE) &&
                (rng.mode != RNG_MODE_10SIDED_DIE) &&
                (rng.mode != RNG_MODE_COIN_FLIP))
                value++;

            strcatUInt32(rng.text, value, 0);
        }

        if ((rng.mode >= RNG_MODE_THROW) ||
            (strlen(rng.text) >= RNG_SYMBOLS_MAX))
        {
            rng.activityState = 0;

#if defined(VOICE)
            if (rng.mode >= RNG_MODE_THROW)
            {
                if (settings.alertVoice)
                    playNumber(value);
                else
                    triggerAlert(true);
            }
#else
            triggerAlert(true);
#endif
        }
    }
}

// RNG view

static void onRNGViewEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setView(&rngMenuView);

        break;

    case EVENT_HEARTBEAT:
        if (rng.activityState)
        {
            rng.activityState++;
            if (rng.activityState > 3)
                rng.activityState = 1;
        }

        break;

    case EVENT_DRAW:
    {
        updateFastDiceRollerText();

        const char *stateString;
        if (rng.activityState)
            stateString = (const char *)(getString(STRING_ELLIPSIS)) + 3 - rng.activityState;
        else
            stateString = getString(STRING_EMPTY);

        drawRNG(getString(rngModeMenuOptions[rng.mode]),
                !rng.activityState &&
                    (rng.mode >= RNG_MODE_THROW),
                rng.text,
                stateString);

        break;
    }

    default:
        break;
    }
}

static View rngView = {
    onRNGViewEvent,
    NULL,
};

// RNG menu

static const char *onRNGMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(rngModeMenuOptions[index]);
}

static void onRNGMenuSelect(uint32_t index)
{
    initFastDiceRoller(index);

    setView(&rngView);
}

static MenuState rngMenuState;

static Menu rngMenu = {
    getString(STRING_RANDOM_GENERATOR),
    &rngMenuState,
    onRNGMenuGetOption,
    onRNGMenuSelect,
    setSettingsMenu,
};

View rngMenuView = {
    onMenuEvent,
    &rngMenu,
};
