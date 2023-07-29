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

uint32_t getBatteryValue(void)
{
    return 3300;
}

float getBatteryVoltage(void)
{
    return getBatteryValue() * (3.3F / 4096 / 2);
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
