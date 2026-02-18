/*
 * Rad Pro
 * Electric field
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(ELECTRIC_FIELD_H)
#define ELECTRIC_FIELD_H

#include "../measurements/measurements.h"
#include "../ui/menu.h"
#include "../ui/view.h"

extern const Menu electricFieldAlarmMenu;

extern const float electricFieldAlerts[];

extern const Unit electricFieldUnits;

extern const int32_t electricFieldMinMetricPrefix;

void setupElectricField(void);

void updateElectricField(void);

float getElectricField(void);

AlertLevel getElectricFieldAlertLevel(void);

void onElectricFieldViewEvent(ViewEvent event);

#endif
