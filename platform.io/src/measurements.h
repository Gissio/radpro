/*
 * Rad Pro
 * Instantaneous, rate and dose measurement
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <stdbool.h>
#include <stdint.h>

#include "settings.h"
#include "view.h"

#define HISTORY_BUFFER_SIZE 120
#define HISTORY_CPS_MIN 0.01F
#define HISTORY_VALUE_DECADE 40

struct Unit
{
    char *const name;
    float scale;
    int8_t minExponent;
};

struct UnitType
{
    struct Unit rate;
    struct Unit dose;
};

extern struct UnitType units[UNITS_NUM];

extern const struct View instantaneousRateView;
extern const struct View averageRateView;
extern const struct View doseView;
extern const struct View historyView;

extern const struct View unitsMenuView;
extern const struct View historyMenuView;
extern const struct View rateAlarmMenuView;
extern const struct View doseAlarmMenuView;
extern const struct View tubeTypeMenuView;

void initMeasurements(void);

void onMeasurementTick(uint32_t pulseCount);
void onMeasurementOneSecond(void);
void updateMeasurement(void);

void setDose(uint32_t time, uint32_t pulseCount);
void getDose(uint32_t *time, uint32_t *pulseCount);

uint8_t getHistoryDataPoint(uint32_t dataIndex);

void setMeasurementView(const struct View *view);
const struct View *getMeasurementView(void);

#endif
