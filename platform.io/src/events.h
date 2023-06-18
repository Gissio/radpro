/*
 * Rad Pro
 * Event handler
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef EVENTS_H
#define EVENTS_H

#include <stdbool.h>
#include <stdint.h>

#define SYS_TICK_FREQUENCY 1000

#define KEY_TICKS ((uint32_t)(0.025F * SYS_TICK_FREQUENCY))

void initEvents(void);

void setMeasurements(bool value);

void sleep(uint32_t value);
void syncThreads(void);

void setBacklightTimer(int32_t value);
void triggerBacklight(void);
void stopBacklightTimer(void);

void setBuzzerTimer(int32_t value);
void stopBuzzerTimer(void);

void updateEvents(void);

#endif
