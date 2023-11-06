/*
 * Rad Pro
 * Events
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(EVENTS_H)

#define EVENTS_H

#include <stdbool.h>
#include <stdint.h>

#define SYS_FREQUENCY 8000000
#define SYS_TICK_FREQUENCY 1000

#define KEY_TICKS ((uint32_t)(0.025 * SYS_TICK_FREQUENCY))

void initEvents(void);
void initEventsHardware(void);
void updateEventsMenus(void);

void onTick(void);
uint32_t getTick(void);
void sleep(uint32_t value);

void startEvents(void);
void stopEvents(void);
void updateEvents(void);

float getDeadTime(void);

void triggerDisplay(void);
bool isDisplayTimerActive(void);
void triggerPulse(void);
void triggerAlarm(void);

#endif
