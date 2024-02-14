/*
 * Rad Pro
 * Flash memory
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "flash.h"

bool isFlashPageFull(FlashIterator *iterator)
{
    uint8_t marker;

    uint32_t oldIndex = iterator->index;
    iterator->index = flashPageDataSize;

    readFlash(iterator,
              &marker,
              sizeof(marker));

    iterator->index = oldIndex;

    return (marker != 0xff);
}

static void markFlashPageFull(FlashIterator *iterator)
{
    uint8_t marker[8];
    memset(&marker,
           0,
           sizeof(marker));

    iterator->index = flashPageDataSize;

    writeFlash(iterator, marker, flashBlockSize);
}

void setFlashPageHead(FlashIterator *iterator)
{
    iterator->index = 0;

    for (iterator->pageIndex = iterator->region->beginPageIndex;
         iterator->pageIndex < iterator->region->endPageIndex;
         iterator->pageIndex++)
    {
        if (!isFlashPageFull(iterator))
            return;
    }

    iterator->pageIndex = iterator->region->beginPageIndex;
}

void setFlashPageTail(FlashIterator *iterator)
{
    setFlashPageHead(iterator);

    uint8_t headPageIndex = iterator->pageIndex;
    uint8_t tailPageIndex = iterator->pageIndex;

    while (true)
    {
        setFlashPagePrev(iterator);

        if ((iterator->pageIndex == headPageIndex) ||
            !isFlashPageFull(iterator))
            break;

        tailPageIndex = iterator->pageIndex;
    }

    iterator->pageIndex = tailPageIndex;
}

void setFlashPageNext(FlashIterator *iterator)
{
    const FlashRegion *region = iterator->region;
    iterator->pageIndex++;
    iterator->index = 0;

    if (iterator->pageIndex >= region->endPageIndex)
        iterator->pageIndex = region->beginPageIndex;
}

void setFlashPagePrev(FlashIterator *iterator)
{
    const FlashRegion *region = iterator->region;
    iterator->pageIndex--;
    iterator->index = 0;

    if (iterator->pageIndex < region->beginPageIndex)
        iterator->pageIndex = region->endPageIndex - 1;
}

void writeFlashPage(FlashIterator *iterator,
                    uint8_t *source, uint32_t size)
{
    // Enough space?
    if ((iterator->index + size) > flashPageDataSize)
    {
        FlashIterator startIterator = *iterator;

        setFlashPageNext(iterator);

        if (iterator->pageIndex != startIterator.pageIndex)
        {
            markFlashPageFull(&startIterator);

            if (isFlashPageFull(iterator))
                eraseFlash(iterator);
        }
        else
            eraseFlash(iterator);
    }

    writeFlash(iterator, source, size);
}
