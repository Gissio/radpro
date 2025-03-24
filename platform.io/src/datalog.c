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

void onDatalogSubMenuBack(const Menu *menu);

static const uint16_t datalogIntervalTime[] = {
    0,
    60 * 60,
    10 * 60,
    1 * 60,
    10,
    1,
};

static const char *const datalogIntervalMenuOptions[] = {
    "Off",
    "Every hour",
    "Every 10 minutes",
    "Every minute",
    "Every 10 seconds",
    "Every second",
    NULL,
};

static const Menu datalogMenu;
static const Menu datalogIntervalMenu;

static bool decodeDatalogEntry(DatalogState *state);

void resetDatalog(void)
{
    datalog.writeState.iterator.region = &flashDatalogRegion;

    setFlashPageHead(&datalog.writeState.iterator);

    while (decodeDatalogEntry(&datalog.writeState))
    {
        // Avoid deadlock
        if ((datalog.writeState.iterator.pageIndex ==
             datalog.writeState.iterator.region->beginPageIndex) &&
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
    {
        uint8_t symbol = *data++;
        value = (value << 8) | symbol;
    }

    value = (value << bitshift) >> bitshift;

    return value;
}

static bool isDatalogSpaceAvailable(uint32_t entrySize)
{
    uint32_t requiredSpace = datalog.bufferSize + entrySize;

    if (requiredSpace > flashWordSize)
        requiredSpace =
            flashWordSize + getFlashPaddedSize(entrySize);

    return (datalog.writeState.iterator.index + requiredSpace) <=
           flashPageDataSize;
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

static void writeDatalogEntry(const uint8_t *entry,
                              uint32_t entrySize)
{
    // Write datalog buffer if entry does not fit
    if ((datalog.bufferSize + entrySize) > flashWordSize)
        writeDatalogBuffer();

    memcpy(datalog.buffer + datalog.bufferSize,
           entry,
           entrySize);
    datalog.bufferSize += entrySize;

    if (datalog.bufferSize >= flashWordSize)
        writeDatalogBuffer();
}

static void writeDatalogValue(bool forceWrite)
{
    if ((settings.datalogInterval == DATALOG_INTERVAL_OFF) ||
        datalog.lock)
        return;

    // Update?
    uint32_t timeFast = getDeviceTimeFast();
    if (!forceWrite &&
        (timeFast == datalog.lastTimeFast))
        return;
    datalog.lastTimeFast = timeFast;

    Dose dose;
    dose.time = getDeviceTime();
    dose.pulseCount = getTubePulseCount();

    uint32_t timeInterval = dose.time - datalog.writeState.dose.time;
    uint32_t sampleInterval = datalogIntervalTime[settings.datalogInterval];

    if (!forceWrite &&
        (timeInterval < sampleInterval))
        return;

    // Build entry
    uint8_t entry[9];
    uint32_t entrySize;
    bool isAbsoluteEntry = forceWrite ||
                           (timeInterval > sampleInterval);

    if (!isAbsoluteEntry)
    {
        int32_t deltaPulseCount = dose.pulseCount - datalog.writeState.dose.pulseCount;
        if ((deltaPulseCount >= -(1 << 27)) && (deltaPulseCount < (1 << 27)))
        {
            // Encode 7, 14, 21 or 28-bit differential pulse count
            if ((deltaPulseCount >= -(1 << 6)) && (deltaPulseCount < (1 << 6)))
                entrySize = 1;
            else if ((deltaPulseCount >= -(1 << 13)) && (deltaPulseCount < (1 << 13)))
                entrySize = 2;
            else if ((deltaPulseCount >= -(1 << 20)) && (deltaPulseCount < (1 << 20)))
                entrySize = 3;
            else
                entrySize = 4;
            encodeDatalogValue(deltaPulseCount,
                               entry,
                               entrySize,
                               8 - entrySize);
        }
        else
        {
            // Encode 32-bit differential pulse count
            entry[0] = 0xf0;
            encodeDatalogValue(deltaPulseCount,
                               entry + 1,
                               4,
                               8);
            entrySize = 5;
        }

        if (!isDatalogSpaceAvailable(entrySize))
        {
            writeDatalogBuffer();

            setFlashPageState(&datalog.writeState.iterator,
                              FLASHPAGE_FULL);
        }
    }

    if (datalog.writeState.iterator.index == 0)
        isAbsoluteEntry = true;

    if (isAbsoluteEntry)
    {
        // Encode logging interval, absolute timestamp and absolute pulse count
        entry[0] = 0xf1 + (settings.datalogInterval - 1);
        encodeDatalogValue(dose.time,
                           entry + 1,
                           4,
                           8);
        encodeDatalogValue(dose.pulseCount,
                           entry + 5,
                           4,
                           8);
        entrySize = 9;
    }

    datalog.writeState.dose.time = dose.time;
    datalog.writeState.dose.pulseCount = dose.pulseCount;

    writeDatalogEntry(entry, entrySize);
}

static void writeDatalogReset(void)
{
    writeDatalogBuffer();

    setFlashPageState(&datalog.writeState.iterator,
                      FLASHPAGE_RESET);
}

void openDatalog(void)
{
    writeDatalogValue(true);
}

void updateDatalog(void)
{
    writeDatalogValue(false);
}

void closeDatalog(void)
{
    writeDatalogBuffer();
}

static void lockDatalog(void)
{
    datalog.lock++;
}

static void unlockDatalog(void)
{
    datalog.lock--;
}

static bool decodeDatalogEntry(DatalogState *state)
{
    while (true)
    {
        if (state->iterator.index >= flashPageDataSize)
        {
            if (getFlashPageState(&state->iterator) == FLASHPAGE_AVAILABLE)
                return false;

            setFlashPageNext(&state->iterator);
        }

        uint8_t *entry = getFlashPage(state->iterator.pageIndex) +
                         state->iterator.index;
        uint32_t entrySize;

        uint8_t value = *entry;
        if (value < 0xf0)
        {
            // 7, 14, 21 or 28-bit differential pulse count value
            if ((value & 0x80) == 0x00)
                entrySize = 1;
            else if ((value & 0xc0) == 0x80)
                entrySize = 2;
            else if ((value & 0xe0) == 0xc0)
                entrySize = 3;
            else
                entrySize = 4;

            state->dose.time += state->timeInterval;
            state->dose.pulseCount +=
                decodeDatalogValue(entry,
                                   entrySize,
                                   8 - entrySize);
        }
        else if (value == 0xf0)
        {
            // 32-bit data differential pulse count value
            entrySize = 5;

            state->dose.time += state->timeInterval;
            state->dose.pulseCount +=
                decodeDatalogValue(entry + 1,
                                   4,
                                   8);
        }
        else if (value < ((0xf1 - 1) + DATALOG_INTERVAL_NUM))
        {
            // Sample interval + absolute timestamp and pulse count value
            entrySize = 9;

            state->timeInterval = datalogIntervalTime[value - 0xf1 + 1];
            state->dose.time =
                decodeDatalogValue(entry + 1,
                                   4,
                                   8);
            state->dose.pulseCount =
                decodeDatalogValue(entry + 5,
                                   4,
                                   8);
        }
        else if (value < 0xff)
        {
            // Filler byte
            state->iterator.index++;

            continue;
        }
        else
        {
            // Unflashed value
            if (getFlashPageState(&state->iterator) != FLASHPAGE_AVAILABLE)
                continue;

            state->iterator.index = getFlashPaddedSize(state->iterator.index);

            return false;
        }

        state->iterator.index += entrySize;

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

// Data log interval menu

static const char *onDatalogIntervalMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.datalogInterval);

    return datalogIntervalMenuOptions[index];
}

static void onDatalogIntervalMenuSelect(const Menu *menu)
{
    uint32_t datalogInterval = menu->state->selectedIndex;

    if (settings.datalogInterval == datalogInterval)
        return;

    settings.datalogInterval = datalogInterval;

    if (datalogInterval == DATALOG_INTERVAL_OFF)
        closeDatalog();
    else
        openDatalog();
}

static MenuState datalogIntervalMenuState;

static const Menu datalogIntervalMenu = {
    "Logging",
    &datalogIntervalMenuState,
    onDatalogIntervalMenuGetOption,
    onDatalogIntervalMenuSelect,
    onDatalogSubMenuBack,
};

static const View datalogIntervalMenuView = {
    onMenuEvent,
    &datalogIntervalMenu,
};

// Data log reset confirmation view

static void onDatalogResetConfirmationEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
    case EVENT_KEY_SELECT:
        onDatalogSubMenuBack(NULL);

        break;

    case EVENT_DRAW:
        drawNotification("SUCCESS",
                         "Data log has been reset.");

        break;

    default:
        break;
    }
}

static const View datalogResetConfirmationView = {
    onDatalogResetConfirmationEvent,
    NULL,
};

// Data log reset alert view

static void onDatalogResetAlertEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        onDatalogSubMenuBack(NULL);

        break;

    case EVENT_KEY_SELECT:
        writeDatalogReset();

        setView(&datalogResetConfirmationView);

        break;

    case EVENT_DRAW:
        drawNotification("Reset data log?",
                         "All records will be deleted.");

        break;

    default:
        break;
    }
}

static const View datalogResetAlertView = {
    onDatalogResetAlertEvent,
    NULL,
};

// Data log menu

static const char *const datalogMenuOptions[] = {
    "Logging",
    "Reset",
    NULL,
};

static const char *onDatalogMenuGetOption(const Menu *menu,
                                          uint32_t index,
                                          MenuStyle *menuStyle)
{
    *menuStyle = (index == 0) ? MENUSTYLE_SUBMENU : 0;

    return datalogMenuOptions[index];
}

static void onDatalogMenuSelect(const Menu *menu)
{
    setView((menu->state->selectedIndex == 0)
                ? &datalogIntervalMenuView
                : &datalogResetAlertView);
}

void onDatalogSubMenuBack(const Menu *menu)
{
    setView(&datalogMenuView);
}

static MenuState datalogMenuState;

static const Menu datalogMenu = {
    "Data log",
    &datalogMenuState,
    onDatalogMenuGetOption,
    onDatalogMenuSelect,
    onSettingsSubMenuBack,
};

const View datalogMenuView = {
    onMenuEvent,
    &datalogMenu,
};
