/*
 * Rad Pro
 * FS2011 flash interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <libopencm3/stm32/flash.h>

#include "../../flash.h"

#define FLASH_PAGE_SIZE 0x400

const uint32_t flashPageSize = FLASH_PAGE_SIZE;
const uint32_t flashSettingsStart = 0x24 * FLASH_PAGE_SIZE;
const uint32_t flashSettingsEnd = 0x26 * FLASH_PAGE_SIZE;
const uint32_t flashLifeStateStart = 0x26 * FLASH_PAGE_SIZE;
const uint32_t flashLifeStateEnd = 0x28 * FLASH_PAGE_SIZE;
const uint32_t flashDoseStateStart = 0x28 * FLASH_PAGE_SIZE;
const uint32_t flashDoseStateEnd = 0x40 * FLASH_PAGE_SIZE;

void initFlash(void)
{
}

void lockFlash(void)
{
    flash_lock();
}

void unlockFlash(void)
{
    flash_unlock();
}

uint8_t *getFlashMemory(uint32_t address)
{
    return ((uint8_t *)FLASH_BASE) + address;
}

void eraseFlash(uint32_t address)
{
    flash_erase_page(FLASH_BASE + address);
}

void writeFlash(uint32_t *source, uint32_t address, uint32_t size)
{
    for (uint32_t i = 0; i < size / 4; i++)
        flash_program_word(FLASH_BASE + address + 4 * i, source[i]);
}

#endif
