/*
 * Rad Pro
 * Simulator pulse LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#if defined(VIBRATOR)

#include "../display.h"
#include "../settings.h"

static bool vibrator;

void initVibratorController(void)
{
}

void setVibrator(bool value)
{
    vibrator = value;
}

#endif

#endif
