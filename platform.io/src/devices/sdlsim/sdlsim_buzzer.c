/*
 * Rad Pro
 * SDLSim buzzer interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include "../../buzzer.h"

static struct {
    bool value;
} buzzer;

void initBuzzer(void)
{
}

void setBuzzer(bool value)
{
    printf("Set buzzer: %d\n", value);

    buzzer.value = value;
}

bool getBuzzer(void)
{
    return buzzer.value;
}

#endif
