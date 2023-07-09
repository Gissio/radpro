/*
 * Rad Pro
 * Flas memory
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

extern const uint32_t flashPageSize;
extern const uint32_t flashSettingsStart;
extern const uint32_t flashSettingsEnd;
extern const uint32_t flashLifeStateStart;
extern const uint32_t flashLifeStateEnd;
extern const uint32_t flashDoseStateStart;
extern const uint32_t flashDoseStateEnd;

void initFlash(void);
uint8_t *getFlashMemory(uint32_t address);
void lockFlash(void);
void unlockFlash(void);
void eraseFlash(uint32_t address);
void writeFlash(uint32_t *source, uint32_t address, uint32_t size);

uint32_t getLastEntry(uint32_t addressStart, uint32_t addressEnd, uint32_t size);
void flashEntry(uint32_t addressStart, uint32_t addressEnd, uint32_t size, uint32_t *source);

#endif
