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
#include "../system/cmath.h"
#include "../system/cstring.h"
#include "../stm32/device.h"

#define FIRMWARE_PAYLOAD_SIZE (FIRMWARE_SIZE - 0x4)
#define FIRMWARE_CRC (*(uint32_t *)(FIRMWARE_BASE + FIRMWARE_PAYLOAD_SIZE))

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#if (FLASH_WORD_SIZE != 2) && (FLASH_WORD_SIZE != 8)
#error "Unsupported FLASH_WORD_SIZE"
#endif

__asm__(".pushsection .comment\n"
        ".string \"FLASH_BASE: " TOSTRING(FLASH_BASE_) "\"\n"
        ".string \"FLASH_SIZE: " TOSTRING(FLASH_SIZE_) "\"\n"
        ".string \"FIRMWARE_BASE: " TOSTRING(FIRMWARE_BASE) "\"\n"
        ".string \"FIRMWARE_SIZE: " TOSTRING(FIRMWARE_SIZE) "\"\n"
        ".string \"STATES_BASE: " TOSTRING(STATES_BASE) "\"\n"
        ".string \"STATES_SIZE: " TOSTRING(STATES_SIZE) "\"\n"
        ".string \"DATALOG_BASE: " TOSTRING(DATALOG_BASE) "\"\n"
        ".string \"DATALOG_SIZE: " TOSTRING(DATALOG_SIZE) "\"\n"
        ".popsection\n");

#if defined(CH32F2)
// The CH32F2 has the bad habit of erasing flash memory to 0xe339.
#define FLASH_INVERT16 0x1cc6
#define FLASH_INVERT32 0x1cc61cc6

static uint8_t flashBuffer[FLASH_PAGE_SIZE];
#else
#define FLASH_INVERT16 0
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

    // Never overrun the buffer
    if (count > FLASH_PAGE_SIZE)
        count = FLASH_PAGE_SIZE;

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

NOINLINE static void startFlashOperation(uint32_t control)
{
    flash_unlock();
    flash_clear_status();
    FLASH->CR = control;
}

static bool finishFlashOperation(void)
{
    flash_wait_while_busy();

    // The CPU stalled while flash was busy: a pulse interrupt that was held
    // off has just run with a late timestamp, so the next pulse interval
    // would be too short for the dead-time measurement. An update event sets
    // the pulse timer's overflow flag, which makes the pulse interrupt skip
    // that interval.
    tim_generate_update(TUBE_DET_TIMER);

    // Clear the operation bits and lock in one store. Whole-register writes
    // are safe on all supported MCUs: the other lock bits (OPTLOCK, FLOCK)
    // are set-only, the interrupt enables are unused and the CH32F2's SCKMOD
    // must be 0 at 96 MHz anyway.
    FLASH->CR = FLASH_CR_LOCK;

    return !flash_has_errors();
}

bool writeFlash(uint32_t address, const uint8_t *data, uint32_t count)
{
    // No retries: a failed word is no longer erased, so reprogramming
    // it would fail as well.
    for (uint32_t i = 0; i < count; i += FLASH_WORD_SIZE)
    {
        startFlashOperation(FLASH_CR_PG);

#if FLASH_WORD_SIZE == 2
        *(__IO uint16_t *)(address + i) = __UNALIGNED_UINT16_READ(data + i) ^ FLASH_INVERT16;
#else
        uint32_t value[2];
        memcpy(value, data + i, sizeof(value));
        ((__IO uint32_t *)(address + i))[0] = value[0];
        ((__IO uint32_t *)(address + i))[1] = value[1];
#endif

        if (!finishFlashOperation())
            return false;
    }

    return true;
}

bool eraseFlash(uint32_t address)
{
    // Try twice
    for (uint32_t i = 0; i < 2; i++)
    {
#if defined(STM32F0) || defined(STM32F1)
        startFlashOperation(FLASH_CR_PER);
        FLASH->AR = address;
#else
        startFlashOperation(FLASH_CR_PER | flash_get_page_bits(address));
#endif
        set_bits(FLASH->CR, FLASH_CR_STRT);

        if (finishFlashOperation())
            return true;
    }

    return false;
}

#if defined(STM32G0) || defined(STM32L4)

// A power loss while programming can leave a flash doubleword with an
// uncorrectable ECC error, which raises an NMI when read. Clear the error
// and resume: the caller gets corrupt data, which it treats as a dirty page
// or an invalid state. Other NMI sources halt, as before.
void NMI_Handler(void)
{
    if (!flash_clear_ecc_double_error())
    {
        while (true)
            ;
    }
}

#endif

#endif
