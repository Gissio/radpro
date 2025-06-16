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

typedef struct
{
    const char *name;
    float scale;
} Unit;

typedef struct
{
    Unit rate;
    Unit dose;
} Units;

extern Units units[];
extern const int8_t unitsMinMetricPrefixIndex[];

extern const View instantaneousRateView;
extern const View averageRateView;
extern const View cumulativeDoseView;
extern const View historyView;

extern const View alarmsMenuView;
extern const View measurementsMenuView;

void resetMeasurements(void);

void updateMeasurementUnits(void);

void enableMeasurements(void);
void disableMeasurements(void);

void onMeasurementTick(uint32_t pulseCount);
void onMeasurementPeriod(void);
void updateMeasurements(void);

void setMeasurementView(int32_t index);

float getInstantaneousRate(void);

void setCumulativeDoseTime(uint32_t value);
uint32_t getCumulativeDoseTime(void);
void setCumulativeDosePulseCount(uint32_t value);
uint32_t getCumulativeDosePulseCount(void);

void setTubeTime(uint32_t value);
uint32_t getTubeTime(void);
void setTubePulseCount(uint32_t value);
uint32_t getTubePulseCount(void);

bool isAlarmEnabled(void);
bool isAlarm(void);
bool isInstantaneousRateAlarm(void);
bool isCumulativeDoseAlarm(void);

char *buildRateAlarmMenuOption(uint32_t index);

#endif
