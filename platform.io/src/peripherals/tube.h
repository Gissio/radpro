/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(TUBE_H)
#define TUBE_H

#include <stdbool.h>
#include <stdint.h>

#include "../ui/view.h"

extern View sourceMenuView;

extern View tubeMenuView;

extern volatile uint32_t tubePulseCount;
extern volatile uint32_t tubeRandomBits;
extern volatile uint32_t tubeDeadTime;

void initTube(void);
void initTubeHardware(void);

void setupTube(void);

float getTubeSensitivity(void);
uint32_t getLossOfCountTime(void);

float getTubeDeadTimeCompensation(void);

bool getTubeDefaultHVFrequency(float *value);
float getTubeHVFrequency(void);
bool setTubeHVFrequency(float value);
bool getTubeDefaultHVDutyCycle(float *value);
float getTubeHVDutyCycle(void);
bool setTubeHVDutyCycle(float value);

void setTubeHVEnabled(bool value);
void updateTubeHV(void);

void onTubeTick(void);
bool readTubeDet(void);

#endif
