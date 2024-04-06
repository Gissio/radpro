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

#include "menu.h"
#include "view.h"

extern const View tubeMenuView;
extern const View pulsesMenuView;

void initTube(void);
void initTubeController(void);

float getTubeConversionFactor(void);
float getTubeDeadTimeCompensation(void);
float getTubeBackgroundCompensation(void);
float getTubeHVFrequency(void);
float getTubeHVDutyCycle(void);

void setTubeHV(bool value);
bool getTubeHV(void);
void updateTubeHV(void);
bool getTubePulse(uint32_t *pulseTime);

void onPulsesSubMenuBack(const Menu *menu);

#endif
