/*
 * Rad Pro
 * STM32 device definitions
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdint.h>

#if defined(FS2011)
#include "fs2011.h"
#elif defined(FS600) || defined(FS1000)
#include "fs600_fs1000.h"
#elif defined(FS5000)
#include "fs5000.h"
#elif defined(GC01)
#include "gc01.h"
#elif defined(GC03)
#include "gc03.h"
#elif defined(GMC800)
#include "gmc800.h"
#endif

uint32_t prescalePWMParameters(uint32_t *period, uint32_t *onTime);
