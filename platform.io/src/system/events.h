/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(EVENTS_H)
#define EVENTS_H

#include <stdbool.h>
#include <stdint.h>

#define SYSTICK_FREQUENCY 1000
#define PULSE_MEASUREMENT_FREQUENCY 1000000

#define KEY_TICKS ((uint32_t)(0.025 * SYSTICK_FREQUENCY))

extern volatile uint32_t currentTick;

void initEvents(void);
void initEventsHardware(void);

void setupEvents(void);

void onTick(void);
void syncTick(void);
void resetWatchdog(void);
void sleep(uint32_t value);

void updateEvents(void);

void requestBacklightTrigger(void);
bool isBacklightTriggerRequested(void);
void triggerBacklight(void);
void cancelBacklight(void);
bool isBacklightActive(void);
bool isDisplayAwake(void);

void triggerVibration(void);

void setPulseLEDIndication(bool value);

void indicatePulse(void);

void triggerAlert(bool alarm);

#endif
