/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(LED_H)
#define LED_H

#include <stdbool.h>

void initLED(void);

#if defined(PULSE_LED) || defined(PULSE_LED_EN)
void initPulseLED(void);

void setPulseLED(bool value);
#endif

#if defined(ALERT_LED) || defined(ALERT_LED_EN)
void initAlertLED(void);

void setAlertLED(bool value);
#endif

void updateLED(void);

#endif
