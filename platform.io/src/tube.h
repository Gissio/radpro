/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(TUBE_H)

#define TUBE_H

#include <stdbool.h>
#include <stdint.h>

#include "view.h"

extern const struct View tubeMenuView;

void initTube(void);
void initTubeHardware(void);

float getTubeCustomConversionFactor(void);

float getTubeHVDutyCycle(void);
float getTubeHVFrequency(void);
void updateTubeHV(void);

float getTubeDeadTimeCompensation(void);

void syncHVPulse(void);

bool getTubePulse(uint32_t *pulseTime);

#endif
