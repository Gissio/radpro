/*
 * Rad Pro
 * Random number generator
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(RNG_H)
#define RNG_H

#include <stdint.h>

#include "view.h"

extern const View rngMenuView;

int32_t getRandomData(void);

void onRNGPulse(uint32_t pulseTime);

#endif
