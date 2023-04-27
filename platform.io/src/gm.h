/*
 * FS2011 Pro
 * Geiger-Müller tube
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef GM_H
#define GM_H

#include <stdint.h>

void initGM(void);

uint32_t getPulseCount(void);

#endif
