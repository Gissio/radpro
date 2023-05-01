/*
 * FS2011 Pro
 * Settings
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <limits.h>
#include <stdbool.h>

#include "main.h"
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
uint8_t eeprom[65536];
#define SETTINGS_PAGE_BASE eeprom
#endif

#define SETTINGS_PAGE_SIZE 0x400
#define SETTINGS_PAGE_START 0x30
#define SETTINGS_PAGE_END 0x40
#define SETTINGS_PER_PAGE (SETTINGS_PAGE_SIZE / sizeof(settings))

void updateTubeType(void)
{
    float cpmPerUSvH;
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

    default:
        cpmPerUSvH = 1;
        break;
    }

    units[0].rate.scale = (60 * 1E-6F) / cpmPerUSvH;
    units[0].dose.scale = (60 * 1E-6F / 3600) / cpmPerUSvH;
    units[1].rate.scale = (60 * 1E-4F) / cpmPerUSvH;
    units[1].dose.scale = (60 * 1E-4F / 3600) / cpmPerUSvH;
}

static uint8_t *getSettingsAddress(uint32_t pageIndex, uint32_t index)
{
    return (uint8_t *)SETTINGS_PAGE_BASE +
           SETTINGS_PAGE_SIZE * pageIndex +
           sizeof(settings) * index;
}

static int32_t getLatestSettingsPageIndex(void)
{
    for (uint32_t pageIndex = SETTINGS_PAGE_START;
         pageIndex < SETTINGS_PAGE_END;
         pageIndex++)
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

static int32_t getLatestSettingsIndex(uint32_t pageIndex)
{
    Settings *page = (Settings *)getSettingsAddress(pageIndex, 0);
    for (int32_t index = (SETTINGS_PER_PAGE - 1); index >= 0; index--)
    {
        if (page[index].lifeTimer != ULONG_MAX)
            return index;
    }

    return -1;
}

static void eraseSettingsPage(int32_t pageIndex)
{
#ifndef SDL_MODE
    flash_erase_page(pageIndex);
#else
    printf("Erasing page %d\n", pageIndex);

    uint8_t *dest = getSettingsAddress(pageIndex, 0);
    for (uint32_t i = 0; i < SETTINGS_PAGE_SIZE; i++)
        dest[i] = 0xff;
#endif
}

static void writeSettingsToPage(int32_t pageIndex, int32_t index)
{
    uint32_t *source = (uint32_t *)&settings;
    uint32_t *dest = (uint32_t *)getSettingsAddress(pageIndex, index);

#ifndef SDL_MODE
    for (uint32_t i = 0; i < (sizeof(settings) / 4); i++)
        flash_program_word((uint32_t)(dest + i), source[i]);
#else
    printf("Writing settings to pageIndex %d, index %d, address %04x\n",
           pageIndex, index, (uint32_t)((uint8_t *)dest - eeprom));

    *((Settings *)dest) = *((Settings *)source);
#endif
}

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

    settings.lifeTimer = 0;
    settings.lifeCounts = 0;

#ifdef SDL_MODE
    for (uint32_t pageIndex = SETTINGS_PAGE_START;
         pageIndex < SETTINGS_PAGE_END;
         pageIndex++)
        eraseSettingsPage(pageIndex);
#endif

    int32_t pageIndex = getLatestSettingsPageIndex();
    if (pageIndex >= 0)
    {
        int32_t index = getLatestSettingsIndex(pageIndex);
        uint8_t *source = getSettingsAddress(pageIndex, index);

#ifdef SDL_MODE
        printf("Reading settings from pageIndex %d, index %d, address %04x\n",
               pageIndex, index, (uint32_t)(source - eeprom));
#endif
        settings = *((Settings *)source);
    }

    updateTubeType();
}

void writeSettings(void)
{
#ifndef SDL_MODE
    flash_unlock();
#endif

    int32_t pageIndex = getLatestSettingsPageIndex();
    int32_t index;

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
            uint32_t erasePageIndex = pageIndex + 1;
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
    flash_lock();
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

uint32_t getBacklightTime(uint32_t index)
{
    return backlightTime[index];
}
