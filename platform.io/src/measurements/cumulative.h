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

extern View cumulativeDoseView;

void setupCumulativeDose(void);

void updateCumulativeDose(PulsePeriod *period);

void setCumulativeDoseTime(uint32_t value);
uint32_t getCumulativeDoseTime(void);
void setCumulativeDosePulseCount(uint32_t value);
uint32_t getCumulativeDosePulseCount(void);

AlertLevel getCumulativeDoseAlertLevel(void);
bool isCumulativeDoseAlertTriggered(void);

#endif
