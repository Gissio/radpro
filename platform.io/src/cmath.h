/*
 * FS2011 Pro
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

int divideDown(int x, int y);
int remainderDown(int x, int y);

int getExponent(float value);
float getPowerOfTen(int value);

float log10fApprox(float value);

#endif
