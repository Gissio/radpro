/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(MEASUREMENTS_H)
#define MEASUREMENTS_H

#include <stdbool.h>
#include <stdint.h>

#include "../system/cstring.h"
#include "../ui/view.h"

typedef struct
{
    cstring name;
    float scale;
} Unit;

typedef enum {
    ALERTLEVEL_NONE,
    ALERTLEVEL_WARNING,
    ALERTLEVEL_ALARM,
} AlertLevel;

extern View alertsMenuView;

extern View measurementsMenuView;

void setupMeasurements(void);

void setMeasurementsEnabled(bool value);
bool isMeasurementsEnabled(void);

void updateMeasurements(void);

bool isAlertEnabled(void);
AlertLevel getAlertLevel(void);
void setAlertPending(bool value);
bool isAlertPending(void);
bool isAlertFlashing(void);
bool isSoundIconActive(void);

void buildValueString(char *valueString, char *unitString, float value, const Unit *unit, int32_t minMetricPrefixIndex);
void setMeasurementViewCurrent(void);
bool onMeasurementViewEvent(Event event);

void setAlertsMenu(void);

void setMeasurementsMenu(void);

#endif
