/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(BUZZER_H)
#define BUZZER_H

#include <stdbool.h>

#include "view.h"

extern const View pulseClicksMenuView;

void initBuzzer(void);
void initBuzzerHardware(void);

#if defined(SDLSIM)

void updateBuzzer(void);

#endif

void setBuzzer(bool value);

void syncBuzzer(void);

void playSystemAlert(void);

#endif
