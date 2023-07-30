/*
 * Rad Pro
 * FS600 flash interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS600

#include <stdint.h>

#define FLASH_PAGE_SIZE 0x800

const uint32_t flashPageSize = FLASH_PAGE_SIZE;
const uint32_t flashSettingsStart = 0x10 * FLASH_PAGE_SIZE;
const uint32_t flashSettingsEnd = 0x12 * FLASH_PAGE_SIZE;
const uint32_t flashDataLogStart = 0x12 * FLASH_PAGE_SIZE;
const uint32_t flashDataLogEnd = 0x40 * FLASH_PAGE_SIZE;

#endif
