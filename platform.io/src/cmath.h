/*
 * Rad Pro
 * Compact math
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(CMATH_H)
#define CMATH_H

#include <stdbool.h>
#include <stdint.h>

#if defined(SIMULATOR)

#include <math.h>

#else

float logf(float x);
float log2f(float x);
float log10f(float x);
float expf(float x);
float exp2f(float x);
float powf(float x, float y);
float sqrtf(float x);

#endif

void addClamped(uint32_t *x, uint32_t y);

uint32_t getDecimalPower(int32_t exponent);
int32_t getDecimalExponent(uint32_t value);
uint32_t truncateMantissa(uint32_t value, int32_t mantissa);

float getConfidenceInterval(uint32_t n);

bool getRandomBit(void);

uint32_t getGCD(uint32_t x, uint32_t y);

#endif
