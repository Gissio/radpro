/*
 * Rad Pro
 * SDLSim battery interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include "../../battery.h"
#include "../../menu.h"
#include "../../power.h"
#include "../../settings.h"

void initBatteryHardware(void)
{
}

void updateBattery(void)
{
    if (isLowBattery())
        powerOffLowBattery();
}

float getBatteryVoltage(void)
{
    return 1.28F;
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
