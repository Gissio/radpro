/*
 * Rad Pro
 * SDLSim Battery
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdio.h>

#include "../power.h"

void initPowerHardware(void)
{
}

void setPower(bool value)
{
}

bool isBatteryCharging(void)
{
    return false;
}

bool isLowBattery(void)
{
    return false;
}

#endif
