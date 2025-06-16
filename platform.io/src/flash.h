/*
 * Rad Pro
 * Flash memory
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(FLASH_H)
#define FLASH_H

#include <stdbool.h>
#include <stdint.h>

typedef uint16_t PageIndex;

typedef struct
{
    PageIndex beginPageIndex;
    PageIndex endPageIndex;
} FlashRegion;

extern const FlashRegion flashSettingsRegion;
extern const FlashRegion flashDatalogRegion;

extern const uint32_t flashPageDataSize;
extern const uint32_t flashWordSize;

typedef struct
{
    const FlashRegion *region;

    PageIndex pageIndex;

    uint32_t index;
} FlashIterator;

typedef enum
{
    FLASHPAGE_FULL = 0x00,
    FLASHPAGE_RESET = 0x01,
    FLASHPAGE_AVAILABLE = 0xff,
} FlashPageState;

void initFlash(void);

bool verifyFlash(void);

uint8_t *getFlashPage(PageIndex pageIndex);
void eraseFlashPage(PageIndex pageIndex);
void writeFlash(PageIndex pageIndex,
                uint32_t index,
                uint8_t *source,
                uint32_t size);

bool isFlashEmpty(const uint8_t *data, uint32_t size);
uint32_t getFlashPaddedSize(uint32_t size);

FlashPageState getFlashPageState(const FlashIterator *iterator);
void setFlashPageState(FlashIterator *iterator,
                       FlashPageState pageState);
bool setFlashPageHead(FlashIterator *iterator);
bool setFlashPageTail(FlashIterator *iterator);
void setFlashPageNext(FlashIterator *iterator);
void setFlashPagePrev(FlashIterator *iterator);
void writeFlashPage(FlashIterator *iterator,
                    uint8_t *source,
                    uint32_t size);

#endif
