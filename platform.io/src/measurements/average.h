/*
 * Rad Pro
 * Average rate
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(AVERAGE_H)
#define AVERAGE_H

#include "../measurements/pulses.h"
#include "../ui/view.h"

extern View averageRateView;

extern View averageMenuView;

void setupAverageRate(void);

void updateAverageRate(PulsePeriod *period);

float getAverageRate(void);

#endif
