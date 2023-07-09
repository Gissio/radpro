/*
 * Rad Pro
 * SDLSim power interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include <stdio.h>

#include "../../power.h"

void initPower(void)
{
}

void setPower(bool value)
{
    printf("Set power: %d\n", value);
}

#endif
