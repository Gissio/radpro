/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef SDL_MODE

#include <libopencm3/stm32/flash.h>

#else

#include <stdio.h>

#endif

#include <limits.h>
#include <stdbool.h>

#include "measurements.h"
#include "settings.h"

Settings settings;
State state;

#ifndef SDL_MODE

#define FLASH_MEMORY ((uint8_t *)FLASH_BASE)

#else

uint8_t flashMemory[65536];

#define FLASH_MEMORY flashMemory

#endif

#define FLASH_PAGE_SIZE 0x400

#define SETTINGS_START (0x30 * FLASH_PAGE_SIZE)
#define SETTINGS_END (0x34 * FLASH_PAGE_SIZE)

#define STATE_START (0x34 * FLASH_PAGE_SIZE)
#define STATE_END (0x40 * FLASH_PAGE_SIZE)

static void erasePage(uint32_t address);
static uint32_t getLastEntry(uint32_t addressStart, uint32_t addressEnd, uint32_t size);

void initSettings(void)
{
    settings.units = UNITS_SIEVERTS;
    settings.history = HISTORY_2M;
    settings.rateAlarm = RATE_ALARM_OFF;
    settings.doseAlarm = DOSE_ALARM_OFF;
    settings.pulseSound = PULSE_SOUND_QUIET;
    settings.backlight = BACKLIGHT_10S;
    settings.batteryType = BATTERY_NI_MH;
    settings.tubeType = TUBE_HH614;
    settings.gameSkillLevel = 0;

    state.lifeTime = 0;
    state.lifePulseCount = 0;
    state.doseTime = 0;
    state.dosePulseCount = 0;

#ifdef SDL_MODE
    for (uint32_t address = SETTINGS_START;
         address < STATE_END;
         address += FLASH_PAGE_SIZE)
        erasePage(address);
#endif

    uint32_t address;

    address = getLastEntry(SETTINGS_START, SETTINGS_END, sizeof(Settings));
    if (address)
    {
#ifdef SDL_MODE
        printf("Reading %d bytes from %04x\n", (int)sizeof(Settings), address);
#endif
        settings = *(Settings *)(FLASH_MEMORY + address);
    }

    address = getLastEntry(STATE_START, STATE_END, sizeof(State));
    if (address)
    {
#ifdef SDL_MODE
        printf("Reading %d bytes from %04x\n", (int)sizeof(State), address);
#endif
        state = *(State *)(FLASH_MEMORY + address);

        setDose(state.doseTime, state.dosePulseCount);
    }
}

static bool isProgrammed(uint32_t address, uint32_t size)
{
    uint32_t *source = (uint32_t *)(FLASH_MEMORY + address);
    for (uint32_t i = 0; i < size / 4; i++)
        if (source[i] != 0xffffffff)
            return true;

    return false;
}

static void erasePage(uint32_t address)
{
    if (!isProgrammed(address, FLASH_PAGE_SIZE))
        return;

#ifndef SDL_MODE
    flash_erase_page(FLASH_BASE + address);
#else
    printf("Erasing page %04x\n", address);

    uint32_t *dest = (uint32_t *)(flashMemory + address);
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE / 4; i++)
        dest[i] = 0xffffffff;
#endif
}

static void flashData(uint32_t *source, uint32_t address, uint32_t size)
{
#ifndef SDL_MODE
    for (uint32_t i = 0; i < size / 4; i++)
        flash_program_word(FLASH_BASE + address + 4 * i, source[i]);
#else
    printf("Flashing %d bytes at %04x\n", size, address);

    uint32_t *dest = (uint32_t *)(FLASH_MEMORY + address);
    for (uint32_t i = 0; i < size / 4; i++)
        dest[i] = source[i];
#endif
}

static uint32_t getLastEntry(uint32_t addressStart, uint32_t addressEnd, uint32_t size)
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

static void flashEntry(uint32_t addressStart, uint32_t addressEnd, uint32_t size, uint32_t *source)
{
#ifndef SDL_MODE
    flash_unlock();
#endif

    uint32_t address = getLastEntry(addressStart, addressEnd, size);
    if (!address)
        address = addressStart;
    else
    {
        address += size;
        if (address >= addressEnd)
            address = addressStart;
    }
    uint32_t addressPage = address / FLASH_PAGE_SIZE;

    // Sanity check
    if (isProgrammed(address, size))
        erasePage(addressPage * FLASH_PAGE_SIZE);

    flashData(source, address, size);

    uint32_t nextAddress = address + size;
    if (nextAddress >= addressEnd)
        nextAddress = addressStart;
    uint32_t nextAddressPage = nextAddress / FLASH_PAGE_SIZE;

    if (addressPage != nextAddressPage)
        erasePage(nextAddress);

#ifndef SDL_MODE
    flash_lock();
#endif
}

void writeSettings(void)
{
    flashEntry(SETTINGS_START, SETTINGS_END, sizeof(Settings), (uint32_t *)&settings);
}

void writeState(void)
{
    getDose(&state.doseTime, &state.dosePulseCount);

    flashEntry(STATE_START, STATE_END, sizeof(State), (uint32_t *)&state);
}
