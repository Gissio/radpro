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

#define RNG_POOL_BITNUM 128
#define RNG_UPDATE_PULSES_MAX (32 / TUBE_BITS_PER_PULSE)

#define RNG_TEXT_LENGTH_MAX 16

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

// Throws whose faces start at 0 (all others start at 1)
#define RNG_ZERO_BASED_THROWS ((1 << RNG_MODE_100SIDED_DIE) | \
                               (1 << RNG_MODE_10SIDED_DIE) |  \
                               (1 << RNG_MODE_COIN_FLIP))

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

static struct
{
    uint32_t previousPulseCount;

    // Bit pool (LIFO)
    uint32_t pool[RNG_POOL_BITNUM / 32];
    uint32_t poolBitCount;

    // Fast Dice Roller state
    RNGMode mode;
    uint32_t v;
    uint32_t c;

    // UI
    char text[RNG_TEXT_LENGTH_MAX + 1];
    uint8_t activity;
} rng;

// Bit pool

void updateRNG(void)
{
    // Read pulse count and random bits consistently (pulse IRQ may fire in between)
    uint32_t pulseCount;
    uint32_t randomBits;
    do
    {
        pulseCount = tubePulseCount;
        randomBits = tubeRandomBits;
    } while (pulseCount != tubePulseCount);

    uint32_t pulseNum = pulseCount - rng.previousPulseCount;
    rng.previousPulseCount = pulseCount;

    if (pulseNum > RNG_UPDATE_PULSES_MAX)
        pulseNum = RNG_UPDATE_PULSES_MAX;

    for (uint32_t bitNum = TUBE_BITS_PER_PULSE * pulseNum;
         bitNum && (rng.poolBitCount < RNG_POOL_BITNUM);
         bitNum--, randomBits >>= 1)
    {
        uint32_t *word = &rng.pool[rng.poolBitCount / 32];
        uint32_t shift = rng.poolBitCount % 32;

        *word = (*word & ~((uint32_t)1 << shift)) | ((randomBits & 1) << shift);
        rng.poolBitCount++;
    }
}

static uint32_t popRNGBit(void)
{
    rng.poolBitCount--;

    return (rng.pool[rng.poolBitCount / 32] >> (rng.poolBitCount % 32)) & 1;
}

int32_t getRNGByte(void)
{
    if (rng.poolBitCount < 8)
        return -1;

    int32_t value = 0;
    for (uint32_t i = 0; i < 8; i++)
        value |= popRNGBit() << i;

    return value;
}

// Fast Dice Roller algorithm: https://arxiv.org/abs/1304.1916

NOINLINE static void startRNG(RNGMode mode)
{
    rng.mode = mode;
    rng.v = 1;
    rng.c = 0;

    strclr(rng.text);
    rng.activity = 1;
}

static int32_t getFastDiceRollerValue(void)
{
    uint32_t n = rngModeRanges[rng.mode];

    while (rng.poolBitCount)
    {
        rng.v <<= 1;
        rng.c = (rng.c << 1) | popRNGBit();

        if (rng.v >= n)
        {
            if (rng.c < n)
            {
                int32_t value = rng.c;

                rng.v = 1;
                rng.c = 0;

                return value;
            }

            rng.v -= n;
            rng.c -= n;
        }
    }

    return -1;
}

static void updateRNGText(void)
{
    while (rng.activity)
    {
        int32_t value = getFastDiceRollerValue();
        if (value < 0)
            return;

        bool isThrow = RNG_IS_THROW(rng.mode);
        if (isThrow)
        {
            value += !((RNG_ZERO_BASED_THROWS >> rng.mode) & 1);

            strcatUInt32(rng.text, value, 0);
        }
        else
        {
            char c;
            if (rng.mode == RNG_MODE_FULL_ASCII)
                c = '!' + value;
            else if (value < 10)
                c = '0' + value;
            else if (value < (10 + 26))
                c = 'a' - 10 + value;
            else
                c = 'A' - (10 + 26) + value;

            strcatChar(rng.text, c);
        }

        if (isThrow || (strlen(rng.text) >= RNG_TEXT_LENGTH_MAX))
        {
            rng.activity = 0;

#if defined(VOICE)
            if (isThrow && settings.alertVoice)
                playNumber(value);
            else
#endif
                triggerNotificationAlert();
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

    case EVENT_KEY_SELECT:
        startRNG(rng.mode);

        break;

    case EVENT_HEARTBEAT:
        if (rng.activity)
            rng.activity = (rng.activity % 3) + 1;

        break;

    case EVENT_DRAW:
        updateRNGText();

        drawRNG(getString(rngModeMenuOptions[rng.mode]),
                !rng.activity && RNG_IS_THROW(rng.mode),
                rng.text,
                rng.activity
                    ? getString(STRING_ELLIPSIS) + (3 - rng.activity)
                    : getString(STRING_EMPTY));

        break;

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
    startRNG(index);

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

void resetRNG(void)
{
    selectMenuItem(&rngMenu, 0);

    rng.poolBitCount = 0;
}

void showRNGMenu(void)
{
    showMenu(&rngMenu);
}
