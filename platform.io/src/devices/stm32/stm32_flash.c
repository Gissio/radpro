/*
 * Rad Pro
 * STM32 flash interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32F0) || defined(STM32F1) || defined(STM32G0)

#include <libopencm3/stm32/flash.h>

#include "../../flash.h"

#ifndef FLASH_BASE
#define FLASH_BASE 0x08000000
#endif

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
#if defined(STM32F0) || defined(STM32F1)
    for (uint32_t i = 0; i < size / 4; i++)
        flash_program_word(FLASH_BASE + address + 4 * i, source[i]);
#elif defined(STM32G0)
    for (uint32_t i = 0; i < size / 8; i++)
        flash_program_double_word(FLASH_BASE + address + 8 * i, ((uint64_t *)source)[i]);
#endif
}

#endif
