/* Rad Pro H240 read-only firmware integrity check. License: MIT */

#if defined(CA32) && defined(H240)

#include "../peripherals/flash.h"

extern const uint8_t __image_start;
extern const uint32_t __image_crc;

void initFlash(void)
{
    // The application only reads memory-mapped flash.
}

bool verifyFlash(void)
{
    uint32_t crc = UINT32_MAX;
    uintptr_t end = (uintptr_t)&__image_crc;
    for (uintptr_t address = (uintptr_t)&__image_start; address < end; ++address)
    {
        crc ^= *(const volatile uint8_t *)address;
        for (unsigned bit = 0; bit < 8; ++bit)
            crc = (crc & 1U) ? (crc >> 1) ^ 0xedb88320U : crc >> 1;
    }

    return ~crc == *(const volatile uint32_t *)end;
}

#endif
