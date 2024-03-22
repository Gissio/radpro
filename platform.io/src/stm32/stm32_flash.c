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

// Flash memory

#define SETTINGS_PAGE_BEGIN ((FIRMWARE_BASE - FLASH_BASE + FIRMWARE_SIZE) / FLASH_PAGE_SIZE)
#define SETTINGS_PAGE_END (SETTINGS_PAGE_BEGIN + 1)
#define DATALOG_PAGE_BEGIN SETTINGS_PAGE_END
#define DATALOG_PAGE_END (FLASH_SIZE / FLASH_PAGE_SIZE)

const FlashRegion flashSettingsRegion = {SETTINGS_PAGE_BEGIN, SETTINGS_PAGE_END};
const FlashRegion flashDatalogRegion = {DATALOG_PAGE_BEGIN, DATALOG_PAGE_END};

const uint32_t flashPageDataSize = FLASH_PAGE_SIZE - FLASH_WORD_SIZE;
const uint32_t flashWordSize = FLASH_WORD_SIZE;

#define FIRMWARE_CRC (*(uint32_t *)(FIRMWARE_BASE + FIRMWARE_SIZE - 0x4))

// Flash

void initFlash(void)
{
}

bool verifyFlash(void)
{
    rcc_enable_crc();

    crc_reset();

    for (int i = 0; i < (FIRMWARE_SIZE - 0x4) / 4; i++)
        crc_write(((uint32_t *)FIRMWARE_BASE)[i]);
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
    uint8_t *dest =
        (uint8_t *)(FLASH_BASE +
                    iterator->pageIndex * FLASH_PAGE_SIZE +
                    iterator->index);

    flash_unlock();

    for (uint32_t i = 0; i < size; i += FLASH_WORD_SIZE)
    {
        if (!flash_program(dest + i, source + i) &&
            (iterator->index == 0))
        {
            flash_erase_page(iterator->pageIndex);

            flash_program(dest + i, source + i);
        }
    }

    flash_lock();

    iterator->index += size;
}

#endif
