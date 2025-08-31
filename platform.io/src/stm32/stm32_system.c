/*
 * Rad Pro
 * STM32 system
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../cmath.h"
#include "../cstring.h"

#include "device.h"

uint32_t prescalePWMParameters(uint32_t *period, uint32_t *onTime)
{
    uint32_t localPeriod = *period;
    uint32_t localOnTime = *onTime;

    // Get presacler factor
    uint32_t prescalerFactor = getGCD(localPeriod, localOnTime);
    if (prescalerFactor == localPeriod)
        prescalerFactor >>= 1;
    localPeriod /= prescalerFactor;
    localOnTime /= prescalerFactor;

    // Scale prescaler factor
    while (prescalerFactor >> 16)
    {
        localPeriod <<= 1;
        localOnTime <<= 1;
        prescalerFactor >>= 1;
    }

    // Scale period
    while (localPeriod >> 16)
    {
        localPeriod >>= 1;
        localOnTime >>= 1;
        prescalerFactor <<= 1;
    }

    *period = localPeriod;
    *onTime = localOnTime;

    return prescalerFactor;
}

void getDeviceId(char *s)
{
    *s = '\0';
    for (uint32_t i = 0; i < 4 * 3; i++)
        strcatUInt8Hex(s, ((uint8_t *)(&UID0))[i]);
}

#endif
