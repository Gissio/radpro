/*
 * Rad Pro
 * Simulator ADC
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdint.h>

#include "../adc.h"

void initADC(void)
{
}

float readBatteryVoltage(void)
{
    return 3.854F;
}

#endif
