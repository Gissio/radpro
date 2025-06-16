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

void setPower(bool value)
{
}

bool isUSBPowered(void)
{
    return false;
}

bool isChargingBattery(void)
{
    return false;
}

#endif
