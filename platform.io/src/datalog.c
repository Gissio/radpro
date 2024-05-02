/*
 * Rad Pro
 * Data logging
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include "cmath.h"
#include "cstring.h"
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

    uint8_t lock;

    uint32_t lastTimeFast;
} datalog;

static const uint16_t datalogTimeIntervals[] = {
    0,
    60 * 60,
    30 * 60,
    10 * 60,
    5 * 60,
    1 * 60,
    30,
    10,
};

static const char *const datalogMenuOptions[] = {
    "Off",
    "Every 60 Minutes",
    "Every 30 Minutes",
    "Every 10 Minutes",
    "Every 5 Minutes",
    "Every Minute",
    "Every 30 Seconds",
    "Every 10 Seconds",
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

    datalog.lastTimeFast = getDeviceTimeFast();
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

static int32_t decodeDatalogValue(FlashIterator *iterator,
                                  uint32_t size,
                                  uint32_t bitsInFirstByte)
{
    uint32_t bitshift = 32 + 8 - bitsInFirstByte - 8 * size;
    int32_t value = 0;

    for (uint32_t i = 0; i < size; i++)
    {
        uint8_t symbol;
        readFlash(iterator,
                  &symbol,
                  1);
        value = (value << 8) | symbol;
    }

    value = (value << bitshift) >> bitshift;

    return value;
}

static void writeDatalogBuffer(void)
{
    uint32_t paddedBufferSize =
        ((datalog.bufferSize + flashWordSize - 1) / flashWordSize) *
        flashWordSize;

    if (paddedBufferSize)
    {
        memset(datalog.buffer + datalog.bufferSize,
               0xfe,
               paddedBufferSize - datalog.bufferSize);

        writeFlashPage(&datalog.writeState.iterator,
                       datalog.buffer,
                       paddedBufferSize);
    }

    datalog.bufferSize = 0;
}

static void writeDatalogEntry(bool isUpdate)
{
    if ((settings.datalogInterval == DATALOGGING_OFF) ||
        datalog.lock)
        return;

    // Update?
    uint32_t timeFast = getDeviceTimeFast();
    if (timeFast == datalog.lastTimeFast)
        return;
    datalog.lastTimeFast = timeFast;

    Dose dose;
    dose.time = getDeviceTime();
    dose.pulseCount = getTubePulseCount();

    uint32_t timeInterval = dose.time - datalog.writeState.dose.time;
    uint32_t sampleInterval = datalogTimeIntervals[settings.datalogInterval];

    if (isUpdate && (timeInterval < sampleInterval))
        return;

    // Build entry
    uint8_t entry[9];
    uint32_t entrySize = 0;

    bool absoluteEntry = false;

    if (!isUpdate || (timeInterval > sampleInterval))
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
        if ((datalog.bufferSize + entrySize) > flashWordSize)
            writeDatalogBuffer();

        // Page crossing?
        absoluteEntry =
            (datalog.writeState.iterator.index + entrySize) > flashPageDataSize;
    }

    if (absoluteEntry)
    {
        // Encode sample interval, absolute timestamp and absolute pulse count value
        entrySize = 0;

        entry[entrySize++] = (0xf1 - 1) + settings.datalogInterval;

        encodeDatalogValue(dose.time, entry + entrySize, 4, 8);
        entrySize += 4;

        encodeDatalogValue(dose.pulseCount, entry + entrySize, 4, 8);
        entrySize += 4;

        // An absolute entry always crosses a flash block: write buffered data
        writeDatalogBuffer();
    }

    // Write entry
    memcpy(datalog.buffer + datalog.bufferSize,
           entry,
           entrySize);
    datalog.bufferSize += entrySize;

    if (datalog.bufferSize >= flashWordSize)
        writeDatalogBuffer();
}

void updateDatalog(void)
{
    writeDatalogEntry(true);
}

void writeDatalog(void)
{
    writeDatalogEntry(false);
}

void lockDatalog(void)
{
    datalog.lock++;
}

void unlockDatalog(void)
{
    datalog.lock--;
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

        uint8_t symbol;
        readFlash(&state->iterator,
                  &symbol,
                  1);

        if (symbol < 0xf0)
        {
            // 7, 14, 21 or 28-bit differential pulse count value
            uint32_t size;
            if ((symbol & 0x80) == 0x00)
                size = 1;
            else if ((symbol & 0xc0) == 0x80)
                size = 2;
            else if ((symbol & 0xe0) == 0xc0)
                size = 3;
            else
                size = 4;

            state->dose.time += state->timeInterval;

            state->iterator.index--;
            state->dose.pulseCount +=
                decodeDatalogValue(&state->iterator, size, 8 - size);
        }
        else if (symbol == 0xf0)
        {
            // 32-bit data differential pulse count value
            state->dose.time += state->timeInterval;
            state->dose.pulseCount +=
                decodeDatalogValue(&state->iterator, 4, 8);
        }
        else if (symbol < ((0xf1 - 1) + DATALOGGING_NUM))
        {
            // Sample interval + absolute timestamp and pulse count value
            state->timeInterval = datalogTimeIntervals[symbol - 0xf1 + 1];
            state->dose.time =
                decodeDatalogValue(&state->iterator, 4, 8);
            state->dose.pulseCount =
                decodeDatalogValue(&state->iterator, 4, 8);
        }
        else if (symbol < 0xff)
        {
            // Filler byte
            continue;
        }
        else
        {
            // Unflashed value
            if (isFlashPageFull(&state->iterator))
                continue;

            state->iterator.index--;

            return false;
        }

        return true;
    }
}

void startDatalogDownload(void)
{
    lockDatalog();

    datalog.readState.iterator.region = &flashDatalogRegion;

    setFlashPageTail(&datalog.readState.iterator);
}

bool getDatalogDownloadEntry(Dose *dose)
{
    bool entryValid = decodeDatalogEntry(&datalog.readState);

    if (entryValid)
        *dose = datalog.readState.dose;
    else
        unlockDatalog();

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
    "Data Logging",
    &datalogMenuState,
    onDatalogMenuGetOption,
    onDatalogMenuSelect,
    onSettingsSubMenuBack,
};

const View datalogMenuView = {
    onMenuEvent,
    &datalogMenu,
};
