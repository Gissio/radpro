/*
 * Rad Pro
 * Debugging
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(DEBUG_H)
#define DEBUG_H

#include <stdbool.h>
#include <stdint.h>

uint32_t getGPIO(uint32_t index);

void debugBeep(void);
void debugWait(uint32_t ms);
void debugBit(bool value);
void debugUInt32(uint32_t value);
void runTestMode(void);

#endif
