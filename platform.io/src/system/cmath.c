/*
 * Rad Pro
 * Math
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../system/cmath.h"

uint32_t addClamped(uint32_t x, uint32_t y)
{
    uint32_t value = x + y;

    return (value < x) ? UINT32_MAX : value;
}

// x must be finite and within int32_t range (float-to-int conversion is UB
// otherwise)
NOINLINE int32_t roundDown(float x)
{
    int32_t xi = (int32_t)x;

    if (x < (float)xi)
        xi--;

    return xi;
}

// Floor division (y > 0). Divides unsigned: avoids linking signed division
// on MCUs without a hardware divider
int32_t divideDown(int32_t x, int32_t y)
{
    if (x >= 0)
        return (uint32_t)x / (uint32_t)y;
    else
    {
        // Negates in unsigned arithmetic: well-defined for INT32_MIN
        uint32_t quotient = (0U - (uint32_t)x + (uint32_t)y - 1) / (uint32_t)y;

        return (int32_t)(0U - quotient);
    }
}

// 10^exponent for 0 <= exponent <= 9 (wraps above, 1 below)
NOINLINE uint32_t getDecimalPower(int32_t exponent)
{
    uint32_t result = 1;

    for (; exponent > 0; exponent--)
        result *= 10;

    return result;
}

// 10^exponent for |exponent| <= 9 (the n..G metric prefix range), correctly
// rounded: 10^9 is exact in float (powf via exp/ln is off by up to ~30 ulp)
float getDecimalPowerFloat(int32_t exponent)
{
    float result = getDecimalPower((exponent < 0) ? -exponent : exponent);

    return (exponent < 0) ? 1.0F / result : result;
}

NOINLINE int32_t getDecimalDigits(uint32_t value)
{
    int32_t digits = 0;

    do
    {
        digits++;
        value /= 10;
    } while (value);

    return digits;
}

// Rounds value to mantissa significant digits (half up; divisor == 1 leaves it
// unchanged). Overflows for values above ~3.8e9; callers keep value below 1e9
uint32_t truncateDecimalMantissa(uint32_t value, int32_t mantissa)
{
    uint32_t divisor = getDecimalPower(getDecimalDigits(value) - mantissa);

    return (value + divisor / 2) / divisor * divisor;
}

float getConfidenceInterval(uint32_t n)
{
    if (n <= 1)
        return 2.6888794F;
    if (n == 2)
        return 1.7858216F;

    // Normal approximation plus first order correction
    return 1.959964F / sqrtf(n) + 0.92095147F / (0.34074598F + n);
}

static uint32_t randomLFSR = 1;

bool getRandomBit(void)
{
    // https://en.wikipedia.org/wiki/Linear-feedback_shift_register#Galois_LFSRs
    int32_t lsb = randomLFSR & 1;

    randomLFSR = (randomLFSR >> 1) ^ (-lsb & 0xb400);

    return lsb;
}

uint32_t getGCD(uint32_t x, uint32_t y)
{
    // Greatest common divisor with Euclidean algorithm
    // https://en.wikipedia.org/wiki/Euclidean_algorithm#Implementations
    while (y > 0)
    {
        uint32_t t = y;
        y = x % y;
        x = t;
    }

    return x;
}
