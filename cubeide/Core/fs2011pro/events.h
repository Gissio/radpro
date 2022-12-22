/*
 * FS2011 Pro
 * Event handler
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef EVENTS_H
#define EVENTS_H

#define TICK_FREQUENCY 1000
#define KEY_TICKS ((int)(TICK_FREQUENCY * 0.025F))

void initEvents();

void triggerPulse();
void triggerBacklight();

void onEventsTick();
void updateEvents();

int getEventsKey();

#endif
