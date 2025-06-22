/*
 * Rad Pro
 * Power management
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(POWER_H)
#define POWER_H

#include <stdbool.h>
#include <stdint.h>

#include "view.h"

#define BATTERY_LEVEL_NUM 5

extern const View powerOnView;
extern const View powerOffView;

extern const View batteryTypeMenuView;

void initPower(void);
void resetPower(void);

void setPower(bool value);

void powerOn(void);
void powerOff(void);
bool isPoweredOff(void);

uint8_t getBatteryLevel(void);

bool isUSBPowered(void);
bool isChargingBattery(void);

#endif
