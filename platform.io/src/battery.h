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

#include "view.h"

#define BATTERY_LEVEL_MAX 9
#define BATTERY_LEVEL_CHARGING 10

extern const float batteryLevelVoltageThresholds[2][9];

extern const struct View batteryTypeMenuView;

void initBattery(void);
void initBatteryHardware(void);
void updateBattery(void);

uint32_t getBatteryValue(void);
float getBatteryVoltage(void);
bool isBatteryCharging(void);
bool isLowBattery(void);

uint8_t getBatteryLevel(void);

#endif
