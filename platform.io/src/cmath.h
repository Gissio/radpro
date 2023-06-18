/*
 * Rad Pro
 * Complementary math
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef CMATH_H
#define CMATH_H

#include <stdint.h>

void addClamped(uint32_t *x, uint32_t y);

int32_t divideDown(int32_t x, int32_t y);
int32_t remainderDown(int32_t x, int32_t y);

int32_t getExponent(float value);
float getPowerOfTen(int32_t exponent);

float log10fApprox(float value);

#endif
