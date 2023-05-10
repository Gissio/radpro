/*
 * FS2011 Pro
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

#define KEY_TICKS ((int)(0.025F * SYS_TICK_FREQUENCY))

void initEvents(void);

void setEventsEnabled(bool value);

void waitSysTicks(uint32_t value);

void updateBacklight(void);
void setBacklightTimer(int32_t value);

void setBuzzerTimer(int32_t value);

void updateEvents(void);

#endif
