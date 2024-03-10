/*
 * Rad Pro
 * Pulse LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(PULSELED_H)
#define PULSELED_H

#include <stdbool.h>

#include "view.h"

extern const View pulseLEDMenuView;

void initPulseLED(void);
void initPulseLEDController(void);

void setPulseLED(bool value);

#endif
