/*
 * Rad Pro
 * LED
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(INIT_H)
#define INIT_H

#include <stdbool.h>

#include "view.h"

void setPowerOnView(void);

void setPowerOffView(void);
bool isPowerOffViewActive(void);

#endif
