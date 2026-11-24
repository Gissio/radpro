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
#include "../ui/measurements.h"
#include "../ui/menu.h"

extern const Menu rateWarningMenu;
extern const Menu rateAlarmMenu;
extern const Menu doseWarningMenu;
extern const Menu doseAlarmMenu;

extern const Menu doseUnitsMenu;
extern const Menu secondaryDoseUnitsMenu;

typedef struct
{
#if defined(RADPRO_RAW_COUNTER)
    uint32_t seconds;
#else
    uint32_t firstTick;
    uint32_t lastTick;
#endif
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

void buildPulseValueString(char *valueString, char *unitString, float value, uint32_t doseUnits, bool isDose);
void drawPulseMeasurementValue(const char *title, float value, bool isDose, float confidence, MeasurementStyle style);

void resetPulses(void);

void updateDoseUnits(void);
#if !defined(RADPRO_RAW_COUNTER)
void calculateRate(Rate *rate, PulsePeriod *period);
#endif
float getRateFactor(void);
float getDoseFactor(void);

void updatePulseThresholdExceeded(void);
bool isPulseThresholdExceeded(void);
void onPulseTick(void);
void onPulsesHeartbeat(void);
void updatePulses(void);

void setTubeTime(uint32_t value);
uint32_t getTubeTime(void);
void setTubePulseCount(uint32_t value);
uint32_t getTubePulseCount(void);
float getTubeDeadTime(void);
AlertLevel getTubeFaultAlertLevel(void);
bool isTubeFaultAlertTriggered(void);

void showPulsesMenu(void);

#endif
