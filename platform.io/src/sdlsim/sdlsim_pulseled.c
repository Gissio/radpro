/*
 * Rad Pro
 * SDLSim pulse LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#if defined(PULSE_LED)

#include "../display.h"
#include "../settings.h"

static bool pulseLED;

void initPulseLEDHardware(void)
{
}

void setPulseLED(bool value)
{
    pulseLED = value;
}

#endif

#endif
