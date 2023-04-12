/*
 * FS2011 Pro
 * Settings
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdbool.h>

#ifndef SDL_MODE
#include "main.h"
#endif

#include "measurements.h"
#include "settings.h"

UnitType units[] = {
    {{"Sv/h", (60 * 1E-6F), -6},
     {"Sv", (60 * 1E-6F / 3600), -6}},
    {{"rem/h", (60 * 1E-4F), -6},
     {"rem", (60 * 1E-4F / 3600), -6}},
    {{"cpm", 60, 0},
     {"counts", 1, 0}},
    {{"cps", 1, 0},
     {"counts", 1, 0}},
};

const float rateAlarmsSvH[] = {
    0,
    0.2E-6F,
    0.5E-6F,
    1E-6F,
    2E-6F,
    5E-6F,
    10E-6F,
    20E-6F,
    50E-6F,
    100E-6F,
};

const float doseAlarmsSv[] = {
    0,
    2E-6F,
    5E-6F,
    10E-6F,
    20E-6F,
    50E-6F,
    100E-6F,
    200E-6F,
    500E-6F,
    1000E-6F,
};

const uint32_t backlightTime[] = {
    0,
    10,
    60,
    0,
};

Settings settings;

#ifndef SDL_MODE
#define SETTINGS_PAGE_BASE FLASH_BASE
#else
#include <stdio.h>

uint8_t eeprom[65536];
#define SETTINGS_PAGE_BASE eeprom
#endif

#define SETTINGS_PAGE_SIZE 0x400
#define SETTINGS_PAGE_START 0x30
#define SETTINGS_PAGE_END 0x40
#define SETTINGS_PER_PAGE (SETTINGS_PAGE_SIZE / sizeof(settings))

void updateTubeType()
{
    float cpmPerUSvH = 1;

    switch (settings.tubeType)
    {
    case TUBE_HH614:
        cpmPerUSvH = 68.4F;
        break;

    case TUBE_M4011:
        cpmPerUSvH = 153.F;
        break;

    case TUBE_SBM20:
        cpmPerUSvH = 150.F;
        break;

    case TUBE_SI3BG:
        cpmPerUSvH = 1.61F;
        break;
    }

    units[0].rate.scale = (60 * 1E-6F) / cpmPerUSvH;
    units[0].dose.scale = (60 * 1E-6F / 3600) / cpmPerUSvH;
    units[1].rate.scale = (60 * 1E-4F) / cpmPerUSvH;
    units[1].dose.scale = (60 * 1E-4F / 3600) / cpmPerUSvH;
}

uint8_t *getSettingsAddress(uint32_t pageIndex, uint32_t index)
{
    return (uint8_t *)SETTINGS_PAGE_BASE +
           SETTINGS_PAGE_SIZE * pageIndex +
           sizeof(settings) * index;
}

int getLatestSettingsPageIndex()
{
    for (int pageIndex = SETTINGS_PAGE_START; pageIndex < SETTINGS_PAGE_END; pageIndex++)
    {
        Settings *page = (Settings *)getSettingsAddress(pageIndex, 0);
        if (page[SETTINGS_PER_PAGE - 1].validState == SETTING_INVALID)
        {
            if (page[0].validState == SETTING_VALID)
                return pageIndex;
            else if (pageIndex > SETTINGS_PAGE_START)
                return pageIndex - 1;
            else
                return -1;
        }
    }

    return -1;
}

int getLatestSettingsIndex(int pageIndex)
{
    Settings *page = (Settings *)getSettingsAddress(pageIndex, 0);
    for (int index = (SETTINGS_PER_PAGE - 1); index >= 0; index--)
    {
        if (page[index].lifeTimer != ULONG_MAX)
            return index;
    }

    return -1;
}

bool eraseSettingsPage(int pageIndex)
{
#ifndef SDL_MODE
    FLASH_EraseInitTypeDef eraseRequest;
    eraseRequest.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseRequest.PageAddress = pageIndex;
    eraseRequest.NbPages = 1;
    uint32_t error;

    return (HAL_FLASHEx_Erase(&eraseRequest,
                              &error) == HAL_OK);
#else
    // printf("Erasing page %d\n", pageIndex);

    uint8_t *dest = getSettingsAddress(pageIndex, 0);
    for (int i = 0; i < SETTINGS_PAGE_SIZE; i++)
        dest[i] = 0xff;

    return true;
#endif
}

bool writeSettingsToPage(int pageIndex, int index)
{
    bool success = true;

    uint32_t *source = (uint32_t *)&settings;
    uint32_t *dest = (uint32_t *)getSettingsAddress(pageIndex, index);

#ifndef SDL_MODE
    for (int i = 0; i < (sizeof(settings) / 4); i++)
        success &= (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                                      (uint32_t)(dest + i),
                                      source[i]) == HAL_OK);
#else
    // printf("Writing settings to pageIndex %d, index %d, address %04x\n",
    //        pageIndex, index, (int)((uint8_t *)dest - eeprom));

    *((Settings *)dest) = *((Settings *)source);
    success = true;
#endif

    return success;
}

void readSettings()
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

    settings.lifeTimer = 0;
    settings.lifeCounts = 0;

#ifdef SDL_MODE
    for (int pageIndex = SETTINGS_PAGE_START;
         pageIndex < SETTINGS_PAGE_END;
         pageIndex++)
        eraseSettingsPage(pageIndex);
#endif

    int pageIndex = getLatestSettingsPageIndex();
    if (pageIndex >= 0)
    {
        int index = getLatestSettingsIndex(pageIndex);
        uint8_t *source = getSettingsAddress(pageIndex, index);

#ifdef SDL_MODE
        // printf("Reading settings from pageIndex %d, index %d, address %04x\n",
        //        pageIndex, index, (int)(source - eeprom));
#endif
        settings = *((Settings *)source);
    }

    updateTubeType();
}

void writeSettings()
{
#ifndef SDL_MODE
    HAL_FLASH_Unlock();
#endif

    int pageIndex = getLatestSettingsPageIndex();
    int index;

    if (pageIndex < 0)
    {
        pageIndex = SETTINGS_PAGE_START;
        index = 0;

        // Just in case: fix invalid condition
        if (getLatestSettingsIndex(pageIndex) >= 0)
            eraseSettingsPage(pageIndex);
    }
    else
    {
        index = getLatestSettingsIndex(pageIndex) + 1;

        if (index == (SETTINGS_PER_PAGE - 1))
        {
            int erasePageIndex = pageIndex + 1;
            if (erasePageIndex >= SETTINGS_PAGE_END)
                erasePageIndex = SETTINGS_PAGE_START;

            if (getLatestSettingsIndex(erasePageIndex) >= 0)
                eraseSettingsPage(erasePageIndex);
        }
        else if (index == SETTINGS_PER_PAGE)
        {
            pageIndex++;
            index = 0;

            if (pageIndex >= SETTINGS_PAGE_END)
                pageIndex = SETTINGS_PAGE_START;
        }
    }

    writeSettingsToPage(pageIndex, index);

#ifndef SDL_MODE
    HAL_FLASH_Lock();
#endif
}

float getRateAlarmSvH(uint32_t index)
{
    return rateAlarmsSvH[index];
}

float getDoseAlarmSv(uint32_t index)
{
    return doseAlarmsSv[index];
}

int getBacklightTime(uint32_t index)
{
    return backlightTime[index];
}
