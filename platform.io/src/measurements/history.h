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

void setupHistory(void);

void resetHistory(void);

void loadHistory(void);

void updateHistory(void);

void onHistoryViewEvent(ViewEvent event);

#endif
