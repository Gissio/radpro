/*
 * Rad Pro
 * Data logging
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "cmath.h"
#include "datalog.h"
#include "flash.h"
#include "measurements.h"
#include "menu.h"
#include "rtc.h"
#include "settings.h"

#define DATALOG_BUFFER_SIZE 16

typedef struct
{
    FlashIterator iterator;

    uint32_t timeInterval;

    Dose dose;
} DatalogState;

static struct
{
    DatalogState writeState;
    DatalogState readState;

    uint32_t bufferSize;
    uint8_t buffer[DATALOG_BUFFER_SIZE];

    bool stopped;
} datalog;

static const uint16_t datalogTimeIntervals[] = {
    0,
    60 * 60,
    30 * 60,
    10 * 60,
    5 * 60,
    1 * 60,
};

static const char *const datalogMenuOptions[] = {
    "Off",
    "Every 60 minutes",
    "Every 30 minutes",
    "Every 10 minutes",
    "Every 5 minutes",
    "Every minute",
    NULL,
};

static const Menu datalogMenu;

static bool decodeDatalogEntry(DatalogState *state);

void initDatalog(void)
{
    selectMenuItem(&datalogMenu,
                   settings.datalogInterval,
                   DATALOGGING_NUM);

    datalog.writeState.iterator.region = &flashDatalogRegion;

    setFlashPageHead(&datalog.writeState.iterator);

    while (decodeDatalogEntry(&datalog.writeState))
        ;

    writeDatalogEntry(false);
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

static int32_t decodeDatalogValue(const uint8_t *data,
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

static void programDatalogBuffer(void)
{
    uint32_t paddedBufferSize =
        ((datalog.bufferSize + flashBlockSize - 1) / flashBlockSize) *
        flashBlockSize;

    memset(datalog.buffer + datalog.bufferSize, 0xfe, paddedBufferSize - datalog.bufferSize);

    programFlashPage(&datalog.writeState.iterator, datalog.buffer, paddedBufferSize);

    datalog.bufferSize = 0;
}

void startDatalog(void)
{
    datalog.stopped = false;
}

void stopDatalog(void)
{
    datalog.stopped = true;
}

void writeDatalogEntry(bool isUpdate)
{
    if ((settings.datalogInterval == DATALOGGING_OFF) ||
        datalog.stopped)
        return;

    // Update?

    Dose dose;
    dose.time = getRTCTime();
    dose.pulseCount = getTubePulseCount();

    uint32_t timeInterval = dose.time - datalog.writeState.dose.time;
    uint32_t sampleInterval = datalogTimeIntervals[settings.datalogInterval];

    if (isUpdate && (timeInterval < sampleInterval))
        return;

    // Build entry

    uint8_t entry[9];
    uint32_t entrySize = 0;

    bool absoluteEntry = false;

    if (!isUpdate || (timeInterval > (sampleInterval + 1)))
    {
        datalog.writeState.dose.time = dose.time;
        datalog.writeState.dose.pulseCount = dose.pulseCount;

        absoluteEntry = true;
    }
    else
    {
        int32_t deltaPulseCount = dose.pulseCount - datalog.writeState.dose.pulseCount;

        datalog.writeState.dose.time += sampleInterval;
        datalog.writeState.dose.pulseCount = dose.pulseCount;

        if ((deltaPulseCount >= -(1 << 27)) && (deltaPulseCount < (1 << 27)))
        {
            // Encode 7, 14, 21 or 28-bit differential pulse count value

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
            entrySize += size;
        }
        else
        {
            // Encode 32-bit differential pulse count value

            entry[entrySize++] = 0xf0;

            encodeDatalogValue(deltaPulseCount, entry + entrySize, 4, 8);
            entrySize += 4;
        }

        // Block crossing?

        if ((datalog.bufferSize + entrySize) > flashBlockSize)
            programDatalogBuffer();

        // Page crossing?

        absoluteEntry =
            (datalog.writeState.iterator.index + entrySize) > flashPageDataSize;
    }

    if (absoluteEntry)
    {
        // Encode sample interval, absolute timestamp and absolute pulse count value

        entrySize = 0;

        entry[entrySize++] = 0xf1 + settings.datalogInterval - 1;

        encodeDatalogValue(dose.time, entry + entrySize, 4, 8);
        entrySize += 4;

        encodeDatalogValue(dose.pulseCount, entry + entrySize, 4, 8);
        entrySize += 4;

        // An absolute entry always crosses a flash block: program buffered data

        programDatalogBuffer();
    }

    // Program entry

    memcpy(datalog.buffer + datalog.bufferSize, entry, entrySize);
    datalog.bufferSize += entrySize;

    if (datalog.bufferSize >= flashBlockSize)
        programDatalogBuffer();
}

static bool decodeDatalogEntry(DatalogState *state)
{
    while (true)
    {
        if (state->iterator.index >= flashPageDataSize)
        {
            if (!isFlashPageFull(&datalog.readState.iterator))
                return false;

            setFlashPageNext(&datalog.readState.iterator);
        }

        uint8_t *page = getFlash(&state->iterator);
        int32_t value = page[state->iterator.index];

        if (value < 0xf0)
        {
            // 7, 14, 21 or 28-bit differential pulse count value

            uint32_t size;

            if ((value & 0x80) == 0x00)
                size = 1;
            else if ((value & 0xc0) == 0x80)
                size = 2;
            else if ((value & 0xe0) == 0xc0)
                size = 3;
            else
                size = 4;

            state->dose.time += state->timeInterval;

            state->dose.pulseCount +=
                decodeDatalogValue(page + state->iterator.index, size, 8 - size);
            state->iterator.index += size;
        }
        else if (value == 0xf0)
        {
            // 32-bit data differential pulse count value

            state->dose.time += state->timeInterval;
            state->iterator.index++;

            state->dose.pulseCount +=
                decodeDatalogValue(page + state->iterator.index, 4, 8);
            state->iterator.index += 4;
        }
        else if (value <= 0xf5)
        {
            // Sample interval + absolute timestamp and pulse count value

            state->timeInterval = datalogTimeIntervals[value - 0xf1 + 1];
            state->iterator.index++;

            state->dose.time =
                decodeDatalogValue(page + state->iterator.index, 4, 8);
            state->iterator.index += 4;

            state->dose.pulseCount =
                decodeDatalogValue(page + state->iterator.index, 4, 8);
            state->iterator.index += 4;
        }
        else if (value < 0xff)
        {
            // Filler byte

            state->iterator.index++;

            continue;
        }
        else
        {
            // Unprogrammed entry

            return false;
        }

        return true;
    }
}

void initDatalogRead(void)
{
    datalog.readState.iterator.region = &flashDatalogRegion;

    setFlashPageTail(&datalog.readState.iterator);
}

bool readDatalog(Dose *dose)
{
    bool entryValid = decodeDatalogEntry(&datalog.readState);

    *dose = datalog.readState.dose;

    return entryValid;
}

// Data logging menu

static const char *onDatalogMenuGetOption(const Menu *menu,
                                          uint32_t index,
                                          MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.datalogInterval);

    return datalogMenuOptions[index];
}

static void onDatalogMenuSelect(const Menu *menu)
{
    uint32_t oldDatalogInterval = settings.datalogInterval;
    uint32_t newDatalogInterval = menu->state->selectedIndex;

    if (newDatalogInterval == DATALOGGING_OFF)
        writeDatalogEntry(false);

    settings.datalogInterval = newDatalogInterval;

    if (oldDatalogInterval == DATALOGGING_OFF)
        writeDatalogEntry(false);
}

static MenuState datalogMenuState;

static const Menu datalogMenu = {
    "Data logging",
    &datalogMenuState,
    onDatalogMenuGetOption,
    onDatalogMenuSelect,
    onSettingsSubMenuBack,
};

const View datalogMenuView = {
    onMenuEvent,
    &datalogMenu,
};
