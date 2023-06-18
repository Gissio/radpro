/*
 * Rad Pro
 * Power management
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef POWER_H
#define POWER_H

#include <stdbool.h>

void initPower(void);

void powerOff(void);
void powerOffLowBattery(void);

#endif
