/*
 * Rad Pro
 * Math
 *
 * (C) 2022-2026 Gissio
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
float expf(float x);
float log2f(float x);
float exp2f(float x);
float log10f(float x);
float powf(float x, float y);
float sqrtf(float x);

#endif

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)
#define CLAMP_UINT8(x) ((x < 0) ? 0 : (x > 255) ? 255 : x)

uint32_t addClamped(uint32_t x, uint32_t y);

int32_t roundDown(float x);
int32_t divideDown(int32_t x, int32_t y);

uint32_t getDecimalPower(int32_t exponent);
int32_t getDecimalDigits(uint32_t value);
uint32_t truncateDecimalMantissa(uint32_t value, int32_t mantissa);

float getConfidenceInterval(uint32_t n);

bool getRandomBit(void);

uint32_t getGCD(uint32_t x, uint32_t y);

#endif
