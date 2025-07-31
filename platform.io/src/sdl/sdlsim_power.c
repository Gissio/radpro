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

static bool powerEnabled;

void initPower(void)
{
}

void setPower(bool value)
{
    powerEnabled = value;
}

bool isPowered(void)
{
    return powerEnabled;
}

bool isPowerOnReset(void)
{
    return true;
}

bool isBatteryCharging(void)
{
    return false;
}

bool isUSBPowered(void)
{
    return false;
}

uint32_t getBatteryNum(void)
{
    return 1;
}

#endif
