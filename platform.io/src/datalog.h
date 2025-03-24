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

extern const View datalogMenuView;

void resetDatalog(void);

void openDatalog(void);
void updateDatalog(void);
void closeDatalog(void);

void startDatalogDownload(void);
bool getDatalogDownloadEntry(Dose *dose);

#endif
