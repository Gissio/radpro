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
    uint8_t begin;
    uint8_t end;
};

extern const uint32_t flashDataSize;
extern const struct FlashRegion flashSettingsRegion;
extern const struct FlashRegion flashDatalogRegion;

#if defined(SDLSIM)

void initFlash(void);

#endif

bool checkFirmware(void);

uint8_t *getFlashData(uint8_t pageIndex);
void eraseFlash(uint8_t pageIndex);
void writeFlash(uint8_t pageIndex, uint32_t index,
                uint8_t *source, uint32_t size);

bool isFlashPageFull(uint8_t pageIndex);
uint8_t getFlashNextPage(const struct FlashRegion *region, uint8_t pageIndex);
uint8_t getFlashHeadPage(const struct FlashRegion *region);
uint8_t getFlashTailPage(const struct FlashRegion *region);
void flashEntry(const struct FlashRegion *region,
                uint8_t pageIndex, uint32_t index,
                uint8_t *source, uint32_t size);

#endif
