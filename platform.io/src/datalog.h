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

struct DatalogState
{
    uint32_t pageIndex;
    uint32_t index;
    uint32_t deltaTime;
    struct Dose dose;
};

extern const struct View datalogMenuView;

void initDatalog(void);

void setDatalogPause(bool value);

void initDatalogState(struct DatalogState *state);
bool updateDatalogState(struct DatalogState *state);

void writeDatalogEntry(void);
void updateDatalog(void);

#endif
