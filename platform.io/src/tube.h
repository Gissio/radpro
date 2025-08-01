/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(TUBE_H)
#define TUBE_H

#include <stdbool.h>
#include <stdint.h>

#include "menu.h"
#include "view.h"

extern View tubeMenuView;

void initTube(void);
void initTubeHardware(void);
void resetTube(void);

float getTubeSensitivity(void);
uint32_t getLossOfCountTime(void);
float getTubeDeadTimeCompensation(void);
float getTubeHVFrequency(void);
bool setTubeHVFrequency(float value);
float getTubeHVDutyCycle(void);
bool setTubeHVDutyCycle(float value);

void setTubeHV(bool value);
bool getTubeHV(void);
void updateTubeHV(void);
bool getTubePulseTime(uint32_t *pulseTime);
bool getTubeDet(void);

void onPulsesSubMenuBack(void);

#endif
