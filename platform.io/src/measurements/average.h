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
#include "../ui/menu.h"
#include "../ui/view.h"

extern const Menu averageMenu;

void setupAverageRate(void);

void updateAverageRate(PulsePeriod *period);

float getAverageRate(void);

void onAverageRateViewEvent(ViewEvent event);

#endif
