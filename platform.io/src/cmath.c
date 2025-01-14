/*
 * Rad Pro
 * Compact math
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "cmath.h"

static uint32_t randomLFSR = 1;

void addClamped(uint32_t *px, uint32_t y)
{
    if (*px > (UINT32_MAX - y))
        *px = UINT32_MAX;
    else
        *px += y;
}

float getConfidenceInterval(uint32_t n)
{
    // Patel, V. - Comparison of Confidence Intervals for the Poisson Mean: Some New Aspects (2012)
    if (n <= 1)
        return 2.6888794541139354F;
    else if (n == 2)
        return 1.785821695469449F;
    else
    {
        // Normal approximation
        float normalApproximation = 1.959964F / sqrtf((float)n);

        // First order correction
        return normalApproximation + 0.92095147F / (0.34074598F + n);
    }
}

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
