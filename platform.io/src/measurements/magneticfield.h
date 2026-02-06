/*
 * Rad Pro
 * Magnetic field
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(MAGNETIC_FIELD_H)
#define MAGNETIC_FIELD_H

#include "../measurements/measurements.h"
#include "../ui/view.h"

extern View magneticFieldView;

extern View magneticFieldUnitsMenuView;

extern View magneticFieldAlarmMenuView;

extern const float magneticFieldAlerts[];

extern const Unit magneticFieldUnits[];

extern const int32_t magneticFieldMinMetricPrefix[];

void setupMagneticField(void);

void updateMagneticField(void);

float getMagneticField(void);

AlertLevel getMagneticFieldAlertLevel(void);

#endif
