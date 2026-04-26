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

const uint8_t *readFlash(uint32_t address, uint32_t count)
{
#if defined(CH32F2)
    // Align to 32-bit boundary
    uint32_t offset = address & 0b11;
    address -= offset;
    count += offset;

    // Invert bits to work around the CH32F2's bad flash erase behavior.
    for (uint32_t i = 0; i < count; i += 4)
    {
        uint32_t value = *(uint32_t *)(address + i) ^ FLASH_INVERT32;
        *(uint32_t *)(flashBuffer + i) = value;
    }

    return flashBuffer + offset;
#else
    return (uint8_t *)address;
#endif
}

#if FLASH_WORD_SIZE == 2

static uint16_t get_halfword(const uint8_t *data)
{
    return data[0] << 0 | data[1] << 8;
}

#endif

#if FLASH_WORD_SIZE == 8

static uint64_t get_doubleword(const uint8_t *data)
{
    return (uint64_t)data[0] << 0 | (uint64_t)data[1] << 8 | (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 |
           (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 | (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
}

#endif

bool writeFlash(uint32_t address, const uint8_t *data, uint32_t count)
{
    flash_unlock();

    bool errors = false;
    for (uint32_t i = 0; i < count; i += FLASH_WORD_SIZE)
    {
        // Try twice
        bool success = false;
        for (uint32_t j = 0; j < 2; j++)
        {
#if FLASH_WORD_SIZE == 2
            uint16_t value = get_halfword(data + i);
#if defined(CH32F2)
            value ^= FLASH_INVERT16;
#endif
            if (flash_program_halfword(address + i, value))
            {
                success = true;

                break;
            }
#elif FLASH_WORD_SIZE == 8
            uint64_t value = get_doubleword(data + i);
            if (flash_program_doubleword(address + i, value))
            {
                success = true;

                break;
            }
#endif
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

bool eraseFlash(uint32_t address)
{
    flash_unlock();

    // Try twice
    bool success = false;
    for (uint32_t i = 0; i < 2; i++)
    {
        if (flash_erase(address))
        {
            success = true;

            break;
        }
    }

    flash_lock();

    return success;
}

#endif
