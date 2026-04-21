/*
 * Rad Pro
 * System
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(SYSTEM_H)
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

#define FIRMWARE_AUTHOR "Gissio"
#define FIRMWARE_NAME "Rad Pro"
#define FIRMWARE_VERSION "3.1test20"
// #define SETTINGS_VERSION {'R','a','d','P','r','o','3','1'}
#define SETTINGS_VERSION {'R','a','d','P','3','1','1','7'}

void initGPIO(void);

void initSystem(void);

void setFastSystemClock(bool value);

void getDeviceId(char *);

void startBootloader(void);

#endif
