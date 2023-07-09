/*
 * Rad Pro
 * Random number generator
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef RNG_H
#define RNG_H

#include <stdint.h>

#include "view.h"

extern const struct View rngMenuView;

void onRNGPulse(uint32_t pulseTime);

#endif
