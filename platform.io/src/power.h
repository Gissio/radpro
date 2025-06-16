/*
 * Rad Pro
 * Battery
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

extern const View batteryTypeMenuView;

void initPower(void);
void resetPower(void);

void setPower(bool value);

uint8_t getBatteryLevel(void);

bool isUSBPowered(void);
bool isChargingBattery(void);

#endif
