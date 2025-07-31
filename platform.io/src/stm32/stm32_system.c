/*
 * Rad Pro
 * STM32 system
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../cstring.h"

#include "device.h"

void getDeviceId(char *s)
{
    *s = '\0';
    for (uint32_t i = 0; i < 4 * 3; i++)
        strcatUInt8Hex(s, ((uint8_t *)(&UID0))[i]);
}

#endif
