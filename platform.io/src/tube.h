/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(TUBE_H)
#define TUBE_H

#include <stdbool.h>
#include <stdint.h>

#include "view.h"

extern const View tubeMenuView;

void initTube(void);
void initTubeController(void);

float getTubeConversionFactor(void);
float getTubeDeadTimeCompensation(void);
float getTubeHVFrequency(void);
float getTubeHVDutyCycle(void);

void setTubeHV(bool value);
bool getTubeHV(void);
void updateTubeHV(void);
bool getTubePulse(uint32_t *pulseTime);

#endif
