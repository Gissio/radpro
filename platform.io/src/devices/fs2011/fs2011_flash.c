/*
 * Rad Pro
 * FS2011 flash interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <stdint.h>

#define FLASH_PAGE_SIZE 0x400

const uint32_t flashPageSize = FLASH_PAGE_SIZE;
const uint32_t flashSettingsStart = 0x20 * FLASH_PAGE_SIZE;
const uint32_t flashSettingsEnd = 0x22 * FLASH_PAGE_SIZE;
const uint32_t flashDataLogStart = 0x22 * FLASH_PAGE_SIZE;
const uint32_t flashDataLogEnd = 0x40 * FLASH_PAGE_SIZE;

#endif
