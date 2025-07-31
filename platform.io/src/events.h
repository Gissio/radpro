/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(EVENTS_H)
#define EVENTS_H

#include <stdbool.h>
#include <stdint.h>

#include "menu.h"
#include "view.h"

#define SYSTICK_FREQUENCY 1000
#define PULSE_MEASUREMENT_FREQUENCY 1000000

#define KEY_TICKS ((uint32_t)(0.025 * SYSTICK_FREQUENCY))

extern View pulsesMenuView;

extern float timerCountToSeconds;

void initEvents(void);
void initEventsHardware(void);
void resetEvents(void);

void onTick(void);
void syncTick(void);
uint32_t getTick(void);
void sleep(uint32_t value);

void dispatchEvents(void);

float getTubeDeadTime(void);

#if defined(PULSE_LED)
void cancelPulseLEDTimer(void);
void setPulseLEDEnabled(bool value);
#endif

void requestBacklightTrigger(void);
bool isBacklightTriggerRequested(void);
void triggerBacklight(void);
void cancelBacklight(void);
bool isBacklightActive(void);

void setPulseThresholdExceeded(bool value);
bool isPulseThresholdExceeded(void);

void triggerPulse(void);
void triggerAlert(void);
void triggerSound(void);
void triggerVibration(void);

void togglePulseSound(void);

#endif
