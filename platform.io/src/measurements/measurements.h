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

#include "../peripherals/adc.h"
#include "../system/cstring.h"
#include "../ui/measurements.h"
#include "../ui/menu.h"
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

void resetMeasurements(void);

void setMeasurementsEnabled(bool value);
bool isMeasurementsEnabled(void);

void updateMeasurements(void);

bool isAlertEnabled(void);
AlertLevel getAlertLevel(void);
void setAlertPending(bool value);
bool isAlertPending(void);
bool isAlertFlashing(void);
bool isSoundIconActive(void);

AlertLevel getMeasurementAlertLevel(float value, const float *alerts, uint32_t warningIndex, uint32_t alarmIndex);
void updateMeasurementAlertLevel(AlertLevel *alertLevel, AlertLevel value);
MeasurementStyle getAlertMeasurementStyle(AlertLevel alertLevel);

#if defined(EMFMETER)
// Field alerts end at 80% of the threshold
#define FIELD_ALERT_HYSTERESIS 1.25F

void updateFieldAlertLevel(AlertLevel *alertLevel, float value, const float *alerts, uint32_t alarmIndex);
float calculateFieldStrength(const uint16_t *samples, const FieldSensor *sensor);
#endif

uint32_t getNextMeasurementTab(uint32_t tab, uint32_t tabNum);
uint32_t updateMeasurementAlertTab(uint32_t tab, uint32_t alertTab, bool alertTriggered, bool alertActive);

void buildValueString(char *valueString, char *unitString, float value, const Unit *unit, int32_t minMetricPrefixIndex);
void drawMeasurementValueWithUnit(const char *title, float value, const Unit *unit, int32_t minMetricPrefixIndex, float confidence, MeasurementStyle style);
void drawMeasurementInfoValue(const char *keyString, float value, const Unit *unit, int32_t minMetricPrefixIndex, MeasurementStyle style);
void drawMeasurementInfoTime(const char *keyString, uint32_t time, MeasurementStyle style);
const char *buildAlertMenuOption(menu_size_t index, float value, const Unit *unit, int32_t minMetricPrefixIndex);

void setMeasurementView(void);

void showAlertsMenu(void);

void showMeasurementsMenu(void);

#endif
