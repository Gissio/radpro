/*
 * Rad Pro
 * STM32 compact math
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32) && defined(QFP)

#include <stdint.h>

#include "qfplib.h"

float log2f(float x)
{
    return 1.44269504088896F * qfp_fln(x);
}

float exp2f(float x)
{
    return qfp_fexp(0.693147180559945F * x);
}

float log10f(float x)
{
    return 0.434294481903251F * qfp_fln(x);
}

float powf(float x, float y)
{
    return qfp_fexp(y * qfp_fln(x));
}

float sqrtf(float x)
{
    return qfp_fsqrt(x);
}

#endif
