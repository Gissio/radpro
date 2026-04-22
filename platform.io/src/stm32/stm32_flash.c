/*
 * Rad Pro
 * STM32 flash
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(STM32)

#include "../peripherals/flash.h"
#include "../system/cstring.h"
#include "../stm32/device.h"

#define FIRMWARE_PAYLOAD_SIZE (FIRMWARE_SIZE - 0x4)
#define FIRMWARE_CRC (*(uint32_t *)(FIRMWARE_BASE + FIRMWARE_PAYLOAD_SIZE))

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

__asm__(".section .comment\n"
        ".string \"FLASH_BASE: " TOSTRING(FLASH_BASE_) "\"\n"
        ".string \"FLASH_SIZE: " TOSTRING(FLASH_SIZE_) "\"\n"
        ".string \"FIRMWARE_BASE: " TOSTRING(FIRMWARE_BASE) "\"\n"
        ".string \"FIRMWARE_SIZE: " TOSTRING(FIRMWARE_SIZE) "\"\n"
        ".string \"STATES_BASE: " TOSTRING(STATES_BASE) "\"\n"
        ".string \"STATES_SIZE: " TOSTRING(STATES_SIZE) "\"\n"
        ".string \"DATALOG_BASE: " TOSTRING(DATALOG_BASE) "\"\n"
        ".string \"DATALOG_SIZE: " TOSTRING(DATALOG_SIZE) "\"\n"
        ".section .text\n");

#if defined(CH32F2)
// The CH32F2 has the bad habit of erasing flash memory to 0xe339.
#define FLASH_INVERT16 0x1cc6
#define FLASH_INVERT32 0x1cc61cc6

static uint8_t flashBuffer[FLASH_PAGE_SIZE];
#endif

void initFlash(void)
{
}

bool verifyFlash(void)
{
    rcc_enable_crc();

    crc_reset();

    for (uint32_t i = 0; i < FIRMWARE_PAYLOAD_SIZE; i += 4)
        crc_write(*(uint32_t *)(FIRMWARE_BASE + i));
    uint32_t crc = crc_read();

    rcc_disable_crc();

    return (crc == FIRMWARE_CRC);
}

const uint8_t *readFlash(uint32_t source, uint32_t count)
{
#if defined(CH32F2)
    for (uint32_t i = 0; i < count; i += 4)
    {
        uint32_t value = *(uint32_t *)(source + i) ^ FLASH_INVERT32;
        *(uint32_t *)(flashBuffer + i) = value;
    }

    return flashBuffer;
#else
    return (uint8_t *)source;
#endif
}

bool writeFlash(uint32_t dest, const uint8_t *source, uint32_t count)
{
    flash_unlock();

    bool errors = false;
    for (uint32_t i = 0; i < count; i += FLASH_WORD_SIZE)
    {
        // Try twice
        bool success = false;
        for (uint32_t j = 0; j < 2; j++)
        {
#if defined(CH32F2)
            uint16_t value = *(uint16_t *)(source + i) ^ FLASH_INVERT16;
            if (flash_program((uint8_t *)dest + i, (uint8_t *)&value))
#else
            if (flash_program((uint8_t *)dest + i, source + i))
#endif
            {
                success = true;

                break;
            }
        }

        if (!success)
        {
            errors = true;

            break;
        }
    }

    flash_lock();

    return !errors;
}

bool eraseFlash(uint32_t dest)
{
    flash_unlock();

    // Try twice
    bool success = false;
    for (uint32_t i = 0; i < 2; i++)
    {
        if (flash_erase(dest))
        {
            success = true;

            break;
        }
    }

    flash_lock();

    return success;
}

#endif
