/*
 * Rad Pro
 * STM32 port and pin definitions
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(STM32_H)
#define STM32_H

#if defined(FS2011)

#include "fs2011.h"

#elif defined(FS600) || defined(FS1000)

#include "fs600_fs1000.h"

#elif defined(FS5000)

#include "fs5000.h"

#elif defined(GC01)

#include "gc01.h"

#endif

#endif
