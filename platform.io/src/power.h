/*
 * Rad Pro
 * Battery
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(POWER_H)

#define POWER_H

#include <stdbool.h>
#include <stdint.h>

#include "view.h"

#define BATTERY_LEVEL_MAX 9
#define BATTERY_LEVEL_CHARGING -1

extern const struct View batteryTypeMenuView;

void initPower(void);
void initPowerHardware(void);

void setPower(bool value);

void powerOff(void);
void powerOffLowBattery(void);

int8_t getBatteryLevel(void);

bool isBatteryCharging(void);
bool isLowBattery(void);

#endif
