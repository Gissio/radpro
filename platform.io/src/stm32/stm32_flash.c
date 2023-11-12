/*
 * Rad Pro
 * STM32 flash
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include <libopencm3/stm32/crc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>

#include "../flash.h"

#if !defined(FLASH_BASE)

#define FLASH_BASE 0x08000000

#endif

#define PAYLOAD_BASE FLASH_BASE
#define PAYLOAD_SIZE (0x8000 - 0x4)

// CRC

#define FIRMWARE_CRC (*(uint32_t *)(FLASH_BASE + PAYLOAD_SIZE))

// Flash

#if defined(STM32F0) || defined(STM32F1)

#define FLASH_PAGE_SIZE 0x400
#define FLASH_BLOCK_SIZE 0x2

#elif defined(STM32G0)

#define FLASH_PAGE_SIZE 0x800
#define FLASH_BLOCK_SIZE 0x8

#endif

#define FLASH_PAGE_OFFSET 0

#if defined(FS2011)

const struct FlashRegion flashSettingsRegion = {
    0x20 + FLASH_PAGE_OFFSET,
    0x21 + FLASH_PAGE_OFFSET,
};
const struct FlashRegion flashDatalogRegion = {
    0x21 + FLASH_PAGE_OFFSET,
    0x40,
};

#elif defined(FS600) || defined(FS1000)

const struct FlashRegion flashSettingsRegion = {
    0x10 + FLASH_PAGE_OFFSET,
    0x11 + FLASH_PAGE_OFFSET,
};
const struct FlashRegion flashDatalogRegion = {
    0x11 + FLASH_PAGE_OFFSET,
    0x40,
};

#elif defined(GC01)

// +++ TODO
const struct FlashRegion flashSettingsRegion = {
    0x38 + FLASH_PAGE_OFFSET,
    0x39 + FLASH_PAGE_OFFSET,
};
const struct FlashRegion flashDatalogRegion = {
    0x39 + FLASH_PAGE_OFFSET,
    0x40 + FLASH_PAGE_OFFSET,
};
// +++ TODO

#endif

const uint32_t flashPageDataSize = FLASH_PAGE_SIZE - FLASH_BLOCK_SIZE;
const uint32_t flashBlockSize = FLASH_BLOCK_SIZE;

bool checkFirmware(void)
{
    rcc_periph_clock_enable(RCC_CRC);

    uint32_t calculatedCRC = crc_calculate_block((uint32_t *)PAYLOAD_BASE, PAYLOAD_SIZE / 4);

    rcc_periph_clock_disable(RCC_CRC);

    return (calculatedCRC == FIRMWARE_CRC);
}

uint8_t *getFlash(struct FlashIterator *iterator)
{
    return (uint8_t *)(FLASH_BASE + iterator->pageIndex * FLASH_PAGE_SIZE);
}

void eraseFlash(struct FlashIterator *iterator)
{
#if defined(STM32F0) || defined(STM32F1)

    flash_unlock();

    flash_erase_page(FLASH_BASE + iterator->pageIndex * FLASH_PAGE_SIZE);

    flash_lock();

#elif defined(STM32G0)

    flash_unlock_progmem();

    flash_erase_page(iterator->pageIndex);

    flash_lock_progmem();

#endif
}

void programFlash(struct FlashIterator *iterator,
                  uint8_t *source, uint32_t size)
{
    uint32_t dest = FLASH_BASE + iterator->pageIndex * FLASH_PAGE_SIZE + iterator->index;

#if defined(STM32F0) || defined(STM32F1)

    flash_unlock();

    for (uint32_t i = 0; i < size; i += FLASH_BLOCK_SIZE)
        flash_program_half_word(dest + i, *(uint16_t *)(source + i));

    flash_lock();

#elif defined(STM32G0)

    flash_unlock_progmem();

    for (uint32_t i = 0; i < size; i += FLASH_BLOCK_SIZE)
        flash_program_double_word(dest + i, *(uint64_t *)(source + i));

    flash_lock_progmem();

#endif
}

#endif
