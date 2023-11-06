/*
 * Rad Pro
 * SDLSim flash
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdio.h>
#include <string.h>

#include "../flash.h"

#define FLASH_PAGE_SIZE 0x400
#define FLASH_DATA_SIZE (FLASH_PAGE_SIZE - 1)

uint8_t flashMemory[0x10000];

const struct FlashRegion flashSettingsRegion = {
    0x20,
    0x21,
};
const struct FlashRegion flashDatalogRegion = {
    0x21,
    0x40,
};

const uint32_t flashDataSize = FLASH_DATA_SIZE;

void initFlash(void)
{
    memset(flashMemory, 0xff, sizeof(flashMemory));

    FILE *fp = fopen("flash.bin", "rb");
    if (fp)
    {
        fread(flashMemory, 1, sizeof(flashMemory), fp);
        fclose(fp);
    }
}

bool checkFirmware(void)
{
    return true;
}

static void backupFlash(void)
{
    FILE *fp = fopen("flash.bin", "wb");
    if (fp)
    {
        fwrite(flashMemory, 1, sizeof(flashMemory), fp);
        fclose(fp);
    }
}

uint8_t *getFlashData(uint8_t pageIndex)
{
    return &flashMemory[pageIndex * FLASH_PAGE_SIZE];
}

void eraseFlash(uint8_t pageIndex)
{
    uint32_t offset = pageIndex * FLASH_PAGE_SIZE;

    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++)
        flashMemory[offset + i] = 0xff;

    backupFlash();
}

void writeFlash(uint8_t pageIndex, uint32_t index,
                uint8_t *source, uint32_t size)
{
    uint32_t dest = pageIndex * FLASH_PAGE_SIZE + index;

    // for (uint32_t i = 0; i < size; i++)
    //     flashMemory[dest + i] = source[i];

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

        *(uint32_t *)(flashMemory + dest) &= value;

        alignment = 0;
        dest += 4;
    }

    backupFlash();
}

#endif
