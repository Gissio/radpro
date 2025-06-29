/*
 * Rad Pro
 * STM32 flash
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../flash.h"
#include "../cstring.h"

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

const uint32_t flashFirmwareSize = FIRMWARE_SIZE;

void initFlash(void)
{
}

bool verifyFlash(void)
{
    rcc_enable_crc();

    crc_reset();

    for (uint32_t i = 0; i < (FIRMWARE_SIZE - 0x4) / 4; i++)
        crc_write(((uint32_t *)FIRMWARE_BASE)[i]);
    uint32_t crc = crc_read();

    rcc_disable_crc();

    return (crc == FIRMWARE_CRC);
}

uint8_t *getFlashPage(PageIndex pageIndex)
{
    return (uint8_t *)(FLASH_BASE + pageIndex * FLASH_PAGE_SIZE);
}

void eraseFlashPage(PageIndex pageIndex)
{
    flash_unlock();

    flash_erase_page(pageIndex);

    flash_lock();
}

void writeFlash(PageIndex pageIndex,
                uint32_t index,
                uint8_t *source,
                uint32_t size)
{
    uint8_t *dest = (uint8_t *)(FLASH_BASE + pageIndex * FLASH_PAGE_SIZE + index);

    flash_unlock();

    for (uint32_t i = 0; i < size; i += FLASH_WORD_SIZE)
    {
        if (!flash_program(dest + i, source + i) &&
            (index == 0))
        {
            flash_erase_page(pageIndex);

            flash_program(dest + i, source + i);
        }
    }

    flash_lock();
}

#endif
