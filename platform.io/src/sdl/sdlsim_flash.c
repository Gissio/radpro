/*
 * Rad Pro
 * Simulator flash
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>
#include <string.h>

#include "../flash.h"

#define FLASH_BASE 0x08000000
#define FLASH_PAGE_SIZE 0x400
#define FLASH_WORD_SIZE 0x2
#define FLASH_SIZE 0x10000
#define FIRMWARE_BASE 0x08000000
#define FIRMWARE_SIZE 0xa800

#define SETTINGS_PAGE_BEGIN ((FIRMWARE_BASE - FLASH_BASE + FIRMWARE_SIZE) / FLASH_PAGE_SIZE)
#define SETTINGS_PAGE_END (SETTINGS_PAGE_BEGIN + 1)
#define DATALOG_PAGE_BEGIN SETTINGS_PAGE_END
#define DATALOG_PAGE_END (FLASH_SIZE / FLASH_PAGE_SIZE)

#define FLASH_FILENAME "radpro-simulator-settings.bin"

uint8_t flashImage[FLASH_SIZE];

const FlashRegion flashSettingsRegion = {SETTINGS_PAGE_BEGIN, SETTINGS_PAGE_END};
const FlashRegion flashDatalogRegion = {DATALOG_PAGE_BEGIN, DATALOG_PAGE_END};

const uint32_t flashPageDataSize = FLASH_PAGE_SIZE - FLASH_WORD_SIZE;
const uint32_t flashWordSize = FLASH_WORD_SIZE;

void initFlash(void)
{
    memset(flashImage,
           0xff,
           sizeof(flashImage));

    FILE *fp = fopen(FLASH_FILENAME, "rb");
    if (fp)
    {
        fread(flashImage,
              1,
              sizeof(flashImage),
              fp);
        fclose(fp);
    }
}

bool verifyFlash(void)
{
    return true;
}

static void writeFlashImage(void)
{
    FILE *fp = fopen(FLASH_FILENAME, "wb");
    if (fp)
    {
        fwrite(flashImage,
               1,
               sizeof(flashImage),
               fp);
        fclose(fp);
    }
}

uint8_t *getFlashPage(PageIndex pageIndex)
{
    return flashImage + pageIndex * FLASH_PAGE_SIZE;
}

void eraseFlashPage(PageIndex pageIndex)
{
    uint32_t address = pageIndex * FLASH_PAGE_SIZE;

    memset(flashImage + address,
           0xff,
           FLASH_PAGE_SIZE);

    writeFlashImage();
}

void writeFlash(PageIndex pageIndex,
                uint32_t index,
                uint8_t *source,
                uint32_t size)
{
    uint32_t destAddress = pageIndex * FLASH_PAGE_SIZE + index;

    bool isError = false;
    for (uint32_t i = 0; i < size; i++)
        if (flashImage[destAddress + i] != 0xff)
            isError = true;
    if (isError)
        printf("error: writeFlash() writing to non-erased memory: 0x%02x 0x%03x 0x%03x\n",
               pageIndex, index, size);

    memcpy(flashImage + destAddress,
           source,
           size);

    writeFlashImage();
}

#endif
