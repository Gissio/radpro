/*
 * Rad Pro
 * Data logging
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "cmath.h"
#include "cstring.h"
#include "datalog.h"
#include "display.h"
#include "flash.h"
#include "measurements.h"
#include "menu.h"
#include "rtc.h"
#include "settings.h"

#define DATALOG_BUFFER_SIZE 16

enum DatalogWriteMode
{
    DATALOG_WRITEMODE_OPEN,
    DATALOG_WRITEMODE_REGULAR,
    DATALOG_WRITEMODE_CLOSE,
};

typedef struct
{
    FlashIterator iterator;

    uint32_t timeInterval;

    bool isNewLoggingSession;
    Dose record;
} DatalogState;

static struct
{
    bool writeOpen;
    DatalogState writeState;

    bool readOpen;
    DatalogState readState;

    uint32_t lastTimeFast;

    uint32_t bufferSize;
    uint8_t buffer[DATALOG_BUFFER_SIZE];
} datalog;

void onDatalogSubMenuBack(void);

static const uint16_t datalogIntervals[] = {
    0,
    60 * 60,
    10 * 60,
    1 * 60,
    10,
    1,
};

static cstring datalogIntervalMenuOptions[] = {
    STRING_OFF,
    STRING_EVERY_HOUR,
    STRING_EVERY10_MINUTES,
    STRING_EVERY_MINUTE,
    STRING_EVERY10_SECONDS,
    STRING_EVERY_SECOND,
    NULL,
};

static Menu datalogMenu;
static Menu datalogIntervalMenu;

static bool decodeDatalogRecord(DatalogState *state);

void resetDatalog(void)
{
    datalog.writeState.iterator.region = &flashDatalogRegion;

    setFlashPageHead(&datalog.writeState.iterator);

    while (decodeDatalogRecord(&datalog.writeState))
    {
        // Avoid deadlock
        if ((datalog.writeState.iterator.pageIndex == datalog.writeState.iterator.region->beginPageIndex) &&
            datalog.writeState.iterator.index == 0)
        {
            eraseFlashPage(datalog.writeState.iterator.region->beginPageIndex);

            break;
        }
    }

    selectMenuItem(&datalogMenu,
                   0,
                   0);
    selectMenuItem(&datalogIntervalMenu,
                   settings.datalogInterval,
                   DATALOG_INTERVAL_NUM);
}

// Datalog write

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

static bool isDatalogSpaceAvailable(uint32_t recordSize)
{
    uint32_t requiredSpace = datalog.bufferSize + recordSize;

    if (requiredSpace > flashWordSize)
        requiredSpace = flashWordSize + getFlashPaddedSize(recordSize);

    return (datalog.writeState.iterator.index + requiredSpace) <= flashPageDataSize;
}

static void writeDatalogBuffer(void)
{
    uint32_t paddedBufferSize = getFlashPaddedSize(datalog.bufferSize);

    memset(datalog.buffer + datalog.bufferSize,
           0xfe,
           paddedBufferSize - datalog.bufferSize);

    writeFlashPage(&datalog.writeState.iterator,
                   datalog.buffer,
                   paddedBufferSize);

    datalog.bufferSize = 0;
}

static void writeDatalog(enum DatalogWriteMode writeMode)
{
    if ((settings.datalogInterval == DATALOG_INTERVAL_OFF) ||
        !datalog.writeOpen ||
        datalog.readOpen)
        return;

    // Get dose relative to datalog open
    Dose dose;
    dose.time = getDeviceTime();
    dose.pulseCount = getTubePulseCount();

    uint32_t elapsedTime = dose.time - datalog.writeState.record.time;
    uint32_t datalogInterval = datalogIntervals[settings.datalogInterval];

    // Update?
    if ((writeMode == DATALOG_WRITEMODE_REGULAR) &&
        (elapsedTime < datalogInterval))
        return;

    // Build record
    uint8_t record[10];
    uint32_t recordSize;
    bool isAbsoluteRecord = (writeMode != DATALOG_WRITEMODE_REGULAR) ||
                            (elapsedTime > datalogInterval);

    if (!isAbsoluteRecord)
    {
        int32_t deltaPulseCount = dose.pulseCount - datalog.writeState.record.pulseCount;
        if ((deltaPulseCount >= -(1 << 27)) && (deltaPulseCount < (1 << 27)))
        {
            // Encode 7, 14, 21 or 28-bit differential pulse count
            if ((deltaPulseCount >= -(1 << 6)) && (deltaPulseCount < (1 << 6)))
                recordSize = 1;
            else if ((deltaPulseCount >= -(1 << 13)) && (deltaPulseCount < (1 << 13)))
                recordSize = 2;
            else if ((deltaPulseCount >= -(1 << 20)) && (deltaPulseCount < (1 << 20)))
                recordSize = 3;
            else
                recordSize = 4;
            encodeDatalogValue(deltaPulseCount,
                               record,
                               recordSize,
                               8 - recordSize);
        }
        else
        {
            // Encode 32-bit differential pulse count
            record[0] = 0xf0;
            encodeDatalogValue(deltaPulseCount,
                               record + 1,
                               4,
                               8);
            recordSize = 5;
        }

        if (!isDatalogSpaceAvailable(recordSize))
        {
            writeDatalogBuffer();

            setFlashPageState(&datalog.writeState.iterator,
                              FLASHPAGE_FULL);
        }
    }

    if (datalog.writeState.iterator.index == 0)
        isAbsoluteRecord = true;

    if (isAbsoluteRecord)
    {
        // Encode logging interval, absolute timestamp and absolute pulse count
        record[0] = 0xf1 + (settings.datalogInterval - 1);
        encodeDatalogValue(dose.time,
                           record + 1,
                           4,
                           8);
        encodeDatalogValue(dose.pulseCount,
                           record + 5,
                           4,
                           8);
        recordSize = 9;
    }

    datalog.writeState.record.time = dose.time;
    datalog.writeState.record.pulseCount = dose.pulseCount;

    // Insert open tag
    if (writeMode == DATALOG_WRITEMODE_OPEN)
    {
        for (int32_t i = recordSize - 1; i >= 0; i--)
            record[i + 1] = record[i];
        record[0] = 0xf8;
        recordSize++;
    }

    // Write datalog buffer if record does not fit
    if ((datalog.bufferSize + recordSize) > flashWordSize)
        writeDatalogBuffer();

    memcpy(datalog.buffer + datalog.bufferSize,
           record,
           recordSize);
    datalog.bufferSize += recordSize;

    if (datalog.bufferSize >= flashWordSize)
        writeDatalogBuffer();
}

void openDatalogWrite(void)
{
    datalog.writeOpen = true;

    writeDatalog(DATALOG_WRITEMODE_OPEN);
}

void closeDatalogWrite(void)
{
    writeDatalog(DATALOG_WRITEMODE_CLOSE);
    writeDatalogBuffer();

    datalog.writeOpen = false;
}

void updateDatalog(void)
{
    uint32_t timeFast = getDeviceTimeFast();
    if (timeFast == datalog.lastTimeFast)
        return;
    datalog.lastTimeFast = timeFast;

    writeDatalog(DATALOG_WRITEMODE_REGULAR);
}

void writeDatalogReset(void)
{
    writeDatalogBuffer();

    setFlashPageState(&datalog.writeState.iterator,
                      FLASHPAGE_RESET);

    resetHistory();
}

// Datalog read

static int32_t decodeDatalogValue(uint8_t *data,
                                  uint32_t size,
                                  uint32_t bitsInFirstByte)
{
    uint32_t bitshift = 32 + 8 - bitsInFirstByte - 8 * size;
    int32_t value = 0;

    for (uint32_t i = 0; i < size; i++)
    {
        uint8_t symbol = *data++;
        value = (value << 8) | symbol;
    }

    value = (value << bitshift) >> bitshift;

    return value;
}

static bool decodeDatalogRecord(DatalogState *state)
{
    state->isNewLoggingSession = false;

    while (true)
    {
        if (state->iterator.index >= flashPageDataSize)
        {
            if (getFlashPageState(&state->iterator) == FLASHPAGE_AVAILABLE)
                return false;

            setFlashPageNext(&state->iterator);
        }

        uint8_t *record = getFlashPage(state->iterator.pageIndex) +
                          state->iterator.index;

        uint8_t value = *record;
        if (value < 0xf0)
        {
            // 7, 14, 21 or 28-bit differential pulse count value
            uint32_t recordSize;
            if ((value & 0x80) == 0x00)
                recordSize = 1;
            else if ((value & 0xc0) == 0x80)
                recordSize = 2;
            else if ((value & 0xe0) == 0xc0)
                recordSize = 3;
            else
                recordSize = 4;

            state->record.time += state->timeInterval;
            state->record.pulseCount += decodeDatalogValue(record,
                                                           recordSize,
                                                           8 - recordSize);
            state->iterator.index += recordSize;

            return true;
        }
        else if (value == 0xf0)
        {
            // 32-bit data differential pulse count value
            state->record.time += state->timeInterval;
            state->record.pulseCount += decodeDatalogValue(record + 1,
                                                           4,
                                                           8);
            state->iterator.index += 5;

            return true;
        }
        else if (value < ((0xf1 - 1) + DATALOG_INTERVAL_NUM))
        {
            // Sample interval + absolute timestamp and pulse count value
            state->timeInterval = datalogIntervals[value - 0xf1 + 1];
            state->record.time = decodeDatalogValue(record + 1,
                                                    4,
                                                    8);
            state->record.pulseCount = decodeDatalogValue(record + 5,
                                                          4,
                                                          8);
            state->iterator.index += 9;

            return true;
        }
        else if (value == 0xf8)
        {
            state->isNewLoggingSession = true;
            state->iterator.index++;
        }
        else if (value < 0xff)
        {
            // Filler byte
            state->iterator.index++;
        }
        else // if (value == 0xff)
        {
            if (getFlashPageState(&state->iterator) == FLASHPAGE_AVAILABLE)
            {
                // End of data
                state->iterator.index = getFlashPaddedSize(state->iterator.index);

                return false;
            }
            else
            {
                // Move to next page
                state->iterator.index = flashPageDataSize;
            }
        }
    }
}

bool openDatalogRead(void)
{
    if (datalog.readOpen)
        return false;

    datalog.readOpen = true;

    datalog.readState.iterator.region = &flashDatalogRegion;

    setFlashPageTail(&datalog.readState.iterator);

    return true;
}

bool readDatalogRecord(bool *isNewLoggingSession, Dose *record)
{
    bool isRecordValid = decodeDatalogRecord(&datalog.readState);

    if (isRecordValid)
    {
        *isNewLoggingSession = datalog.readState.isNewLoggingSession;
        *record = datalog.readState.record;
    }

    return isRecordValid;
}

void closeDatalogRead(void)
{
    datalog.readOpen = false;
}

// Data log interval menu

static const char *onDatalogIntervalMenuGetOption(uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.datalogInterval);

    return getString(datalogIntervalMenuOptions[index]);
}

static void onDatalogIntervalMenuSelect(uint32_t index)
{
    if (settings.datalogInterval == index)
        return;

    settings.datalogInterval = index;

    if (index == DATALOG_INTERVAL_OFF)
        closeDatalogWrite();
    else
        openDatalogWrite();
}

static MenuState datalogIntervalMenuState;

static Menu datalogIntervalMenu = {
    STRING_LOGGING_MODE,
    &datalogIntervalMenuState,
    onDatalogIntervalMenuGetOption,
    onDatalogIntervalMenuSelect,
    onDatalogSubMenuBack,
};

static View datalogIntervalMenuView = {
    onMenuEvent,
    &datalogIntervalMenu,
};

// Data log reset confirmation view

static void onDatalogResetConfirmationEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
    case EVENT_KEY_SELECT:
        onDatalogSubMenuBack();

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_RESET),
                         getString(STRING_NOTIFICATION_DATALOG_RESET_SUCCESS));

        break;

    default:
        break;
    }
}

static View datalogResetConfirmationView = {
    onDatalogResetConfirmationEvent,
    NULL,
};

// Data log reset notification view

static void onDatalogResetAlertEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        onDatalogSubMenuBack();

        break;

    case EVENT_KEY_SELECT:
        writeDatalogReset();

        setView(&datalogResetConfirmationView);

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_RESET),
                         getString(STRING_NOTIFICATION_DATALOG_RESET_CONFIRM));

        break;

    default:
        break;
    }
}

static View datalogResetAlertView = {
    onDatalogResetAlertEvent,
    NULL,
};

// Data log menu

static cstring datalogMenuOptions[] = {
    STRING_LOGGING_MODE,
    STRING_RESET,
    NULL,
};

static const char *onDatalogMenuGetOption(uint32_t index,
                                          MenuStyle *menuStyle)
{
    *menuStyle = (index == 0) ? MENUSTYLE_SUBMENU : 0;

    return getString(datalogMenuOptions[index]);
}

static void onDatalogMenuSelect(uint32_t index)
{
    setView((index == 0)
                ? &datalogIntervalMenuView
                : &datalogResetAlertView);
}

void onDatalogSubMenuBack(void)
{
    setView(&datalogMenuView);
}

static MenuState datalogMenuState;

static Menu datalogMenu = {
    STRING_DATALOG,
    &datalogMenuState,
    onDatalogMenuGetOption,
    onDatalogMenuSelect,
    onSettingsSubMenuBack,
};

View datalogMenuView = {
    onMenuEvent,
    &datalogMenu,
};
