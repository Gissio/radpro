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

#define DATALOG_PAGE_SIZE FLASH_PAGE_SIZE
#define DATALOG_PAGE_STATE_OFFSET (DATALOG_PAGE_SIZE - DATALOG_PAGE_STATE_SIZE)
#define DATALOG_PAGE_STATE_SIZE FLASH_WORD_SIZE

#define DATALOG_BUFFER_SIZE 16

#define DATALOG_ENTRY_RELATIVE_2BYTES 0x80
#define DATALOG_ENTRY_RELATIVE_3BYTES 0xc0
#define DATALOG_ENTRY_RELATIVE_4BYTES 0xe0
#define DATALOG_ENTRY_RELATIVE_5BYTES 0xf0
#define DATALOG_ENTRY_ABSOLUTE 0xf1
#define DATALOG_ENTRY_SESSION_START 0xf8
#define DATALOG_ENTRY_FILLER 0xfe
#define DATALOG_ENTRY_EMPTY 0xff

static Menu datalogMenu;
static Menu datalogModeMenu;

static void setDatalogMenu(void);

typedef struct
{
    bool active;
    uint32_t pageBase;
    uint32_t pageOffset;
    Dose dose;
    uint8_t buffer[DATALOG_BUFFER_SIZE];
    size_t bufferLength;
} DatalogWrite;

typedef struct
{
    bool active;
    uint32_t pageBase;
    const uint8_t *page;
    uint32_t pageOffset;
    uint32_t timeInterval;
} DatalogRead;

static struct
{
    uint32_t lastTimeFast;

    DatalogWrite write;
    DatalogRead read;
} datalog;

static const uint8_t datalogEndOfRead = DATALOG_ENTRY_EMPTY;

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
    datalog.read.page = readFlash(datalog.read.pageBase, DATALOG_PAGE_SIZE);
    datalog.read.pageOffset = 0;
}

static bool isPageEmpty(void)
{
    const uint8_t *pageData = readFlash(datalog.write.pageBase, DATALOG_PAGE_SIZE);

    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        if (pageData[i] != 0xff)
            return false;
    }

    return true;
}

static PageState readPageState(uint32_t pageBase)
{
    const uint8_t *pageStateData = readFlash(pageBase + DATALOG_PAGE_STATE_OFFSET, DATALOG_PAGE_STATE_SIZE);

    // Check bytes 1 to (DATALOG_PAGE_STATE_SIZE - 1)
    for (uint8_t i = 1; i < DATALOG_PAGE_STATE_SIZE; i++)
    {
        if (pageStateData[i] != 0xff)
            return PAGESTATE_RESET;
    }

    // Check byte 0
    PageState pageState = pageStateData[0];
    switch (pageState)
    {
    case PAGESTATE_FULL:
    case PAGESTATE_WRITABLE:
        return pageState;

    default:
        return PAGESTATE_RESET;
    }
}

static void writePageStateAndAdvance(PageState pageState)
{
    // Move to next page
    uint32_t previousPageBase = datalog.write.pageBase;
    datalog.write.pageBase = getNextPage(datalog.write.pageBase);
    datalog.write.pageOffset = 0;

    // Erase if not empty
    if (!isPageEmpty())
        eraseFlash(datalog.write.pageBase);

    // Mark previous page
    uint8_t buffer[DATALOG_PAGE_STATE_SIZE];
    memset(buffer, 0xff, sizeof(buffer));
    buffer[0] = pageState;
    writeFlash(previousPageBase + DATALOG_PAGE_STATE_OFFSET, buffer, DATALOG_PAGE_STATE_SIZE);
}

static bool setDatalogTail(void)
{
    // Gets most recent datalog page
    for (uint32_t pageBase = DATALOG_BASE; pageBase < DATALOG_END; pageBase += DATALOG_PAGE_SIZE)
    {
        if (readPageState(pageBase) == PAGESTATE_WRITABLE)
        {
            datalog.read.pageBase = pageBase;

            return true;
        }
    }

    datalog.read.pageBase = DATALOG_BASE;

    return false;
}

static bool setDatalogHead(void)
{
    // Get most recent datalog page
    if (!setDatalogTail())
        return false;

    // Move back to oldest datalog page
    while (true)
    {
        uint32_t previousPageBase = getPreviousPage(datalog.read.pageBase);

        if (readPageState(previousPageBase) != PAGESTATE_FULL)
            return true;

        datalog.read.pageBase = previousPageBase;
    }
}

// Datalog entries

static void encodeFixedUInt32(uint8_t *p, uint32_t value)
{
    p[0] = (value >> 24) & 0xff;
    p[1] = (value >> 16) & 0xff;
    p[2] = (value >> 8) & 0xff;
    p[3] = (value >> 0) & 0xff;
}

static uint32_t decodeFixedUInt32(void)
{
    const uint8_t *p = datalog.read.page + datalog.read.pageOffset;

    datalog.read.pageOffset += 4;

    return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3] << 0);
}

static uint32_t encodeVariableUInt32(uint8_t *p, uint32_t value)
{
    if (value < 0x80)
    {
        // 7-bit value
        p[0] = value;

        return 1;
    }
    else if (value < 0x4000)
    {
        // 14-bit value
        p[0] = DATALOG_ENTRY_RELATIVE_2BYTES | (value >> 8);
        p[1] = (value >> 0) & 0xff;

        return 2;
    }
    else if (value < 0x200000)
    {
        // 21-bit value
        p[0] = DATALOG_ENTRY_RELATIVE_3BYTES | (value >> 16);
        p[1] = (value >> 8) & 0xff;
        p[2] = (value >> 0) & 0xff;

        return 3;
    }
    else if (value < 0x10000000)
    {
        // 28-bit value
        p[0] = DATALOG_ENTRY_RELATIVE_4BYTES | (value >> 24);
        p[1] = (value >> 16) & 0xff;
        p[2] = (value >> 8) & 0xff;
        p[3] = (value >> 0) & 0xff;

        return 4;
    }
    else
    {
        // 32-bit value
        p[0] = DATALOG_ENTRY_RELATIVE_5BYTES;
        encodeFixedUInt32(p + 1, value);

        return 5;
    }
}

static bool decodeVariableUInt32(uint32_t *value)
{
    const uint8_t *p = datalog.read.page + datalog.read.pageOffset;
    uint8_t c = p[0];
    if (c < DATALOG_ENTRY_RELATIVE_2BYTES)
    {
        // 7-bit value
        *value = c;
        datalog.read.pageOffset += 1;
    }
    else if (c < DATALOG_ENTRY_RELATIVE_3BYTES)
    {
        // 14-bit value
        *value = ((c & 0x3f) << 8) | p[1];
        datalog.read.pageOffset += 2;
    }
    else if (c < DATALOG_ENTRY_RELATIVE_4BYTES)
    {
        // 21-bit value
        *value = ((c & 0x1f) << 16) | (p[1] << 8) | p[2];
        datalog.read.pageOffset += 3;
    }
    else if (c < DATALOG_ENTRY_RELATIVE_5BYTES)
    {
        // 28-bit value
        *value = ((c & 0x0f) << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
        datalog.read.pageOffset += 4;
    }
    else if (c == DATALOG_ENTRY_RELATIVE_5BYTES)
    {
        // 32-bit value
        datalog.read.pageOffset += 1;
        *value = decodeFixedUInt32();
    }
    else
        return false;

    return true;
}

static uint32_t alignDatalogBufferLength(uint32_t value)
{
    return (value + FLASH_WORD_SIZE - 1) & ~(FLASH_WORD_SIZE - 1);
}

static void flushDatalogBuffer(void)
{
    uint32_t alignedCount = alignDatalogBufferLength(datalog.write.bufferLength);
    memset(datalog.write.buffer + datalog.write.bufferLength, DATALOG_ENTRY_FILLER, alignedCount - datalog.write.bufferLength);

    writeFlash(datalog.write.pageBase + datalog.write.pageOffset, datalog.write.buffer, alignedCount);

    datalog.write.pageOffset += alignedCount;
    datalog.write.bufferLength = 0;
}

static bool appendDatalogEntryToCurrentPage(const uint8_t *entry, uint32_t count)
{
    // Fail if entry does not fit in page
    if ((datalog.write.pageOffset + datalog.write.bufferLength + count) > DATALOG_PAGE_STATE_OFFSET)
        return false;

    // Enqueue
    memcpy(datalog.write.buffer + datalog.write.bufferLength, entry, count);
    datalog.write.bufferLength += count;

    // Flush if buffer full
    if (datalog.write.bufferLength >= FLASH_WORD_SIZE)
        flushDatalogBuffer();

    return true;
}

static bool appendDatalogEntryWithPageRollover(const uint8_t *entry, uint32_t count)
{
    if (appendDatalogEntryToCurrentPage(entry, count))
        return true;

    writePageStateAndAdvance(PAGESTATE_FULL);

    return appendDatalogEntryToCurrentPage(entry, count);
}

static void writeDatalogSessionStart(void)
{
    const uint8_t sessionStartEntry[] = {DATALOG_ENTRY_SESSION_START};

    appendDatalogEntryWithPageRollover(sessionStartEntry, sizeof(sessionStartEntry));
}

static void appendDatalogAbsoluteEntry(void)
{
    datalog.write.dose.time = getDeviceTime();
    datalog.write.dose.pulseCount = getTubePulseCount();

    uint8_t absoluteEntry[9];
    absoluteEntry[0] = DATALOG_ENTRY_ABSOLUTE + (settings.loggingMode - 1);
    encodeFixedUInt32(absoluteEntry + 1, datalog.write.dose.time);
    encodeFixedUInt32(absoluteEntry + 5, datalog.write.dose.pulseCount);
    appendDatalogEntryWithPageRollover(absoluteEntry, sizeof(absoluteEntry));

    // Stop datalog read
    datalog.read.page = &datalogEndOfRead;
    datalog.read.pageOffset = 0;
}

static bool appendDatalogRelativeEntry(void)
{
    uint32_t pulseCount = getTubePulseCount();
    uint32_t value = pulseCount - datalog.write.dose.pulseCount;

    uint8_t entry[5];
    uint32_t entrySize = encodeVariableUInt32(entry, value);

    if (!appendDatalogEntryToCurrentPage(entry, entrySize))
        return false;

    datalog.write.dose.pulseCount = pulseCount;
    datalog.write.dose.time += loggingModeIntervals[settings.loggingMode];

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

    // Reset when settings were not loaded
    if (!isSettingsLoaded())
        resetDatalog();

    // Get most recent record
    readPage();

    DatalogRecord record;
    uint32_t readPageBase = datalog.read.pageBase;
    while (readDatalog(&record))
    {
        // Recover from page rollover
        if (datalog.read.pageBase != readPageBase)
            break;
    }

    // Validate rest of page empty
    for (uint32_t i = datalog.read.pageOffset; i < DATALOG_PAGE_STATE_OFFSET; i++)
    {
        if (datalog.read.page[i] != 0xff)
        {
            eraseFlash(datalog.read.pageBase);
            datalog.read.pageOffset = 0;

            break;
        }
    }

    // Set write head
    datalog.write.pageBase = datalog.read.pageBase;
    // Recover non-aligned offset
    datalog.write.pageOffset = datalog.read.pageOffset & (~(FLASH_WORD_SIZE - 1));
}

void startDatalog(void)
{
    datalog.write.active = true;

    writeDatalogSessionStart();
    appendDatalogAbsoluteEntry();
}

void stopDatalog(void)
{
    if (datalog.write.active)
    {
        appendDatalogAbsoluteEntry();

        datalog.write.active = false;
    }
}

void writeDatalogTimeChange(void)
{
    if (datalog.write.active)
        appendDatalogAbsoluteEntry();
}

void resetDatalog(void)
{
    flushDatalogBuffer();
    writePageStateAndAdvance(PAGESTATE_RESET);
    resetHistory();

    // Stop datalog read
    datalog.read.page = &datalogEndOfRead;
    datalog.read.pageOffset = 0;
}

void updateDatalog(void)
{
    uint32_t timeFast = getDeviceTimeFast();
    if (timeFast != datalog.lastTimeFast)
    {
        datalog.lastTimeFast = timeFast;

        if (datalog.write.active && !datalog.read.active)
        {
            uint32_t time = getDeviceTime();
            uint32_t elapsedTime = time - datalog.write.dose.time;
            uint32_t loggingInterval = loggingModeIntervals[settings.loggingMode];

            if (elapsedTime == loggingInterval)
            {
                if (!appendDatalogRelativeEntry())
                    appendDatalogAbsoluteEntry();
            }
            else if (elapsedTime > loggingInterval)
                appendDatalogAbsoluteEntry();
        }
    }
}

bool startDatalogRead(void)
{
    if (datalog.read.active)
        return false;

    if (!setDatalogHead())
        return false;

    readPage();

    datalog.read.active = true;

    return true;
}

bool readDatalog(DatalogRecord *record)
{
    record->sessionStart = false;

    while (true)
    {
        // Out of data?
        if (datalog.read.pageOffset >= DATALOG_PAGE_STATE_OFFSET)
        {
            if (datalog.read.page[DATALOG_PAGE_STATE_OFFSET] == PAGESTATE_WRITABLE)
                break;

            datalog.read.pageBase = getNextPage(datalog.read.pageBase);
            readPage();
        }

        uint32_t value;
        if (decodeVariableUInt32(&value))
        {
            record->dose.pulseCount += value;
            record->dose.time += datalog.read.timeInterval;

            return true;
        }
        else
        {
            const uint8_t *p = datalog.read.page + datalog.read.pageOffset;
            uint8_t c = p[0];
            if (c < (DATALOG_ENTRY_ABSOLUTE + (DATALOG_LOGGINGMODE_NUM - 1)))
            {
                // Time interval, absolute timestamp and pulse count value
                uint32_t mode = (c - DATALOG_ENTRY_ABSOLUTE) + 1;
                datalog.read.timeInterval = loggingModeIntervals[mode];

                datalog.read.pageOffset++;
                record->dose.time = decodeFixedUInt32();
                record->dose.pulseCount = decodeFixedUInt32();

                return true;
            }
            else if (c == DATALOG_ENTRY_SESSION_START)
            {
                // Session start
                record->sessionStart = true;

                datalog.read.pageOffset++;
            }
            else if (c < DATALOG_ENTRY_EMPTY)
            {
                // Fill bytes
                datalog.read.pageOffset++;
            }
            else // if (c == DATALOG_ENTRY_EMPTY)
            {
                if (datalog.read.page[DATALOG_PAGE_STATE_OFFSET] != PAGESTATE_WRITABLE)
                    datalog.read.pageOffset++;
                else
                {
                    // End of data
                    datalog.read.active = false;

                    return false;
                }
            }
        }
    }
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
