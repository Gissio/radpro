/*
 * Rad Pro
 * Data logging
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(DATALOG_H)

#define DATALOG_H

#include <stdbool.h>
#include <stdint.h>

#include "settings.h"
#include "view.h"

extern const struct View datalogMenuView;

void initDatalog(void);

void startDatalog(void);
void stopDatalog(void);

void writeDatalogEntry(bool isUpdate);

void initDatalogRead(void);
bool readDatalog(struct Dose *dose);

#endif
