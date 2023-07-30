/*
 * Rad Pro
 * Communications
 * 
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef COMM_H
#define COMM_H

#include <stdint.h>

void initComm(void);

void sendComm(uint32_t time, uint32_t pulseCount);

#endif
