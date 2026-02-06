/*
 * Rad Pro
 * Math
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "../system/cmath.h"

uint32_t addClamped(uint32_t x, uint32_t y)
{
    uint32_t value = x + y;
    if (value < x)
        return UINT32_MAX;
    else
        return value;
}

int32_t roundDown(float x)
{
    int32_t xi = (int32_t)x;

    if (x < (float)xi)
        xi--;

    return xi;
}

int32_t divideDown(int32_t x, int32_t y)
{
    if (x >= 0)
        return x / y;
    else
        return -((-x + y - 1) / y);
}

uint32_t getDecimalPower(int32_t exponent)
{
    uint32_t result = 1;

    for (int32_t i = 0; i < exponent; i++)
        result *= 10;

    return result;
}

int32_t getDecimalDigits(uint32_t value)
{
    int32_t digits = 0;

    do
    {
        digits++;
        value /= 10;
    } while (value);

    return digits;
}

uint32_t truncateDecimalMantissa(uint32_t value, int32_t mantissa)
{
    int32_t digits = getDecimalDigits(value);
    uint32_t divisor = getDecimalPower(digits - mantissa);
    uint32_t remainder = value % divisor;
    value /= divisor;
    if (remainder >= (divisor / 2))
        value++;
    return value * divisor;
}

float getConfidenceInterval(uint32_t n)
{
    if (n <= 1)
        return 2.6888794F;
    else if (n == 2)
        return 1.7858216F;
    else
    {
        // Normal approximation
        float normalApproximation = 1.959964F / sqrtf(n);

        // First order correction
        return normalApproximation + 0.92095147F / (0.34074598F + n);
    }
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
