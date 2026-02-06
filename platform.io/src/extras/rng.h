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

#include "../ui/view.h"

extern View rngMenuView;

void setupRNG(void);

void updateRNG();

int32_t getRNGByte(void);

#endif
