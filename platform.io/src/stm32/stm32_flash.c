/*
 * Rad Pro
 * STM32 flash
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../devices/flash.h"
#include "../system/cstring.h"
#include "../stm32/device.h"

#define FIRMWARE_CRC (FIRMWARE_BASE + FIRMWARE_SIZE - 0x4)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

__asm__(".section .comment\n"
        ".string \"FLASH_BASE: " TOSTRING(FLASH_BASE_) "\"\n"
        ".string \"FLASH_SIZE: " TOSTRING(FLASH_SIZE_) "\"\n"
        ".string \"FIRMWARE_BASE: " TOSTRING(FIRMWARE_BASE) "\"\n"
        ".string \"FIRMWARE_SIZE: " TOSTRING(FIRMWARE_SIZE) "\"\n"
        ".string \"STATE_BASE: " TOSTRING(STATE_BASE) "\"\n"
        ".string \"STATE_SIZE: " TOSTRING(STATE_SIZE) "\"\n"
        ".string \"DATALOG_BASE: " TOSTRING(DATALOG_BASE) "\"\n"
        ".string \"DATALOG_SIZE: " TOSTRING(DATALOG_SIZE) "\"\n"
        ".section .text\n");

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

    return (crc == (*(uint32_t *)FIRMWARE_CRC));
}

const uint8_t *readFlash(uint32_t source, uint32_t count)
{
    return (uint8_t *)source;
}

void writeFlash(uint32_t dest,
                const uint8_t *source,
                uint32_t count)
{
    flash_unlock();

    for (uint32_t i = 0; i < count; i += FLASH_WORD_SIZE)
    {
        // Try twice
        if (!flash_program((uint8_t *)dest + i, source + i))
            flash_program((uint8_t *)dest + i, source + i);
    }

    flash_lock();
}

void eraseFlash(uint32_t dest)
{
    flash_unlock();

    flash_erase(dest);

    flash_lock();
}

#endif
