/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>

#include "flash.h"

static bool isProgrammed(uint32_t address, uint32_t size)
{
    uint32_t *source = (uint32_t *)getFlashMemory(address);
    for (uint32_t i = 0; i < size / 4; i++)
        if (source[i] != 0xffffffff)
            return true;

    return false;
}

static void erasePage(uint32_t address)
{
    if (!isProgrammed(address, flashPageSize))
        return;

    eraseFlash(address);
}

uint32_t getLastEntry(uint32_t addressStart, uint32_t addressEnd, uint32_t size)
{
    uint32_t selectedAddress = 0;

    for (uint32_t address = addressStart; address < addressEnd; address += size)
    {
        if (isProgrammed(address, size))
            selectedAddress = address;
        else
        {
            // Not programmed. Last found?
            if (selectedAddress)
                break;
        }
    }

    return selectedAddress;
}

void flashEntry(uint32_t addressStart, uint32_t addressEnd, uint32_t size, uint32_t *source)
{
    unlockFlash();

    uint32_t address = getLastEntry(addressStart, addressEnd, size);
    if (!address)
        address = addressStart;
    else
    {
        address += size;
        if (address >= addressEnd)
            address = addressStart;
    }
    uint32_t addressPage = address / flashPageSize;

    // Sanity check
    if (isProgrammed(address, size))
        erasePage(addressPage * flashPageSize);

    writeFlash(source, address, size);

    uint32_t nextAddress = address + size;
    if (nextAddress >= addressEnd)
        nextAddress = addressStart;
    uint32_t nextAddressPage = nextAddress / flashPageSize;

    if (addressPage != nextAddressPage)
        erasePage(nextAddress);

    lockFlash();
}
