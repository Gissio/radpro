/*
 * Rad Pro
 * Simulator events
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <SDL.h>

#include "../events.h"

float timerCountToSeconds = (1.0F / PULSE_MEASUREMENT_FREQUENCY);

void initEventsHardware(void)
{
}

void resetWatchdog(void)
{
}

void sleep(uint32_t value)
{
}

#endif
