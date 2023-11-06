/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(MEASUREMENTS_H)

#define MEASUREMENTS_H

#include <stdbool.h>
#include <stdint.h>

#include "settings.h"
#include "view.h"

#define HISTORY_BUFFER_SIZE 128
#define HISTORY_BUFFER_MASK (HISTORY_BUFFER_SIZE - 1)
#define HISTORY_CPS_MIN 0.01F
#define HISTORY_VALUE_DECADE 40

extern const struct View instantaneousRateView;
extern const struct View averageRateView;
extern const struct View doseView;
extern const struct View historyView;

extern const struct View unitsMenuView;
extern const struct View rateAlarmMenuView;
extern const struct View doseAlarmMenuView;
extern const struct View averageTimerMenuView;

void initMeasurements(void);

void updateTubeType(void);

void onMeasurementTick(uint32_t pulseCount);
void onMeasurementPeriod(void);
void updateMeasurements(void);

void setMeasurementView(int32_t index);

float getInstantaneousRate(void);

void setDoseTime(uint32_t value);
uint32_t getDoseTime(void);
void setDosePulseCount(uint32_t value);
uint32_t getDosePulseCount(void);

void setTubeTime(uint32_t value);
uint32_t getTubeTime(void);
void setTubePulseCount(uint32_t value);
uint32_t getTubePulseCount(void);

uint32_t getHistoryDataPoint(uint32_t dataIndex);

#endif
