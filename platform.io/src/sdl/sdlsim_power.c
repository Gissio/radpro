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

uint32_t getBatteryNum(void)
{
    return 1;
}

void setPower(bool value)
{
    power.enabled = value;
}

bool isBatteryCharging(void)
{
    return false;
}

bool isUSBPowered(void)
{
    return false;
}

#endif
