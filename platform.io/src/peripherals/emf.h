/*
 * Rad Pro
 * EMF meter
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(EMF_H)
#define EMF_H

#include <stdbool.h>

#include "../ui/view.h"

void initEMFMeter(void);

void setEMFMeterEnabled(bool value);

void updateEMFMeter(void);

#endif
