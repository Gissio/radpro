/*
 * Rad Pro
 * Data log
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(DATALOG_H)
#define DATALOG_H

#include <stdbool.h>
#include <stdint.h>

#include "../measurements/pulses.h"

typedef struct {
    bool sessionStart;
    Dose dose;
} DatalogRecord;

void initDatalog(void);

void resetDatalog(void);

void startDatalog(void);
void stopDatalog(void);
void writeDatalogTimeChange(void);
void clearDatalog(void);
void updateDatalog(void);

bool startDatalogRead(void);
bool readDatalog(DatalogRecord *record);

void showDatalogMenu(void);

#endif
