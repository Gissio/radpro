/*
 * Rad Pro
 * Flash
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "flash.h"

bool isFlashPageFull(uint8_t pageIndex)
{
    uint8_t *page = getFlashData(pageIndex);

    return (page[flashDataSize] != 0xff);
}

static void markFlashPageFull(uint8_t pageIndex)
{
    uint8_t marker = 0x00;

    writeFlash(pageIndex, flashDataSize, &marker, 1);
}

uint8_t getFlashNextPage(const struct FlashRegion *region, uint8_t pageIndex)
{
    pageIndex++;

    if (pageIndex >= region->end)
        pageIndex = region->begin;

    return pageIndex;
}

static uint8_t getFlashPrevPage(const struct FlashRegion *region, uint8_t pageIndex)
{
    pageIndex--;

    if (pageIndex < region->begin)
        pageIndex = region->end - 1;

    return pageIndex;
}

uint8_t getFlashHeadPage(const struct FlashRegion *region)
{
    for (uint8_t pageIndex = region->begin;
         pageIndex < region->end;
         pageIndex++)
    {
        if (!isFlashPageFull(pageIndex))
            return pageIndex;
    }

    return region->begin;
}

uint8_t getFlashTailPage(const struct FlashRegion *region)
{
    uint8_t headPageIndex = getFlashHeadPage(region);
    uint8_t tailPageIndex = headPageIndex;
    uint8_t pageIndex = headPageIndex;

    while (true)
    {
        pageIndex = getFlashPrevPage(region, pageIndex);

        if ((pageIndex == headPageIndex) ||
            !isFlashPageFull(pageIndex))
            break;

        tailPageIndex = pageIndex;
    }

    return tailPageIndex;
}

void flashEntry(const struct FlashRegion *region,
                uint8_t pageIndex, uint32_t index,
                uint8_t *source, uint32_t size)
{
    // Enough space?
    if ((index + size) > flashDataSize)
    {
        uint8_t nextPageIndex = getFlashNextPage(region, pageIndex);

        if (pageIndex != nextPageIndex)
        {
            markFlashPageFull(pageIndex);

            if (isFlashPageFull(nextPageIndex))
                eraseFlash(nextPageIndex);

            pageIndex = nextPageIndex;
        }
        else
            eraseFlash(pageIndex);

        index = 0;
    }

    writeFlash(pageIndex, index, source, size);
}
