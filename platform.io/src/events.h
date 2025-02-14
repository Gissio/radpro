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

extern const View pulsesMenuView;

extern float timerCountToSeconds;

void initEventsMenus(void);

void initEvents(void);
void startEvents(void);

void onTick(void);
uint32_t getTick(void);
void sleep(uint32_t value);

void syncTimerThread(void);
void dispatchEvents(void);

float getTubeDeadTime(void);

void requestDisplayBacklightTrigger(void);
bool isDisplayBacklightTriggerRequested(void);
void triggerDisplayBacklight(void);
void cancelDisplayBacklight(void);
bool isDisplayBacklightActive(void);

void setPulseThreshold(bool value);
bool isPulseThresholdEnabled(void);

void triggerPulse(void);
void triggerAlarm(void);
void triggerVibration(void);

void togglePulseClicks(void);

#endif
