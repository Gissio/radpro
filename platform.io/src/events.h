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

#include "view.h"

#define SYS_TICK_FREQUENCY 1000

#define KEY_TICKS ((uint32_t)(0.025F * SYS_TICK_FREQUENCY))

extern const struct View pulseClicksMenuView;
extern const struct View backlightMenuView;
extern const struct View dataLoggingMenuView;

void initEvents(void);
void initEventsHardware(void);
void initEventsSettings(void);

void enableMeasurement(bool value);

void onTick(void);
void sleep(uint32_t value);
void syncTimerThread(void);

void setBacklightTimer(int32_t value);
void triggerBacklight(void);
void stopBacklightTimer(void);

void playSystemAlert(void);
void setBuzzerTimer(int32_t value);
void stopBuzzerTimer(void);

void startKeyboardTimer(void);

void resetDataLoggingTimer(void);

void updateEvents(void);

#endif
