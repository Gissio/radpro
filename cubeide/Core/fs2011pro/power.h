/*
 * FS2011 Pro
 * Power management
 * 
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef BATTERY_H
#define BATTERY_H

#include <stdint.h>

#define BATTERY_LEVEL_MAX 9
#define BATTERY_LEVEL_CHARGING 10

void initPower();
void waitForInterrupt();
void updateWatchdog();
void powerDown(int ms);

void updateBattery();
uint8_t getBatteryLevel();

#endif
