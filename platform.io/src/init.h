/*
 * Rad Pro
 * Initialization
 *
 * (C) 2022-2025 Gissio
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
