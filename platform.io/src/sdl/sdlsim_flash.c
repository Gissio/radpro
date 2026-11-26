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
#include <stdlib.h>
#include <string.h>

#include "../peripherals/flash.h"

#define FLASH_FILENAME "radpro-settings.bin"

uint8_t flashImage[FLASH_SIZE_];

static void loadFlashImage(void)
{
    FILE *fp = fopen(FLASH_FILENAME, "rb");
    if (fp)
    {
        size_t readBytes = fread(flashImage, 1, sizeof(flashImage), fp);
        if (readBytes != sizeof(flashImage))
            printf("warning: %s is truncated (%zu of %zu bytes)\n",
                   FLASH_FILENAME,
                   readBytes,
                   sizeof(flashImage));

        fclose(fp);
    }
}

static bool isFlashRangeValid(const char *function, uint32_t address, uint32_t count)
{
    if ((address > sizeof(flashImage)) ||
        (count > (sizeof(flashImage) - address)))
    {
        printf("error: %s out of range: 0x%08x (%u bytes)\n",
               function,
               address,
               count);

        return false;
    }

    return true;
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
    // printf("readFlash(%08x, %u)\n", source, count);

    // Callers cannot handle a failed read: stop here so the debugger catches it
    if (!isFlashRangeValid("readFlash", source, count))
        abort();

    return flashImage + source;
}

bool writeFlash(uint32_t dest, const uint8_t *source, uint32_t count)
{
    // printf("writeFlash(%08x, %08x)\n", dest, count);

    if (!isFlashRangeValid("writeFlash", dest, count))
        return false;

    for (uint32_t i = 0; i < count; i++)
        if (flashImage[dest + i] != 0xff)
        {
            printf("warning: writeFlash writing to non-erased memory: 0x%08x\n", dest);

            return false;
        }

    memcpy(flashImage + dest, source, count);

    saveFlashImage();

    return true;
}

bool eraseFlash(uint32_t dest)
{
    // printf("eraseFlash(%08x)\n", dest);

    dest &= ~(FLASH_PAGE_SIZE - 1);

    if (!isFlashRangeValid("eraseFlash", dest, FLASH_PAGE_SIZE))
        return false;

    memset(flashImage + dest, 0xff, FLASH_PAGE_SIZE);

    saveFlashImage();

    return true;
}

#endif
