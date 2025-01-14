/*
 * Rad Pro
 * Flash memory
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "cstring.h"
#include "flash.h"

bool isFlashEmpty(uint8_t *data,
                  uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        if (data[i] != 0xff)
            return false;
    }

    return true;
}

uint32_t getFlashPaddedSize(uint32_t size)
{
    return ((size + flashWordSize - 1) / flashWordSize) * flashWordSize;
}

FlashPageState getFlashPageState(FlashIterator *iterator)
{
    uint8_t *data = getFlashPage(iterator->pageIndex);

    return data[flashPageDataSize];
}

void setFlashPageState(FlashIterator *iterator,
                       FlashPageState pageState)
{
    // Write in next page
    FlashIterator prevPageIterator = *iterator;

    setFlashPageNext(iterator);

    if (iterator->pageIndex == prevPageIterator.pageIndex)
        eraseFlashPage(iterator->pageIndex);
    else
    {
        uint8_t buffer[8];
        memset(&buffer,
               0,
               flashWordSize);
        buffer[0] = pageState;

        writeFlash(prevPageIterator.pageIndex,
                   flashPageDataSize,
                   buffer,
                   flashWordSize);

        if (getFlashPageState(iterator) != FLASHPAGE_AVAILABLE)
            eraseFlashPage(iterator->pageIndex);
    }
}

bool setFlashPageHead(FlashIterator *iterator)
{
    iterator->index = 0;

    for (iterator->pageIndex = iterator->region->beginPageIndex;
         iterator->pageIndex < iterator->region->endPageIndex;
         iterator->pageIndex++)
    {
        if (getFlashPageState(iterator) == FLASHPAGE_AVAILABLE)
            return true;
    }

    // Head not found
    iterator->pageIndex = iterator->region->beginPageIndex;
    eraseFlashPage(iterator->pageIndex);

    return false;
}

bool setFlashPageTail(FlashIterator *iterator)
{
    if (!setFlashPageHead(iterator))
        return false;

    uint8_t headPageIndex = iterator->pageIndex;
    uint8_t tailPageIndex = iterator->pageIndex;

    while (true)
    {
        setFlashPagePrev(iterator);

        if ((iterator->pageIndex == headPageIndex) ||
            getFlashPageState(iterator) != FLASHPAGE_FULL)
            break;

        tailPageIndex = iterator->pageIndex;
    }

    iterator->pageIndex = tailPageIndex;

    return true;
}

void setFlashPageNext(FlashIterator *iterator)
{
    iterator->pageIndex++;
    iterator->index = 0;

    const FlashRegion *region = iterator->region;
    if (iterator->pageIndex >= region->endPageIndex)
        iterator->pageIndex = region->beginPageIndex;
}

void setFlashPagePrev(FlashIterator *iterator)
{
    iterator->pageIndex--;
    iterator->index = 0;

    const FlashRegion *region = iterator->region;
    if (iterator->pageIndex < region->beginPageIndex)
        iterator->pageIndex = region->endPageIndex - 1;
}

void writeFlashPage(FlashIterator *iterator,
                    uint8_t *source,
                    uint32_t size)
{
    if ((iterator->index + size) > flashPageDataSize)
        setFlashPageState(iterator, FLASHPAGE_FULL);

    writeFlash(iterator->pageIndex,
               iterator->index,
               source,
               size);

    iterator->index += size;
}
