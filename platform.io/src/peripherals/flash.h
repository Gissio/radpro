/*
 * Rad Pro
 * Flash memory
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(FLASH_H)
#define FLASH_H

#include <stdbool.h>
#include <stdint.h>

#define FLASH_END_ (FLASH_BASE_ + FLASH_SIZE_)

#define getFlashPageBase(value) (value & (~(FLASH_PAGE_SIZE - 1)))
#define getFlashPageNumber(value) (value / FLASH_PAGE_SIZE)
#define getFlashPageOffset(value) (value & (FLASH_PAGE_SIZE - 1))

#define STATES_BASE (FIRMWARE_BASE + FIRMWARE_SIZE)
#define STATES_SIZE FLASH_PAGE_SIZE
#define STATES_END (STATES_BASE + STATES_SIZE)
#define DATALOG_BASE STATES_END
#if defined(GC03)
#define DATALOG_SIZE (FLASH_END_ - DATALOG_BASE - FLASH_PAGE_SIZE)
#else
#define DATALOG_SIZE (FLASH_END_ - DATALOG_BASE)
#endif
#define DATALOG_END (DATALOG_BASE + DATALOG_SIZE)

#define DATALOG_PAGE_SIZE FLASH_PAGE_SIZE
#define DATALOG_PAGESTATE_OFFSET (DATALOG_PAGE_SIZE - DATALOG_PAGESTATE_SIZE)
#define DATALOG_PAGESTATE_SIZE FLASH_WORD_SIZE

void initFlash(void);

bool verifyFlash(void);

const uint8_t *readFlash(uint32_t source, uint32_t count);
bool writeFlash(uint32_t dest, const uint8_t *source, uint32_t count);
void eraseFlash(uint32_t dest);

#endif
