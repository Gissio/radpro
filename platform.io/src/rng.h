/*
 * FS2011 Pro
 * Random number generator
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */


#ifndef RNG_H
#define RNG_H

#include <stdint.h>

#include "keyboard.h"

enum RNGMode
{
    RNG_MODE_ALPHANUMERIC,
    RNG_MODE_FULL_ASCII,
    RNG_MODE_HEXADECIMAL,
    RNG_MODE_DECIMAL,
    RNG_MODE_6DICE,
    RNG_MODE_COIN_TOSS,
};

void resetRNG(enum RNGMode mode);

void onRNGPulse(uint32_t pulseTime);

void drawRNGView(void);
void onRNGViewKey(KeyEvent keyEvent);

#endif
