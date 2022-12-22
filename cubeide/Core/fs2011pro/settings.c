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
    {{"Sv/h", (60 * 1E-6F) / CPM_PER_USVH, -6},
     {"Sv", (60 * 1E-6F / 3600) / CPM_PER_USVH, -6}},
    {{"rem/h", (60 * 1E-4F) / CPM_PER_USVH, -6},
     {"rem", (60 * 1E-4F / 3600) / CPM_PER_USVH, -6}},
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

const unsigned int backlightTime[] = {
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

unsigned char eeprom[65536];
#define SETTINGS_PAGE_BASE eeprom
#endif

#define SETTINGS_PAGE_SIZE 0x400
#define SETTINGS_PAGE_START 0x30
#define SETTINGS_PAGE_END 0x40
#define SETTINGS_PER_PAGE (SETTINGS_PAGE_SIZE / sizeof(settings))

unsigned char *getSettingsAddress(unsigned int pageIndex, unsigned int index)
{
    return (unsigned char *)SETTINGS_PAGE_BASE +
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

    unsigned char *dest = getSettingsAddress(pageIndex, 0);
    for (int i = 0; i < SETTINGS_PAGE_SIZE; i++)
        dest[i] = 0xff;

    return true;
#endif
}

bool writeSettingsToPage(int pageIndex, int index)
{
    bool success = true;

    unsigned int *source = (unsigned int *)&settings;
    unsigned int *dest = (unsigned int *)getSettingsAddress(pageIndex, index);

#ifndef SDL_MODE
    for (int i = 0; i < (sizeof(settings) / 4); i++)
        success &= (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,
                                      (uint32_t)(dest + i),
                                      source[i]) == HAL_OK);
#else
    // printf("Writing settings to pageIndex %d, index %d, address %04x\n",
    //        pageIndex, index, (int)((unsigned char *)dest - eeprom));

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
        unsigned char *source = getSettingsAddress(pageIndex, index);

#ifdef SDL_MODE
        // printf("Reading settings from pageIndex %d, index %d, address %04x\n",
        //        pageIndex, index, (int)(source - eeprom));
#endif
        settings = *((Settings *)source);
    }
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

float getRateAlarmSvH(unsigned int index)
{
    return rateAlarmsSvH[index];
}

float getDoseAlarmSv(unsigned int index)
{
    return doseAlarmsSv[index];
}

int getBacklightTime(unsigned int index)
{
    return backlightTime[index];
}
