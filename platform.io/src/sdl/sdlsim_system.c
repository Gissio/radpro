/*
 * Rad Pro
 * Simulator system
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include "../system.h"

void initSystem(void)
{
}

uint32_t getDeviceId(void)
{
    return 0x08192A3B;
}

void startBootloader(void)
{
}

#endif
