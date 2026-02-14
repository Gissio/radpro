/*
 * Rad Pro
 * Simulator flash memory
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>
#include <string.h>

#include "../peripherals/flash.h"

#define FLASH_FILENAME "radpro-settings.bin"

uint8_t flashImage[FLASH_SIZE_];

static void loadFlashImage(void)
{
    FILE *fp = fopen(FLASH_FILENAME, "rb");
    if (fp)
    {
        fread(flashImage, 1, sizeof(flashImage), fp);
        fclose(fp);
    }
}

static void saveFlashImage(void)
{
    FILE *fp = fopen(FLASH_FILENAME, "wb");
    if (fp)
    {
        fwrite(flashImage, 1, sizeof(flashImage), fp);
        fclose(fp);
    }
}

void initFlash(void)
{
    memset(flashImage, 0xff, sizeof(flashImage));

    loadFlashImage();
}

bool verifyFlash(void)
{
    return true;
}

const uint8_t *readFlash(uint32_t source, uint32_t count)
{
    return flashImage + source;
}

bool writeFlash(uint32_t dest, const uint8_t *source, uint32_t count)
{
    printf("writeFlash(%08x, %08x)\n", dest, count);

    for (uint32_t i = 0; i < count; i++)
        if (flashImage[dest + i] != 0xff)
        {
            printf("writeFlash: writing to non-erased memory: 0x%08x\n", dest);

            return false;
        }

    memcpy(flashImage + dest, source, count);

    saveFlashImage();

    return true;
}

void eraseFlash(uint32_t dest)
{
    printf("eraseFlash(%08x)\n", dest);

    dest &= ~(FLASH_PAGE_SIZE - 1);

    memset(flashImage + dest, 0xff, FLASH_PAGE_SIZE);

    saveFlashImage();
}

#endif
