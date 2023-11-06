/*
 * Rad Pro
 * Data logging
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "datalog.h"
#include "flash.h"
#include "measurements.h"
#include "menu.h"
#include "rtc.h"
#include "settings.h"

static struct
{
    uint32_t lastEntryTime;

    bool pause;
} datalog;

static const uint16_t datalogIntervalTime[] = {
    0,
    60 * 60,
    30 * 60,
    10 * 60,
    5 * 60,
    1 * 60,
};

#define DATALOG_INTERVAL_TIME_NUM (sizeof(datalogIntervalTime) / sizeof(uint16_t))

static const char *const datalogMenuOptions[] = {
    "Off",
    "Every 60 minutes",
    "Every 30 minutes",
    "Every 10 minutes",
    "Every 5 minutes",
    "Every minute",
    NULL,
};

static const struct Menu datalogMenu;

void initDatalog(void)
{
    selectMenuIndex(&datalogMenu,
                    settings.datalogInterval,
                    DATA_LOGGING_NUM);

    writeDatalogEntry();
}

void setDatalogPause(bool value)
{
    datalog.pause = value;
}

static void encodeDatalogValue(int32_t value,
                               uint8_t *data,
                               uint32_t size,
                               uint32_t bitsInFirstByte)
{
    uint32_t bitshift = 8 * size - 8;
    uint32_t mask = (1 << bitsInFirstByte) - 1;
    uint32_t prefix = (~mask) << 1;

    data[0] = ((value >> bitshift) & mask) | prefix;
    for (uint32_t i = 1; i < size; i++)
        data[i] = value >> (bitshift - 8 * i);
}

static int32_t decodeDatalogValue(uint8_t *data,
                                  uint32_t size,
                                  uint32_t bitsInFirstByte)
{
    uint32_t bitshift = 32 + 8 - bitsInFirstByte - 8 * size;
    int32_t value = 0;

    for (uint32_t i = 0; i < size; i++)
        value = (value << 8) | data[i];
    value = (value << bitshift) >> bitshift;

    return value;
}

static bool decodeDatalogEntry(struct DatalogState *datalogState)
{
    if (datalogState->index >= flashDataSize)
        return false;

    uint8_t *page = getFlashData(datalogState->pageIndex);
    int32_t value = page[datalogState->index];

    if (value < 0xf0)
    {
        uint32_t size;
        if ((value & 0x80) == 0x00)
            size = 1;
        else if ((value & 0xc0) == 0x80)
            size = 2;
        else if ((value & 0xe0) == 0xc0)
            size = 3;
        else
            size = 4;

        datalogState->dose.time += datalogState->deltaTime;
        datalogState->dose.pulseCount +=
            decodeDatalogValue(page + datalogState->index, size, 8 - size);
        datalogState->index += size;
    }
    else if (value == 0xf0)
    {
        datalogState->dose.time += datalogState->deltaTime;
        datalogState->index++;
        datalogState->dose.pulseCount +=
            decodeDatalogValue(page + datalogState->index, 4, 8);
        datalogState->index += 4;
    }
    else if (value >= 0xf8 && value <= 0xfc)
    {
        datalogState->deltaTime = datalogIntervalTime[value - 0xf8 + 1];

        datalogState->index++;
        datalogState->dose.time =
            decodeDatalogValue(page + datalogState->index, 4, 8);
        datalogState->index += 4;
        datalogState->dose.pulseCount =
            decodeDatalogValue(page + datalogState->index, 4, 8);
        datalogState->index += 4;
    }
    else
        return false;

    return true;
}

void initDatalogState(struct DatalogState *state)
{
    state->pageIndex = getFlashTailPage(&flashDatalogRegion);
    state->index = 0;
}

bool updateDatalogState(struct DatalogState *state)
{
    if (decodeDatalogEntry(state))
        return true;

    if (!isFlashPageFull(state->pageIndex))
        return false;

    state->pageIndex = getFlashNextPage(
        &flashDatalogRegion, state->pageIndex);
    state->index = 0;

    return decodeDatalogEntry(state);
}

static void writeDatalogEntryWithTime(uint32_t time)
{
    struct DatalogState datalogState;
    datalogState.pageIndex = getFlashHeadPage(&flashDatalogRegion);
    datalogState.index = 0;

    while (decodeDatalogEntry(&datalogState));

    uint32_t deltaTime = time - datalog.lastEntryTime;
    uint32_t pulseCount = getTubePulseCount();

    datalog.lastEntryTime = time;

    uint8_t entry[9];
    uint32_t entryIndex;

    bool writeAbsoluteEntry;

    if (deltaTime == datalogState.deltaTime)
    {
        int32_t deltaPulseCount = pulseCount - datalogState.dose.pulseCount;

        entryIndex = 0;

        if ((deltaPulseCount >= -(1 << 27)) && (deltaPulseCount < (1 << 27)))
        {
            uint32_t size;
            if ((deltaPulseCount >= -(1 << 6)) && (deltaPulseCount < (1 << 6)))
                size = 1;
            else if ((deltaPulseCount >= -(1 << 13)) && (deltaPulseCount < (1 << 13)))
                size = 2;
            else if ((deltaPulseCount >= -(1 << 20)) && (deltaPulseCount < (1 << 20)))
                size = 3;
            else
                size = 4;

            encodeDatalogValue(deltaPulseCount, entry, size, 8 - size);
            entryIndex += size;
        }
        else
        {
            entry[entryIndex++] = 0xf0;

            encodeDatalogValue(deltaPulseCount, entry + entryIndex, 4, 8);
            entryIndex += 4;
        }

        writeAbsoluteEntry =
            ((datalogState.index + entryIndex) > flashDataSize);
    }
    else
        writeAbsoluteEntry = true;

    if (writeAbsoluteEntry)
    {
        entryIndex = 0;

        entry[entryIndex++] = 0xf8 + settings.datalogInterval - 1;
        encodeDatalogValue(time, entry + entryIndex, 4, 8);
        entryIndex += 4;
        encodeDatalogValue(pulseCount, entry + entryIndex, 4, 8);
        entryIndex += 4;
    }

    flashEntry(&flashDatalogRegion,
               datalogState.pageIndex, datalogState.index,
               entry, entryIndex);
}

void writeDatalogEntry(void)
{
    if (settings.datalogInterval == DATA_LOGGING_OFF)
        return;

    writeDatalogEntryWithTime(getRTCTime());
}

void updateDatalog(void)
{
    if ((settings.datalogInterval == DATA_LOGGING_OFF) ||
        datalog.pause)
        return;

    uint32_t time = getRTCTime();
    uint32_t nextWriteTime = datalog.lastEntryTime +
                        datalogIntervalTime[settings.datalogInterval];

    if (time >= nextWriteTime)
    {
        if (time == (nextWriteTime + 1))
            time = nextWriteTime;

        writeDatalogEntryWithTime(time);
    }
}

// Data logging menu

static void onDataLogMenuSelect(const struct Menu *menu)
{
    uint32_t datalogInterval = settings.datalogInterval;
    uint32_t datalogIntervalNext = menu->state->selectedIndex;

    if (datalogIntervalNext == DATA_LOGGING_OFF)
        writeDatalogEntry();

    settings.datalogInterval = datalogIntervalNext;

    if (datalogInterval == DATA_LOGGING_OFF)
        writeDatalogEntry();
}

static struct MenuState datalogMenuState;

static const struct Menu datalogMenu = {
    "Data logging",
    &datalogMenuState,
    onMenuGetOption,
    datalogMenuOptions,
    onDataLogMenuSelect,
    NULL,
    onSettingsSubMenuBack,
};

const struct View datalogMenuView = {
    onMenuEvent,
    &datalogMenu,
};
