/*
 * FS2011 Pro
 * Power management
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef BATTERY_H
#define BATTERY_H

#include <stdint.h>

#define BATTERY_LEVEL_MAX 9
#define BATTERY_LEVEL_CHARGING 10

void initPower(void);

void setPower(bool value);

void updateBattery(void);
// +++ TEST
uint32_t getBatteryValue(void);
// --- TEST
uint8_t getBatteryLevel(void);

#endif
