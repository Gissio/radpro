/*
 * Rad Pro
 * STM32 compact math
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <stdint.h>

#include "qfplib.h"
#include "../cmath.h"

// uint32_t __udivsi3(uint32_t x, uint32_t y);
// uint32_t __aeabi_uidivmod(uint32_t x, uint32_t y);

float __aeabi_fadd(float x, float y);
float __aeabi_fsub(float x, float y);
float __aeabi_fmul(float x, float y);
float __aeabi_fdiv(float x, float y);

float __aeabi_i2f(int x);
float __aeabi_ui2f(unsigned int x);
int __aeabi_f2iz(float x);
unsigned int __aeabi_f2uiz(float x);

int __aeabi_fcmpeq(float x, float y);
int __aeabi_fcmplt(float x, float y);
int __aeabi_fcmple(float x, float y);
int __aeabi_fcmpge(float x, float y);
int __aeabi_fcmpgt(float x, float y);
int __aeabi_fcmpun(float x, float y);

// uint32_t __udivsi3(uint32_t x, uint32_t y)
// {
//     if (y == 0)
//         return 0;

//     uint32_t q = 0;
//     uint32_t r = 0;

//     for (int i = 31; i >= 0; i--)
//     {
//         r = (r << 1) | ((x >> i) & 1);
//         if (r >= y)
//         {
//             r -= y;
//             q |= (1U << i);
//         }
//     }

//     return q;
// }

// uint32_t __aeabi_uidivmod(uint32_t x, uint32_t y)
// {
//     return 0;
// }

float __aeabi_fadd(float x, float y)
{
    return qfp_fadd(x, y);
}

float __aeabi_fsub(float x, float y)
{
    return qfp_fsub(x, y);
}

float __aeabi_fmul(float x, float y)
{
    return qfp_fmul(x, y);
}

float __aeabi_fdiv(float x, float y)
{
    return qfp_fdiv(x, y);
}

float __aeabi_i2f(int x)
{
    return qfp_float2int(x);
}

float __aeabi_ui2f(unsigned int x)
{
    return qfp_uint2float(x);
}

int __aeabi_f2iz(float x)
{
    int value = qfp_float2int(x);

    return (value < 0) ? value + 1 : value;
}

unsigned int __aeabi_f2uiz(float x)
{
    return qfp_float2uint(x);
}

int __aeabi_fcmpeq(float x, float y)
{
    return (qfp_fcmp(x, y) == 0);
}

int __aeabi_fcmplt(float x, float y)
{
    return (qfp_fcmp(x, y) < 0);
}

int __aeabi_fcmple(float x, float y)
{
    return (qfp_fcmp(x, y) <= 0);
}

int __aeabi_fcmpge(float x, float y)
{
    return (qfp_fcmp(x, y) >= 0);
}

int __aeabi_fcmpgt(float x, float y)
{
    return (qfp_fcmp(x, y) > 0);
}

int __aeabi_fcmpun(float x, float y)
{
    return (qfp_fcmp(x, y) != 0);
}

float exp2f(float x)
{
    return qfp_fexp(0.693147180559945F * x);
}

float log2f(float x)
{
    return 1.44269504088896F * qfp_fln(x);
}

float exp10f(float x)
{
    return qfp_fexp(2.302585092994045F * x);
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
