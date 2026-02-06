/*
 * Rad Pro
 * Pulses
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(PULSES_H)
#define PULSES_H

#include <stdbool.h>
#include <stdint.h>

#include "../measurements/measurements.h"
#include "../ui/view.h"

typedef struct
{
    uint32_t firstTick;
    uint32_t lastTick;
    uint32_t pulseCount;
} PulsePeriod;

typedef struct
{
    uint32_t time;
    uint32_t pulseCount;
} Dose;

typedef struct
{
    uint32_t time;
    float value;
    float confidence;
} Rate;

typedef struct
{
    Unit rate;
    Unit dose;
} PulseUnits;

extern const float rateAlerts[];
extern const float doseAlerts[];

extern PulseUnits pulseUnits[];

extern const int8_t doseUnitsMinMetricPrefix[];

extern const int8_t doseUnitsBarMinExponent[];

extern View pulsesMenuView;

extern View rateWarningMenuView;
extern View rateAlarmMenuView;
extern View doseWarningMenuView;
extern View doseAlarmMenuView;

extern View doseUnitsMenuView;
extern View secondaryDoseUnitsMenuView;

void setupPulses(void);

void updateDoseUnits(void);
void calculateRate(Rate *rate, PulsePeriod *period);
float getRateFactor(void);
float getDoseFactor(void);

void setPulseMeasurementsEnabled(bool value);
void updatePulseThresholdExceeded(void);
bool isPulseThresholdExceeded(void);
void onPulseTick(void);
void onPulseHeartbeat(void);
void updatePulses(void);

void setTubeTime(uint32_t value);
uint32_t getTubeTime(void);
void setTubePulseCount(uint32_t value);
uint32_t getTubePulseCount(void);
float getTubeDeadTime(void);
AlertLevel getTubeFaultAlertLevel(void);
bool isTubeFaultAlertTriggered(void);

#endif
