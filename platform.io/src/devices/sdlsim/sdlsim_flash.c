/*
 * Rad Pro
 * SDLSim flash interface
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef SDLSIM

#include <stdio.h>
#include <string.h>

#include "../../flash.h"

#define FLASH_PAGE_SIZE 0x400

uint8_t flashMemory[0x10000];

const uint32_t flashPageSize = FLASH_PAGE_SIZE;
const uint32_t flashSettingsStart = 0x24 * FLASH_PAGE_SIZE;
const uint32_t flashSettingsEnd = 0x26 * FLASH_PAGE_SIZE;
const uint32_t flashLifeStateStart = 0x26 * FLASH_PAGE_SIZE;
const uint32_t flashLifeStateEnd = 0x28 * FLASH_PAGE_SIZE;
const uint32_t flashDoseStateStart = 0x28 * FLASH_PAGE_SIZE;
const uint32_t flashDoseStateEnd = 0x40 * FLASH_PAGE_SIZE;

static void backupFlash(void)
{
    FILE *fp = fopen("flash.bin", "wb");
    if (fp)
    {
        fwrite(flashMemory, 1, 0x10000, fp);
        fclose(fp);
    }
}

void initFlash(void)
{
    memset(flashMemory, 0xff, sizeof(flashMemory));

    FILE *fp = fopen("flash.bin", "rb");
    if (fp)
    {
        fread(flashMemory, 1, 0x10000, fp);
        fclose(fp);
    }
}

void lockFlash(void)
{
}

void unlockFlash(void)
{
}

uint8_t *getFlashMemory(uint32_t address)
{
    return flashMemory + address;
}

void eraseFlash(uint32_t address)
{
    printf("Erasing page %04x\n", address);

    uint32_t *dest = (uint32_t *)(flashMemory + address);
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE / 4; i++)
        dest[i] = 0xffffffff;

    backupFlash();
}

void writeFlash(uint32_t *source, uint32_t address, uint32_t size)
{
    printf("Flashing %d bytes at %04x\n", size, address);

    uint32_t *dest = (uint32_t *)(flashMemory + address);
    for (uint32_t i = 0; i < size / 4; i++)
        dest[i] = source[i];

    backupFlash();
}

#endif
