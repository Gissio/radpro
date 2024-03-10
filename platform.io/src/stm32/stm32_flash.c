/*
 * Rad Pro
 * STM32 flash
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <string.h>

#include "../flash.h"

#include "device.h"

// Flash

const uint32_t flashPageDataSize = FLASH_PAGE_SIZE - FLASH_BLOCK_SIZE;
const uint32_t flashBlockSize = FLASH_BLOCK_SIZE;

#define FIRMWARE_CRC (*(uint32_t *)(PAYLOAD_BASE + PAYLOAD_SIZE))

// Flash

void initFlash(void)
{
#if defined(STM32G0)
    rcc_enable_flash();
#endif
}

bool verifyFlash(void)
{
    rcc_enable_crc();

    crc_reset();

    for (int i = 0; i < PAYLOAD_SIZE / 4; i++)
        crc_write(((uint32_t *)PAYLOAD_BASE)[i]);
    uint32_t crc = crc_read();

    rcc_disable_crc();

    return (crc == FIRMWARE_CRC);
}

void readFlash(FlashIterator *iterator,
               uint8_t *dest,
               uint32_t size)
{
    uint32_t address = iterator->pageIndex * FLASH_PAGE_SIZE + iterator->index;

    memcpy(dest,
           (uint8_t *)(FLASH_BASE + address),
           size);

    iterator->index += size;
}

void eraseFlash(FlashIterator *iterator)
{
    flash_unlock();

    flash_erase_page(iterator->pageIndex);

    flash_lock();
}

void writeFlash(FlashIterator *iterator,
                uint8_t *source,
                uint32_t size)
{
    uint32_t address = FLASH_BASE + iterator->pageIndex * FLASH_PAGE_SIZE + iterator->index;

    flash_unlock();

#if defined(STM32F0) || defined(STM32F1)

    for (uint32_t i = 0; i < size; i += FLASH_BLOCK_SIZE)
        flash_program_halfword(address + i, *(uint16_t *)(source + i));

#elif defined(STM32G0)

    for (uint32_t i = 0; i < size; i += FLASH_BLOCK_SIZE)
        flash_program_doubleword(address + i, *(uint64_t *)(source + i));

#endif

    flash_lock();

    iterator->index += size;
}

#endif
