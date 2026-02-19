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
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/menu.h"
#include "../ui/rng.h"
#include "../ui/view.h"

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
} RNGMode;

#define RNG_IS_THROW(mode) ((mode) > RNG_MODE_BINARY)

static const Menu rngMenu;

static void pushRNGBit(bool bit);
static bool popRNGBit(void);
static uint32_t getRNGBitCount(void);

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
};

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

typedef struct {
    uint32_t data[RNG_STACK_UINT32NUM];
    uint32_t index;
} RNGStack;

typedef struct {
    uint32_t n;
    uint32_t v;
    uint32_t c;
} RNGFastDiceRoller;

typedef struct {
    char text[RNG_SYMBOLS_MAX + 1];
    uint8_t activity;
} RNGUI;

static struct
{
    uint32_t previousPulseCount;

    RNGStack stack;
    RNGFastDiceRoller fastDiceRoller;
    RNGMode mode;
    RNGUI ui;
} rng;

void setupRNG(void)
{
    selectMenuItem(&rngMenu, 0);

    rng.stack.index = 0;
}

void updateRNG(void)
{
    uint32_t pulseCount = tubePulseCount;
    uint32_t randomBits = tubeRandomBits;

    uint32_t bitCount = 2 * (pulseCount - rng.previousPulseCount);
    rng.previousPulseCount = pulseCount;

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
    if (rng.stack.index >= RNG_STACK_BITNUM)
        return;

    uint32_t uint32Index = rng.stack.index / 32;
    uint32_t bitIndex = rng.stack.index % 32;
    uint32_t mask = (uint32_t)1u << bitIndex;

    if (bit)
        rng.stack.data[uint32Index] |= mask;
    else
        rng.stack.data[uint32Index] &= ~mask;

    rng.stack.index++;
}

static bool popRNGBit(void)
{
    rng.stack.index--;

    uint32_t uint32Index = rng.stack.index / 32;
    uint32_t bitIndex = rng.stack.index % 32;

    return (rng.stack.data[uint32Index] >> bitIndex) & 0x1;
}

static uint32_t getRNGBitCount(void)
{
    return rng.stack.index;
}

// Random data interface

int32_t getRNGByte(void)
{
    if (getRNGBitCount() < 8)
        return -1;

    int32_t value = 0;
    for (uint32_t i = 0; i < 8; i++)
        value |= (uint32_t)popRNGBit() << i;

    return value;
}

// Fast Dice Roller algorithm: https://arxiv.org/abs/1304.1916

static int32_t getFastDiceRollerValue(void)
{
    while (getRNGBitCount())
    {
        bool bit = popRNGBit();

        rng.fastDiceRoller.v = rng.fastDiceRoller.v << 1;
        rng.fastDiceRoller.c = (rng.fastDiceRoller.c << 1) + bit;

        if (rng.fastDiceRoller.v >= rng.fastDiceRoller.n)
        {
            if (rng.fastDiceRoller.c < rng.fastDiceRoller.n)
            {
                uint32_t c = rng.fastDiceRoller.c;

                rng.fastDiceRoller.v = 1;
                rng.fastDiceRoller.c = 0;

                return c;
            }
            else
            {
                rng.fastDiceRoller.v = rng.fastDiceRoller.v - rng.fastDiceRoller.n;
                rng.fastDiceRoller.c = rng.fastDiceRoller.c - rng.fastDiceRoller.n;
            }
        }
    }

    return -1;
}

static void initFastDiceRoller(RNGMode mode)
{
    rng.mode = mode;
    rng.fastDiceRoller.n = rngModeRanges[mode];
    rng.fastDiceRoller.v = 1;
    rng.fastDiceRoller.c = 0;

    strclr(rng.ui.text);
    rng.ui.activity = 1;
}

static void updateFastDiceRollerText(void)
{
    while (rng.ui.activity)
    {
        int32_t value = getFastDiceRollerValue();
        if (value < 0)
            return;

        if (rng.mode == RNG_MODE_FULL_ASCII)
            strcatChar(rng.ui.text, '!' + value);
        else if (!RNG_IS_THROW(rng.mode))
        {
            char c;
            if (value < 10)
                c = '0' + value;
            else if (value < (10 + 26))
                c = 'a' + value - 10;
            else
                c = 'A' + value - (10 + 26);

            strcatChar(rng.ui.text, c);
        }
        else
        {
            if ((rng.mode != RNG_MODE_100SIDED_DIE) &&
                (rng.mode != RNG_MODE_10SIDED_DIE) &&
                (rng.mode != RNG_MODE_COIN_FLIP))
                value++;

            strcatUInt32(rng.ui.text, value, 0);
        }

        if (RNG_IS_THROW(rng.mode) || (strlen(rng.ui.text) >= RNG_SYMBOLS_MAX))
        {
            rng.ui.activity = 0;

#if defined(VOICE)
            if (RNG_IS_THROW(rng.mode))
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

static void onRNGViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        showRNGMenu();

        break;

    case EVENT_HEARTBEAT:
        if (rng.ui.activity)
        {
            rng.ui.activity++;
            if (rng.ui.activity > 3)
                rng.ui.activity = 1;
        }

        break;

    case EVENT_DRAW:
    {
        updateFastDiceRollerText();

        const char *stateString;
        if (rng.ui.activity)
            stateString = (const char *)(getString(STRING_ELLIPSIS)) + 3 - rng.ui.activity;
        else
            stateString = getString(STRING_EMPTY);

        drawRNG(getString(rngModeMenuOptions[rng.mode]),
                !rng.ui.activity && RNG_IS_THROW(rng.mode),
                rng.ui.text,
                stateString);

        break;
    }

    default:
        break;
    }
}

// RNG menu

static const char *onRNGMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(rngModeMenuOptions[index]);
}

static void onRNGMenuSelect(menu_size_t index)
{
    initFastDiceRoller(index);

    showView(onRNGViewEvent);
}

static MenuState rngMenuState;

static const Menu rngMenu = {
    getString(STRING_RANDOM_GENERATOR),
    &rngMenuState,
    ARRAY_SIZE(rngModeMenuOptions),
    onRNGMenuGetOption,
    onRNGMenuSelect,
    showSettingsMenu,
};

void showRNGMenu(void)
{
    showMenu(&rngMenu);
}
