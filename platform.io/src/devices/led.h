/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(LED_H)
#define LED_H

#include <stdbool.h>

void initLED(void);

#if defined(PULSE_LED)
void initPulseLED(void);

void setPulseLED(bool value);
#endif

#if defined(ALERT_LED)
void initAlertLED(void);

void setAlertLED(bool value);
#endif

#if defined(PULSE_LED_EN)
void initPulseLEDEnable(void);

void setPulseLEDEnable(bool value);
#endif

#if defined(ALERT_LED_EN)
void initAlertLEDEnable(void);

void setAlertLEDEnable(bool value);
#endif

void updateLED(void);

#endif
