/*
 * Rad Pro
 * Flash
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(FLASH_H)

#define FLASH_H

#include <stdbool.h>
#include <stdint.h>

struct FlashRegion
{
    uint8_t beginPageIndex;
    uint8_t endPageIndex;
};

extern const struct FlashRegion flashSettingsRegion;
extern const struct FlashRegion flashDatalogRegion;

extern const uint32_t flashPageDataSize;
extern const uint32_t flashBlockSize;

struct FlashIterator
{
    const struct FlashRegion *region;

    uint8_t pageIndex;
    uint32_t index;
};

void initFlash(void);

bool verifyFlash(void);

uint8_t *getFlash(struct FlashIterator *iterator);
void eraseFlash(struct FlashIterator *iterator);
void programFlash(struct FlashIterator *iterator,
                  uint8_t *source, uint32_t size);

bool isFlashPageFull(struct FlashIterator *iterator);
void setFlashPageHead(struct FlashIterator *iterator);
void setFlashPageTail(struct FlashIterator *iterator);
void setFlashPageNext(struct FlashIterator *iterator);
void setFlashPagePrev(struct FlashIterator *iterator);
void programFlashPage(struct FlashIterator *iterator,
                      uint8_t *source, uint32_t size);

#endif
