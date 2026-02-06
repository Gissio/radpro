/*
 * Rad Pro
 * Power management
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(POWER_H)
#define POWER_H

#include <stdbool.h>
#include <stdint.h>

#include "../ui/view.h"

extern View powerOnView;
extern View powerOffView;

#if defined(BATTERY_REMOVABLE) || !defined(START_POWERED)
extern View powerMenuView;
#endif

void initPower(void);

void setPowerEnabled(bool value);
bool isUSBPowered(void);
bool isBatteryCharging(void);
uint32_t getBatteryNum(void);

void powerOn(bool isBoot);
void powerOff(bool showBatteryIndicator);
bool isPoweredOn(void);

void updatePowerState(void);

float getBatteryVoltage(void);
uint8_t getBatteryLevel(void);

#endif
