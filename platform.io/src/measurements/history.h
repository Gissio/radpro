/*
 * Rad Pro
 * History
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(HISTORY_H)
#define HISTORY_H

#include "../ui/view.h"

extern View historyView;

void setupHistory(void);

void resetHistory(void);

void loadHistory(void);

void updateHistory(void);

#endif
