/*
 * Rad Pro
 * Cumulative dose
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(CUMULATIVE_H)
#define CUMULATIVE_H

#include <stdbool.h>

#include "../measurements/pulses.h"
#include "../ui/view.h"

void resetCumulativeDose(void);

void updateCumulativeDose(PulsePeriod *period);

Dose *getCumulativeDose(void);
uint32_t getCumulativeDosePulseCount(void);

AlertLevel getCumulativeDoseAlertLevel(void);
bool isCumulativeDoseAlertTriggered(void);

void clearCumulativeDose(void);
void advanceCumulativeDoseTab(void);
void drawCumulativeDoseView(void);

#endif
