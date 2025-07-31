/*
 * Rad Pro
 * System
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(SYSTEM_H)
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#include "view.h"

#define FIRMWARE_AUTHOR "Gissio"
#define FIRMWARE_NAME "Rad Pro"
#define FIRMWARE_VERSION "2.2beta5"

extern View statisticsView;

void initSystem(void);

void getDeviceId(char *);

void startBootloader(void);

#endif
