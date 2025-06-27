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

typedef enum
{
    POWERON_VIEW_FLASHFAILURE,
    POWERON_VIEW_SPLASH,
} PowerOnViewState;

typedef struct {
    bool enabled;

    PowerOnViewState onViewState;

    uint32_t offDisplayTimer;
} Power;

extern Power power;

extern const View powerOnView;
extern const View powerOffView;

#if defined(BATTERY_REMOVABLE)
extern const View batteryTypeMenuView;
#endif

void initPower(void);

uint32_t getBatteryNum(void);

void setPower(bool value);
bool isPowered(void);

void powerOn(void);
void powerOff(bool displayEnabled);

void updateBattery(void);
uint8_t getBatteryLevel(void);
float getBatteryVoltage(void);

bool isBatteryCharging(void);
bool isUSBPowered(void);

#endif
