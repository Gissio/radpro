/*
 * Rad Pro
 * Buzzer
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <stdbool.h>

void initBuzzer(void);

void setBuzzer(bool value);
bool getBuzzer(void);

void playSystemAlert(void);

#endif
