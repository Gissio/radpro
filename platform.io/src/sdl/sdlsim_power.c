/*
 * Rad Pro
 * Simulator power
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include "../power.h"

void initPower(void)
{
}

void setPowerEnabled(bool value)
{
}

bool isPowerOnReset(void)
{
    return true;
}

bool isBatteryCharging(void)
{
    return true;
}

bool isUSBPowered(void)
{
    return true;
}

uint32_t getBatteryNum(void)
{
    return 1;
}

#endif
