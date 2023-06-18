/*
 * Rad Pro
 * Buzzer
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef BUZZ_H
#define BUZZ_H

#include <stdbool.h>

void initBuzzer(void);
void setBuzzer(bool value);
bool getBuzzer(void);

#endif
