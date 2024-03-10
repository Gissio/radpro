/*
 * Rad Pro
 * Simulator power
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include "../power.h"

void initPowerController(void)
{
}

void setPower(bool value)
{
}

bool isBatteryCharging(void)
{
    return false;
}

#endif
