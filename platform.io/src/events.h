/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(EVENTS_H)
#define EVENTS_H

#include <stdbool.h>
#include <stdint.h>

#define SYSTICK_FREQUENCY 1000

#define KEY_TICKS ((uint32_t)(0.025 * SYSTICK_FREQUENCY))

extern float timerCountToSeconds;

void initEvents(void);
void initEventsHardware(void);
void updateEventsMenus(void);

void onTick(void);
uint32_t getTick(void);
void sleep(uint32_t value);

void setEventHandling(bool value);
void updateDisplayEvents(void);
void dispatchEvents(void);

float getDeadTime(void);

void triggerDisplay(void);
bool isDisplayTimerActive(void);
void triggerPulse(void);
void triggerAlarm(void);

#endif
