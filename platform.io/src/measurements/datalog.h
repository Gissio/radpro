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
#include "../ui/view.h"

typedef struct {
    bool sessionStart;
    Dose dose;
} DatalogRecord;

extern View datalogMenuView;

void initDatalog(void);

void setupDatalog(void);

void startDatalog(void);
void stopDatalog(void);
void writeDatalogTimeChange(void);
void resetDatalog(void);
void updateDatalog(void);

bool startDatalogRead(void);
bool readDatalog(DatalogRecord *record);

#endif
