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

#include "keyboard.h"
#include "settings.h"

#define HISTORY_BUFFER_SIZE 120
#define HISTORY_CPS_MIN 0.01F
#define HISTORY_VALUE_DECADE 40

typedef struct
{
    char *const name;
    float scale;
    int8_t minExponent;
} Unit;

typedef struct
{
    Unit rate;
    Unit dose;
} UnitType;

extern UnitType units[UNITS_NUM];

extern const float rateAlarmsSvH[];
extern const float doseAlarmsSv[];

void initMeasurement(void);

void updateUnits(void);

void onMeasurementTick(uint32_t pulseCount);
void onMeasurementOneSecond(void);
void updateMeasurement(void);

void drawInstantaneousRateView(void);

void drawAverageRateView(void);

void setDose(uint32_t time, uint32_t pulseCount);
void getDose(uint32_t *time, uint32_t *pulseCount);
void drawDoseView(void);

uint8_t getHistoryDataPoint(uint32_t dataIndex);
void drawHistoryView(void);

void onMeasurementViewKey(KeyEvent keyEvent);

#endif
