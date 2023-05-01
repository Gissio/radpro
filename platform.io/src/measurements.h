/*
 * FS2011 Pro
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

#define HISTORY_BUFFER_SIZE 120
#define HISTORY_CPS_MIN 0.01F
#define HISTORY_VALUE_DECADE 40

void initMeasurement(void);

void resetInstantaneousRate(void);
void resetAverageRate(void);
void resetDose(void);
void resetHistory(void);

void onMeasurementTick(uint32_t pulseCount);
void onMeasurementOneSecond(void);
void updateMeasurement(void);

bool isInstantaneousRateAlarm(void);
bool isDoseAlarm(void);

uint8_t getHistoryDataPoint(uint32_t dataIndex);

void drawInstantaneousRateView(void);
void drawAverageRateView(void);
void drawDoseView(void);
void drawHistoryView(void);
void onMeasurementViewKey(KeyEvent keyEvent);

#endif
