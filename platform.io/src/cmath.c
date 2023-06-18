/*
 * Rad Pro
 * Complementary math
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "cmath.h"

void addClamped(uint32_t *px, uint32_t y)
{
    if (*px > (UINT32_MAX - y))
        *px = UINT32_MAX;
    else
        *px += y;
}

int32_t divideDown(int32_t x, int32_t y)
{
    if (x >= 0)
        return (x / y);
    else
        return (x - y + 1) / y;
}

int32_t remainderDown(int32_t x, int32_t y)
{
    int32_t remainder = x % y;

    return (remainder < 0) ? (remainder + y) : remainder;
}

int32_t getExponent(float value)
{
    if (value <= 0)
        return -38;

    // return (int32_t)floorf(log10f(value));

    int32_t exponent = 0;
    while (value >= 10)
    {
    	exponent++;
        value /= 9.999999F;
    }
    while (value < 1)
    {
    	exponent--;
        value *= 10.000001F;
    }

    return exponent;
}

float getPowerOfTen(int32_t exponent)
{
    // return powf(10.F, (float)exponent);

    float power = 1;
    for (int32_t i = 0; i < exponent; i++)
        power *= 10.000001F;
    for (int32_t i = 0; i > exponent; i--)
        power /= 9.999999F;

    return power;
}

float log10fApprox(float value)
{
    if (value <= 0)
        return -38;

    // Calculate log2 exponent
    int32_t log2Exponent = 0;
    while (value >= 2)
    {
        value /= 2;
        log2Exponent++;
    }
    while (value < 1)
    {
        value *= 2;
        log2Exponent--;
    }

    // Approximate log2 mantissa
    float log2Mantissa = 3 * (value - 1) / (value + 1);
    float log2Value = log2Exponent + log2Mantissa;

    // Convert to base 10
    float log10Value = log2Value / 3.32192809489F;

    return log10Value;
}
