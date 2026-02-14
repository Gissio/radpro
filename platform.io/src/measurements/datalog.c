/*
 * Rad Pro
 * Data log
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/datalog.h"
#include "../measurements/history.h"
#include "../peripherals/flash.h"
#include "../peripherals/rtc.h"
#include "../system/settings.h"
#include "../ui/menu.h"
#include "../ui/system.h"

#define DATALOG_BUFFER_SIZE 16

static Menu datalogMenu;
static Menu datalogModeMenu;

static void setDatalogMenu(void);

static struct
{
    uint32_t lastTimeFast;

    bool writing;
    uint32_t writePageBase;
    uint32_t writePageOffset;
    Dose writeDose;
    uint8_t writeBuffer[DATALOG_BUFFER_SIZE];
    uint32_t writeBufferLength;

    bool reading;
    uint32_t readPageBase;
    const uint8_t *readPage;
    uint32_t readPageOffset;
    uint32_t readTimeInterval;
} datalog;

static const uint8_t datalogEndOfRead = 0xff;

static cstring loggingModeMenuOptions[] = {
    STRING_OFF,
    STRING_EVERY_HOUR,
    STRING_EVERY10_MINUTES,
    STRING_EVERY_MINUTE,
    STRING_EVERY10_SECONDS,
    STRING_EVERY_SECOND,
    NULL,
};

static const uint16_t loggingModeIntervals[] = {
    0,
    60 * 60,
    10 * 60,
    1 * 60,
    10,
    1,
};

void setupDatalog(void)
{
    selectMenuItem(&datalogMenu, 0, 0);
    selectMenuItem(&datalogModeMenu, settings.loggingMode, DATALOG_LOGGINGMODE_NUM);
}

// Datalog pages

typedef enum
{
    PAGESTATE_FULL = 0x00,
    PAGESTATE_RESET = 0x01,
    PAGESTATE_WRITABLE = 0xff,
} PageState;

static uint32_t getPreviousPage(uint32_t pageBase)
{
    if (pageBase >= (DATALOG_BASE + DATALOG_PAGE_SIZE))
        return pageBase - DATALOG_PAGE_SIZE;
    else
        return DATALOG_END - DATALOG_PAGE_SIZE;
}

static uint32_t getNextPage(uint32_t pageBase)
{
    if (pageBase < (DATALOG_END - DATALOG_PAGE_SIZE))
        return pageBase + DATALOG_PAGE_SIZE;
    else
        return DATALOG_BASE;
}

static void readPage(void)
{
    datalog.readPage = readFlash(datalog.readPageBase, DATALOG_PAGE_SIZE);
    datalog.readPageOffset = 0;
}

static PageState getPageState(uint32_t pageBase)
{
    return *readFlash(pageBase + DATALOG_PAGESTATE_OFFSET, DATALOG_PAGESTATE_SIZE);
}

static void setPageState(PageState pageState)
{
    // Move to next page
    uint32_t previousPageBase = datalog.writePageBase;
    datalog.writePageBase = getNextPage(datalog.writePageBase);
    datalog.writePageOffset = 0;

    // Erase if not empty
    const uint8_t *page = readFlash(datalog.writePageBase, DATALOG_PAGE_SIZE);
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        if (page[i] != 0xff)
        {
            eraseFlash(datalog.writePageBase);

            break;
        }
    }

    // Mark previous page
    uint8_t buffer[FLASH_PAGE_SIZE];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = pageState;
    writeFlash(previousPageBase + DATALOG_PAGESTATE_OFFSET, buffer, DATALOG_PAGESTATE_SIZE);
}

static bool setDatalogTail(void)
{
    // Returns most recent datalog page
    for (uint32_t pageBase = DATALOG_BASE; pageBase < DATALOG_END; pageBase += DATALOG_PAGE_SIZE)
    {
        if (getPageState(pageBase) == PAGESTATE_WRITABLE)
        {
            datalog.readPageBase = pageBase;

            return true;
        }
    }

    datalog.readPageBase = DATALOG_BASE;

    return false;
}

static bool setDatalogHead(void)
{
    // Get most recent datalog page
    if (!setDatalogTail())
        return false;

    // Returns oldest datalog page
    while (true)
    {
        uint32_t previousPageBase = getPreviousPage(datalog.readPageBase);

        if (getPageState(previousPageBase) != PAGESTATE_FULL)
            return true;

        datalog.readPageBase = previousPageBase;
    }
}

// Datalog entries

static void writeDoubleWord(uint8_t *p, uint32_t value)
{
    p[0] = (value >> 24) & 0xff;
    p[1] = (value >> 16) & 0xff;
    p[2] = (value >> 8) & 0xff;
    p[3] = (value >> 0) & 0xff;
}

static uint32_t readDoubleWord(void)
{
    const uint8_t *p = datalog.readPage + datalog.readPageOffset;

    datalog.readPageOffset += 4;

    return (p[0] << 24) |
           (p[1] << 16) |
           (p[2] << 8) |
           (p[3] << 0);
}

static void flushDatalogBuffer(void)
{
    uint32_t alignedCount = (datalog.writeBufferLength + FLASH_WORD_SIZE - 1) & (~(FLASH_WORD_SIZE - 1));
    memset(datalog.writeBuffer + datalog.writeBufferLength, 0xfe, alignedCount - datalog.writeBufferLength);

    writeFlash(datalog.writePageBase + datalog.writePageOffset, datalog.writeBuffer, alignedCount);

    datalog.writePageOffset += alignedCount;
    datalog.writeBufferLength = 0;
}

static bool writeDatalogEntryToPage(const uint8_t *entry, uint32_t count)
{
    // Fail if entry does not fit in page
    if ((datalog.writePageOffset + datalog.writeBufferLength + count) > DATALOG_PAGESTATE_OFFSET)
        return false;

    // Enqueue
    memcpy(datalog.writeBuffer + datalog.writeBufferLength, entry, count);
    datalog.writeBufferLength += count;

    // Flush if buffer full
    if (datalog.writeBufferLength >= FLASH_WORD_SIZE)
        flushDatalogBuffer();

    return true;
}

static bool writeDatalogEntry(const uint8_t *entry, uint32_t count)
{
    if (writeDatalogEntryToPage(entry, count))
        return true;

    setPageState(PAGESTATE_FULL);

    return writeDatalogEntryToPage(entry, count);
}

static void writeDatalogSessionStart(void)
{
    const uint8_t sessionStartEntry[] = {0xf8};

    writeDatalogEntry(sessionStartEntry, sizeof(sessionStartEntry));
}

static void writeDatalogAbsoluteEntry(void)
{
    datalog.writeDose.time = getDeviceTime();
    datalog.writeDose.pulseCount = getTubePulseCount();

    uint8_t absoluteEntry[9];
    absoluteEntry[0] = 0xf0 + settings.loggingMode;
    writeDoubleWord(absoluteEntry + 1, datalog.writeDose.time);
    writeDoubleWord(absoluteEntry + 5, datalog.writeDose.pulseCount);
    writeDatalogEntry(absoluteEntry, sizeof(absoluteEntry));

    // Stop datalog read
    datalog.readPage = &datalogEndOfRead;
    datalog.readPageOffset = 0;
}

static uint32_t writeDatalogRelativeEntry(void)
{
    uint32_t pulseCount = getTubePulseCount();
    uint32_t value = pulseCount - datalog.writeDose.pulseCount;

    uint8_t entry[5];
    uint32_t entrySize;

    if (value < 0x80)
    {
        entry[0] = value;

        entrySize = 1;
    }
    else if (value < 0x4000)
    {
        entry[0] = 0x80 | (value >> 8);
        entry[1] = (value >> 0) & 0xff;

        entrySize = 2;
    }
    else if (value < 0x200000)
    {
        entry[0] = 0xc0 | (value >> 16);
        entry[1] = (value >> 8) & 0xff;
        entry[2] = (value >> 0) & 0xff;

        entrySize = 3;
    }
    else if (value < 0x10000000)
    {
        entry[0] = 0xe0 | (value >> 24);
        entry[1] = (value >> 16) & 0xff;
        entry[2] = (value >> 8) & 0xff;
        entry[3] = (value >> 0) & 0xff;

        entrySize = 4;
    }
    else
    {
        entry[0] = 0xf0;
        writeDoubleWord(entry + 1, value);

        entrySize = 5;
    }

    if (!writeDatalogEntryToPage(entry, entrySize))
        return false;

    datalog.writeDose.pulseCount = pulseCount;
    datalog.writeDose.time += loggingModeIntervals[settings.loggingMode];

    return true;
}

// Datalog interface

void initDatalog(void)
{
    // Get most recent datalog page
    if (!setDatalogTail())
    {
        // Recover when no pages are marked writable
        eraseFlash(DATALOG_BASE);
    }

    // Get most recent record
    readPage();

    DatalogRecord record;
    while (readDatalog(&record))
    {
        // Stop at end of page
        if (datalog.readPageOffset >= DATALOG_PAGESTATE_OFFSET)
            break;
    }

    datalog.writePageBase = datalog.readPageBase;
    // Recover non-aligned offset
    datalog.writePageOffset = datalog.readPageOffset & (~(FLASH_WORD_SIZE - 1));
}

void startDatalog(void)
{
    datalog.writing = true;

    writeDatalogSessionStart();
    writeDatalogAbsoluteEntry();
}

void stopDatalog(void)
{
    if (datalog.writing)
    {
        writeDatalogAbsoluteEntry();

        datalog.writing = false;
    }
}

void writeDatalogTimeChange(void)
{
    if (datalog.writing)
        writeDatalogAbsoluteEntry();
}

void resetDatalog(void)
{
    flushDatalogBuffer();
    setPageState(PAGESTATE_RESET);
    resetHistory();

    // Stop datalog read
    datalog.readPage = &datalogEndOfRead;
    datalog.readPageOffset = 0;
}

void updateDatalog(void)
{
    uint32_t timeFast = getDeviceTimeFast();
    if (timeFast != datalog.lastTimeFast)
    {
        datalog.lastTimeFast = timeFast;

        if (datalog.writing && !datalog.reading)
        {
            uint32_t time = getDeviceTime();
            uint32_t elapsedTime = time - datalog.writeDose.time;
            uint32_t loggingInterval = loggingModeIntervals[settings.loggingMode];

            if (elapsedTime == loggingInterval)
            {
                if (!writeDatalogRelativeEntry())
                    writeDatalogAbsoluteEntry();
            }
            else if (elapsedTime > loggingInterval)
                writeDatalogAbsoluteEntry();
        }
    }
}

bool startDatalogRead(void)
{
    if (datalog.reading)
        return false;

    if (!setDatalogHead())
        return false;

    readPage();

    datalog.reading = true;

    return true;
}

bool readDatalog(DatalogRecord *record)
{
    record->sessionStart = false;

    while (true)
    {
        // Out of data?
        if (datalog.readPageOffset >= DATALOG_PAGESTATE_OFFSET)
        {
            if (datalog.readPage[DATALOG_PAGESTATE_OFFSET] == PAGESTATE_WRITABLE)
                break;

            datalog.readPageBase = getNextPage(datalog.readPageBase);
            readPage();
        }

        uint8_t byte1 = datalog.readPage[datalog.readPageOffset++];
        if (byte1 < 0x80)
        {
            // 7-bit differential value
            record->dose.pulseCount += byte1;
            record->dose.time += datalog.readTimeInterval;

            return true;
        }
        else if (byte1 < 0xc0)
        {
            // 14-bit differential value
            uint8_t byte2 = datalog.readPage[datalog.readPageOffset++];

            record->dose.pulseCount += ((byte1 & 0x3f) << 8) | byte2;
            record->dose.time += datalog.readTimeInterval;

            return true;
        }
        else if (byte1 < 0xe0)
        {
            // 21-bit differential value
            uint8_t byte2 = datalog.readPage[datalog.readPageOffset++];
            uint8_t byte3 = datalog.readPage[datalog.readPageOffset++];

            record->dose.pulseCount += ((byte1 & 0x1f) << 16) | (byte2 << 8) | byte3;
            record->dose.time += datalog.readTimeInterval;

            return true;
        }
        else if (byte1 < 0xf0)
        {
            // 28-bit differential value
            uint8_t byte2 = datalog.readPage[datalog.readPageOffset++];
            uint8_t byte3 = datalog.readPage[datalog.readPageOffset++];
            uint8_t byte4 = datalog.readPage[datalog.readPageOffset++];

            record->dose.pulseCount += ((byte1 & 0x0f) << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
            record->dose.time += datalog.readTimeInterval;

            return true;
        }
        else if (byte1 == 0xf0)
        {
            // 32-bit differential value
            record->dose.pulseCount += readDoubleWord();
            record->dose.time += datalog.readTimeInterval;

            return true;
        }
        else if (byte1 < (0xf0 + DATALOG_LOGGINGMODE_NUM))
        {
            // Time interval, absolute timestamp and pulse count value
            datalog.readTimeInterval = loggingModeIntervals[byte1 - 0xf0];

            record->dose.time = readDoubleWord();
            record->dose.pulseCount = readDoubleWord();

            return true;
        }
        else if (byte1 == 0xf8)
        {
            // Session start
            record->sessionStart = true;
        }
        else if (byte1 == 0xff)
        {
            if (datalog.readPage[DATALOG_PAGESTATE_OFFSET] == PAGESTATE_WRITABLE)
            {
                // End of data
                datalog.readPageOffset--;

                break;
            }
        }
    }

    datalog.reading = false;

    return false;
}

// Logging mode menu

static const char *onDatalogModeMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.loggingMode);

    return getString(loggingModeMenuOptions[index]);
}

static void onDatalogModeMenuSelect(uint32_t index)
{
    if (settings.loggingMode == index)
        return;

    if ((settings.loggingMode != DATALOG_LOGGINGMODE_OFF) && (index == DATALOG_LOGGINGMODE_OFF))
        stopDatalog();
    else if ((settings.loggingMode == DATALOG_LOGGINGMODE_OFF) && (index != DATALOG_LOGGINGMODE_OFF))
        startDatalog();

    settings.loggingMode = index;
}

static MenuState datalogModeMenuState;

static Menu datalogModeMenu = {
    STRING_LOGGING_MODE,
    &datalogModeMenuState,
    onDatalogModeMenuGetOption,
    onDatalogModeMenuSelect,
    setDatalogMenu,
};

static View datalogModeMenuView = {
    onMenuEvent,
    &datalogModeMenu,
};

// Data log reset confirmation view

static void onDatalogResetConfirmationEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
    case EVENT_KEY_SELECT:
        setDatalogMenu();

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_RESET), getString(STRING_NOTIFICATION_DATALOG_RESET_SUCCESS));

        break;

    default:
        break;
    }
}

static View datalogResetConfirmationView = {
    onDatalogResetConfirmationEvent,
    NULL,
};

// Data log reset alert view

static void onDatalogResetAlertEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setDatalogMenu();

        break;

    case EVENT_KEY_SELECT:
        resetDatalog();
        setView(&datalogResetConfirmationView);

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_RESET), getString(STRING_NOTIFICATION_DATALOG_RESET_CONFIRM));

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

static MenuOption datalogMenuOptions[] = {
    {STRING_LOGGING_MODE, &datalogModeMenuView},
    {STRING_RESET, &datalogResetAlertView},
    {NULL},
};

static const char *onDatalogMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == 0) ? MENUSTYLE_SUBMENU : 0;

    return getString(datalogMenuOptions[index].title);
}

static void onDatalogMenuSelect(uint32_t index)
{
    setView(datalogMenuOptions[index].view);
}

static MenuState datalogMenuState;

static Menu datalogMenu = {
    STRING_DATALOG,
    &datalogMenuState,
    onDatalogMenuGetOption,
    onDatalogMenuSelect,
    setSettingsMenu,
};

View datalogMenuView = {
    onMenuEvent,
    &datalogMenu,
};

static void setDatalogMenu(void)
{
    setView(&datalogMenuView);
}
