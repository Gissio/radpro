/*
 * Rad Pro
 * Simulator system
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include "../system.h"

void initSystem(void)
{
}

void getDeviceId(char *s)
{
    strcpy(s, "b5706d937087f975b5812810");
}

void startBootloader(void)
{
    printf("Starting bootloader...\n");
}

// Pulse control

void initPulseControl(void)
{
}

void updatePulseControl(void)
{
}

void setPulseControlEnabled(bool value)
{
}

void onPulseControlTick(void)
{
}

#endif
