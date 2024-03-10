/*
 * Rad Pro
 * Battery
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(POWER_H)
#define POWER_H

#include <stdbool.h>
#include <stdint.h>

#include "view.h"

#define BATTERY_LEVEL_MAX 5
#define BATTERY_LEVEL_CHARGING -1

typedef enum
{
    POWERSTATE_ON,
    POWERSTATE_OFF_REQUESTED,
    POWERSTATE_OFF,
} PowerState;

extern const View batteryTypeMenuView;

void initPower(void);
void initPowerController(void);

void setPower(bool value);

void requestPowerOff(void);
bool isPowerOffRequested(void);

int8_t getDeviceBatteryLevel(void);

bool isBatteryCharging(void);

#endif
