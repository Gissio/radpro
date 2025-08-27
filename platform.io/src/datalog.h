/*
 * Rad Pro
 * Data logging
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(DATALOG_H)
#define DATALOG_H

#include <stdbool.h>
#include <stdint.h>

#include "settings.h"
#include "view.h"

extern View datalogMenuView;

void resetDatalog(void);

void openDatalogWrite(void);
void closeDatalogWrite(void);
void updateDatalog(void);
void writeDatalogReset(void);

bool openDatalogRead(void);
void closeDatalogRead(void);
bool readDatalogRecord(bool *isNewLoggingSession, Dose *record);

#endif
