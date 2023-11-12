/*
 * Rad Pro
 * Flash
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "flash.h"

bool isFlashPageFull(struct FlashIterator *iterator)
{
    uint8_t *page = getFlash(iterator);

    return (page[flashPageDataSize] != 0xff);
}

static void markFlashPageFull(struct FlashIterator *iterator)
{
    uint8_t marker[8];
    memset(&marker, 0, sizeof(marker));

    iterator->index = flashPageDataSize;

    programFlash(iterator, marker, flashBlockSize);
}

void setFlashPageHead(struct FlashIterator *iterator)
{
    const struct FlashRegion *region = iterator->region;
    iterator->index = 0;

    for (iterator->pageIndex = region->beginPageIndex;
         iterator->pageIndex < region->endPageIndex;
         iterator->pageIndex++)
    {
        if (!isFlashPageFull(iterator))
            return;
    }

    iterator->pageIndex = region->beginPageIndex;
}

void setFlashPageTail(struct FlashIterator *iterator)
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

void setFlashPageNext(struct FlashIterator *iterator)
{
    const struct FlashRegion *region = iterator->region;
    iterator->pageIndex++;
    iterator->index = 0;

    if (iterator->pageIndex >= region->endPageIndex)
        iterator->pageIndex = region->beginPageIndex;
}

void setFlashPagePrev(struct FlashIterator *iterator)
{
    const struct FlashRegion *region = iterator->region;
    iterator->pageIndex--;
    iterator->index = 0;

    if (iterator->pageIndex < region->beginPageIndex)
        iterator->pageIndex = region->endPageIndex - 1;
}

void programFlashPage(struct FlashIterator *iterator,
                      uint8_t *source, uint32_t size)
{
    // Enough space?
    if ((iterator->index + size) > flashPageDataSize)
    {
        struct FlashIterator startIterator = *iterator;

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

    programFlash(iterator, source, size);

    iterator->index += size;
}
