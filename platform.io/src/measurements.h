/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(MEASUREMENTS_H)
#define MEASUREMENTS_H

#include <stdbool.h>
#include <stdint.h>

#include "settings.h"
#include "view.h"

typedef enum {
    ALERTLEVEL_NONE,
    ALERTLEVEL_WARNING,
    ALERTLEVEL_ALARM,
} AlertLevel;

extern View alertsMenuView;
extern View measurementsMenuView;

void resetMeasurements(void);

void updateMeasurementUnits(void);
float getCurrentRateFactor(void);
float getCurrentDoseFactor(void);

void setMeasurements(bool value);

void onMeasurementsTick(uint32_t pulseCount);
void onMeasurementsHeartbeat(void);
void updateMeasurements(void);

void setMeasurementView(int32_t index);

float getInstantaneousRate(void);

float getAverageRate(void);

void setCumulativeDoseTime(uint32_t value);
uint32_t getCumulativeDoseTime(void);
void setCumulativeDosePulseCount(uint32_t value);
uint32_t getCumulativeDosePulseCount(void);

void setTubeTime(uint32_t value);
uint32_t getTubeTime(void);
void setTubePulseCount(uint32_t value);
uint32_t getTubePulseCount(void);

void loadHistory(void);
void resetHistory(void);

bool isAlertEnabled(void);
bool isAlertBlink(void);
AlertLevel getAlertLevel(void);
bool isInstantaneousRateAlert(void);
bool isCumulativeDoseAlert(void);
bool isAlertUndismissed(void);
void dismissAlert(void);
bool isPulseSoundIconEnabled(void);

char *buildRateAlertMenuOption(uint32_t index);

#endif
