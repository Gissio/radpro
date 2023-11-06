/*
 * Rad Pro
 * Buzzer
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(BUZZER_H)

#define BUZZER_H

#include <stdbool.h>

#include "view.h"

extern const struct View pulseClicksMenuView;

void initBuzzer(void);
void initBuzzerHardware(void);

#if defined(SDLSIM)

void updateBuzzer(void);

#endif

void setBuzzer(bool value);
bool getBuzzer(void);

void playSystemAlert(void);

#endif
