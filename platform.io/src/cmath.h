/*
 * Rad Pro
 * Compact math
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(CMATH_H)

#define CMATH_H

#include <stdbool.h>
#include <stdint.h>

#if defined(SDLSIM)

#include <math.h>

#else

float exp2f(float x);
float log2f(float x);
float exp10f(float x);
float log10f(float x);
float powf(float x, float y);
float sqrtf(float x);

#endif

void addClamped(uint32_t *x, uint32_t y);

void getConfidenceIntervals(float *lowerConfidenceInterval,
                            float *upperConfidenceInterval,
                            uint32_t n);

bool getRandomBit(void);

#endif
