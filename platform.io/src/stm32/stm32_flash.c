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

#define FIRMWARE_CRC (*(uint32_t *)(FLASH_BASE + PAYLOAD_SIZE))

#if defined(FS2011)

#define FLASH_PAGE_SIZE 0x400

const struct FlashRegion flashSettingsRegion = {
    0x20,
    0x21,
};
const struct FlashRegion flashDatalogRegion = {
    0x21,
    0x40,
};

#elif defined(FS600) || defined(FS1000)

#define FLASH_PAGE_SIZE 0x800

const struct FlashRegion flashSettingsRegion = {
    0x10,
    0x11,
};
const struct FlashRegion flashDatalogRegion = {
    0x11,
    0x40,
};

#elif defined(GC01)

#define FLASH_PAGE_SIZE 0x400

// +++ TODO
const struct FlashRegion flashSettingsRegion = {
    0x38,
    0x39,
};
const struct FlashRegion flashDatalogRegion = {
    0x39,
    0x40,
};
// +++ TODO

#endif

#define FLASH_DATA_SIZE (FLASH_PAGE_SIZE - 1)

const uint32_t flashDataSize = FLASH_DATA_SIZE;

bool checkFirmware(void)
{
    rcc_periph_clock_enable(RCC_CRC);

    uint32_t calculatedCRC = crc_calculate_block((uint32_t *)PAYLOAD_BASE, PAYLOAD_SIZE / 4);

    rcc_periph_clock_disable(RCC_CRC);

    return (calculatedCRC == FIRMWARE_CRC);
}

uint8_t *getFlashData(uint8_t pageIndex)
{
    return (uint8_t *)(FLASH_BASE + pageIndex * FLASH_PAGE_SIZE);
}

void eraseFlash(uint8_t pageIndex)
{
    flash_erase_page(FLASH_BASE + pageIndex * FLASH_PAGE_SIZE);
}

void writeFlash(uint8_t pageIndex, uint32_t index,
                uint8_t *source, uint32_t size)
{
    uint32_t dest = FLASH_BASE + pageIndex * FLASH_PAGE_SIZE + index;

    flash_unlock();

#if defined(STM32F0) || defined(STM32F1)

    uint32_t alignment = dest & 0x3;
    dest &= ~0x3;

    uint32_t value = 0;
    while (size)
    {
        for (uint32_t i = 0; i < 4; i++)
        {
            value >>= 8;

            if ((i >= alignment) && size)
            {
                value |= *source++ << 24;
                size--;
            }
            else
                value |= 0xff << 24;
        }

        flash_program_word(dest, value);

        alignment = 0;
        dest += 4;
    }

#elif defined(STM32G0)

    uint32_t alignment = dest & 0x7;
    dest &= ~0x7;

    uint64_t value = 0;
    while (size)
    {
        for (uint32_t i = 0; i < 8; i++)
        {
            value >>= 8;

            if ((i >= alignment) && size)
            {
                value |= (uint64_t)(*source++) << 56;
                size--;
            }
            else
                value |= 0xffULL << 56;
        }

        flash_program_double_word(dest, value);

        alignment = 0;
        dest += 8;
    }

#endif

    flash_lock();
}

#endif
