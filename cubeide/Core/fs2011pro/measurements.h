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

#include "events.h"

// M4011 tube:
// #define CPM_PER_USVH 153.8F
// HH614 tube:
#define CPM_PER_USVH 60.0F

#define HISTORY_BUFFER_SIZE 120
#define HISTORY_CPS_MIN 0.01F
#define HISTORY_VALUE_DECADE 40

void initMeasurements();

void resetInstantaneousRate();
void resetAverageRate();
void resetDose();
void resetHistory();

void onMeasurementTick(unsigned int pulseCount);
void onMeasurementOneSecond();
void updateMeasurements();

bool isInstantaneousRateAlarm();
bool isDoseAlarm();

unsigned char getHistoryDataPoint(int dataIndex);

void drawInstantaneousRateView();
void drawAverageRateView();
void drawDoseView();
void drawHistoryView();
void onMeasurementViewKey(int key);

#endif
