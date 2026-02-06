/*
 * Rad Pro
 * Instantaneous rate
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(INSTANTANEOUS_H)
#define INSTANTANEOUS_H

#include <stdbool.h>

#include "../measurements/cumulative.h"
#include "../measurements/pulses.h"
#include "../ui/view.h"

extern View instantaneousRateView;

extern View instantaneousMenuView;

void setupInstantaneousRate(void);

void updateInstantaneousRate(uint32_t periodTick, PulsePeriod *period);

float getDeadTimeCompensationFactor(void);

float getInstantaneousRate(void);

bool isInstantaneousRateConfidenceGood(void);

AlertLevel getInstantaneousRateAlertLevel(void);

#endif
