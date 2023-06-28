/*
 * Rad Pro
 * Battery management
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef BATTERY_H
#define BATTERY_H

#include <stdbool.h>
#include <stdint.h>

#define BATTERY_LEVEL_MAX 9
#define BATTERY_LEVEL_CHARGING 10

void initBattery(void);

void updateBattery(void);

uint32_t getBatteryValue(void);
uint8_t getBatteryLevel(void);
bool isLowBattery(void);

#endif
