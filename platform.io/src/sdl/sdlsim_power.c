/*
 * Rad Pro
 * Simulator power
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include "../system/power.h"

void initPower(void)
{
}

void setPowerEnabled(bool value)
{
}

bool isUSBPowered(void)
{
    return true;
}

bool isBatteryCharging(void)
{
    return true;
}    

uint32_t getBatteryNum(void)
{
    return 1;
}

#endif
