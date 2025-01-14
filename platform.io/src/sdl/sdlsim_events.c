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

extern volatile uint32_t eventsCurrentTick;

float timerCountToSeconds = (1.0F / PULSE_MEASUREMENT_FREQUENCY);

void initEvents(void)
{
}

void sleep(uint32_t value)
{
}

#endif
