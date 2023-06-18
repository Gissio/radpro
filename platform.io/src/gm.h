/*
 * Rad Pro
 * Geiger-Müller tube
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef GM_H
#define GM_H

#include <stdbool.h>
#include <stdint.h>

void initGM(void);

void syncGMHVPulse(void);

bool getGMPulse(uint32_t *pulseTime);

#endif
