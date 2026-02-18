/*
 * Rad Pro
 * Random number generator
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(RNG_H)
#define RNG_H

#include <stdint.h>

void setupRNG(void);

void updateRNG();

int32_t getRNGByte(void);

void showRNGMenu(void);

#endif
