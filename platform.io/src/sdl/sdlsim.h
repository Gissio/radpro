/*
 * Rad Pro
 * Simulator shared definitions
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(SDLSIM_H)
#define SDLSIM_H

#include <stdbool.h>
#include <stdlib.h>

// Simulated tube count rate, adjustable with Ctrl+key
extern float tubeCPS;

// Runs one simulated tick; returns false when ahead of real time
bool onSDLTick(void);

// Uniform random value in [0, 1)
static inline double getUniformRandomValue(void)
{
    return rand() / (RAND_MAX + 1.0);
}

#endif
