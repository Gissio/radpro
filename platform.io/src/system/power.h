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

#if defined(BATTERY_REMOVABLE) || defined(USB_AUTOPOWER_ON) || defined(DATA_MODE)
#define POWER_MENU
#endif

void initPower(bool value);

void setPowerEnabled(bool value);
bool isUSBPowered(void);
bool isBatteryCharging(void);

bool wasResetByWatchdog(void);
void clearResetFlags(void);

void boot(void);
void powerOn(void);
void powerOff(void);
bool isPoweredOn(void);

void updatePowerState(void);

float getBatteryVoltage(void);
uint8_t getBatteryLevel(void);

void showPowerMenu(void);

#endif
