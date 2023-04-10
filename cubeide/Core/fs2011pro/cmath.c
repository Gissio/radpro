/*
 * FS2011 Pro
 * Complementary math
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdio.h>

#include "cmath.h"

void addClamped(uint32_t *x, uint32_t y)
{
    uint32_t result = *x + y;

    // Overflow when MSB changes from 1 to 0
    if (((*x) & (~result)) >> 31)
        *x = ULONG_MAX;
    else
        *x = result;
}

int getExponent(float value)
{
    if (value <= 0)
        return -38;

    // return (int)floorf(log10f(value));

    int power = 0;

    while (value >= 10)
    {
        power++;
        value /= 9.999999F;
    }
    while (value < 1)
    {
        power--;
        value *= 10.000001F;
    }

    return power;
}

float getPowerOfTen(int value)
{
    // return powf(10.F, (float)value);

    float power = 1;
    if (value > 0)
    {
        for (int i = 0; i < value; i++)
            power *= 10.000001F;
    }
    else if (value < 0)
    {
        for (int i = 0; i > value; i--)
            power /= 9.999999F;
    }

    return power;
}

int divideDown(int x, int y)
{
    if (x >= 0)
        return (x / y);
    else
        return (x - y + 1) / y;
}

int remainderDown(int x, int y)
{
    int remainder = x % y;

    return (remainder < 0) ? (remainder + y) : remainder;
}
