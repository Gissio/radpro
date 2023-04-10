/*
 * FS2011 Pro
 * Instantaneous, rate and dose measurement
 * 
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <stdbool.h>
#include <stdint.h>

#include "events.h"

#define HISTORY_BUFFER_SIZE 120
#define HISTORY_CPS_MIN 0.01F
#define HISTORY_VALUE_DECADE 40

void initMeasurements();

void resetInstantaneousRate();
void resetAverageRate();
void resetDose();
void resetHistory();

void onMeasurementTick(uint32_t pulseCount);
void onMeasurementOneSecond();
void updateMeasurements();

bool isInstantaneousRateAlarm();
bool isDoseAlarm();

uint8_t getHistoryDataPoint(int dataIndex);

void drawInstantaneousRateView();
void drawAverageRateView();
void drawDoseView();
void drawHistoryView();
void onMeasurementViewKey(int key);

#endif
