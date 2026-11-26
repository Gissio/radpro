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

// Aliased to qfplib with --defsym in platformio.ini
float logf(float x);
float expf(float x);
float sqrtf(float x);

#endif

// Fold these into a constant factor at the call site:
// log10(x) = LOG10E * logf(x), 2^x = expf(LN2 * x)
#define LOG10E 0.434294482F
#define LN2 0.693147181F

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// Arguments are evaluated more than once: pass no side effects
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define CLAMP_UINT8(x) (((x) < 0) ? 0 : ((x) > 255) ? 255 : (x))

// Keeps small helpers out of line: GCC inlines them when that grows code
#if defined(__GNUC__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

uint32_t addClamped(uint32_t x, uint32_t y);

int32_t roundDown(float x);
int32_t divideDown(int32_t x, int32_t y);

uint32_t getDecimalPower(int32_t exponent);
float getDecimalPowerFloat(int32_t exponent);
int32_t getDecimalDigits(uint32_t value);
uint32_t truncateDecimalMantissa(uint32_t value, int32_t mantissa);

float getConfidenceInterval(uint32_t n);

bool getRandomBit(void);

uint32_t getGCD(uint32_t x, uint32_t y);

#endif
