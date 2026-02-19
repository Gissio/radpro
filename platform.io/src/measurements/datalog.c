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
#include "../system/cmath.h"
#include "../system/settings.h"
#include "../ui/menu.h"
#include "../ui/system.h"
#include "../ui/view.h"

#define DATALOG_PAGE_SIZE FLASH_PAGE_SIZE
#define DATALOG_PAGE_STATE_OFFSET (DATALOG_PAGE_SIZE - DATALOG_PAGE_STATE_SIZE)
#define DATALOG_PAGE_STATE_SIZE FLASH_WORD_SIZE

#define DATALOG_BUFFER_SIZE 16

#define DATALOG_ENTRY_INCREMENTAL_2BYTES 0x80
#define DATALOG_ENTRY_INCREMENTAL_3BYTES 0xc0
#define DATALOG_ENTRY_INCREMENTAL_4BYTES 0xe0
#define DATALOG_ENTRY_INCREMENTAL_5BYTES 0xf0
#define DATALOG_ENTRY_ABSOLUTE 0xf1
#define DATALOG_ENTRY_SESSION_START 0xf8
#define DATALOG_ENTRY_FILLER 0xfe
#define DATALOG_ENTRY_EMPTY 0xff

typedef enum
{
    PAGESTATE_FULL = 0x00,
    PAGESTATE_RESET = 0x01,
    PAGESTATE_WRITABLE = 0xff,
} PageState;

static const Menu datalogMenu;
static const Menu datalogModeMenu;

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
    uint32_t pageOffset;
    const uint8_t *page;

    uint32_t timeInterval;
} DatalogRead;

static struct
{
    uint32_t previousTimeFast;

    DatalogWrite write;
    DatalogRead read;
} datalog;

static const uint16_t loggingModeIntervals[] = {
    0,
    60 * 60,
    10 * 60,
    1 * 60,
    10,
    1,
};

static void stopDatalogRead(void);

void setupDatalog(void)
{
    selectMenuItem(&datalogMenu, 0);
    selectMenuItem(&datalogModeMenu, settings.loggingMode);
}

// Flash access

static bool isFlashEmpty(uint32_t address, uint32_t count)
{
    const uint8_t *p = readFlash(address, count);

    for (uint32_t i = 0; i < count; i++)
    {
        if (p[i] != 0xff)
            return false;
    }

    return true;
}

static uint32_t alignToFlashWordSize(uint32_t value)
{
    return (value + FLASH_WORD_SIZE - 1) & ~(FLASH_WORD_SIZE - 1);
}

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

static PageState readPageState(uint32_t pageBase)
{
    const uint8_t *pageStateData = readFlash(pageBase + DATALOG_PAGE_STATE_OFFSET, DATALOG_PAGE_STATE_SIZE);

    // Check page state trailing bytes
    for (uint8_t i = 1; i < DATALOG_PAGE_STATE_SIZE; i++)
    {
        if (pageStateData[i] != 0xff)
            return PAGESTATE_RESET;
    }

    // Check page state first byte
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

// Value encoders/decoders

static uint32_t encodeFixedUInt32(uint8_t *p, uint32_t value)
{
    p[0] = (value >> 24) & 0xff;
    p[1] = (value >> 16) & 0xff;
    p[2] = (value >> 8) & 0xff;
    p[3] = (value >> 0) & 0xff;

    return 4;
}

static uint32_t decodeFixedUInt32(const uint8_t *p, uint32_t *value)
{
    *value = (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | (p[3] << 0);

    return 4;
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
        p[0] = DATALOG_ENTRY_INCREMENTAL_2BYTES | (value >> 8);
        p[1] = (value >> 0) & 0xff;

        return 2;
    }
    else if (value < 0x200000)
    {
        // 21-bit value
        p[0] = DATALOG_ENTRY_INCREMENTAL_3BYTES | (value >> 16);
        p[1] = (value >> 8) & 0xff;
        p[2] = (value >> 0) & 0xff;

        return 3;
    }
    else if (value < 0x10000000)
    {
        // 28-bit value
        p[0] = DATALOG_ENTRY_INCREMENTAL_4BYTES | (value >> 24);
        p[1] = (value >> 16) & 0xff;
        p[2] = (value >> 8) & 0xff;
        p[3] = (value >> 0) & 0xff;

        return 4;
    }
    else
    {
        // 32-bit value
        p[0] = DATALOG_ENTRY_INCREMENTAL_5BYTES;
        encodeFixedUInt32(p + 1, value);

        return 5;
    }
}

static uint32_t decodeVariableUInt32(const uint8_t *p, uint32_t *value)
{
    uint8_t c = p[0];

    if (c < DATALOG_ENTRY_INCREMENTAL_2BYTES)
    {
        // 7-bit value
        *value = c;

        return 1;
    }
    else if (c < DATALOG_ENTRY_INCREMENTAL_3BYTES)
    {
        // 14-bit value
        *value = ((c & 0x3f) << 8) | p[1];

        return 2;
    }
    else if (c < DATALOG_ENTRY_INCREMENTAL_4BYTES)
    {
        // 21-bit value
        *value = ((c & 0x1f) << 16) | (p[1] << 8) | p[2];

        return 3;
    }
    else if (c < DATALOG_ENTRY_INCREMENTAL_5BYTES)
    {
        // 28-bit value
        *value = ((c & 0x0f) << 24) | (p[1] << 16) | (p[2] << 8) | p[3];

        return 4;
    }
    else if (c == DATALOG_ENTRY_INCREMENTAL_5BYTES)
    {
        // 32-bit value
        return 1 + decodeFixedUInt32(p + 1, value);
    }

    return 0;
}

// Write functions

static void writePageStateAndAdvance(PageState pageState)
{
    // Mark current page
    uint8_t pageStateData[DATALOG_PAGE_STATE_SIZE];
    pageStateData[0] = pageState;
    memset(pageStateData + 1, 0xff, DATALOG_PAGE_STATE_SIZE - 1);

    if (!writeFlash(datalog.write.pageBase + DATALOG_PAGE_STATE_OFFSET, pageStateData, DATALOG_PAGE_STATE_SIZE))
    {
        eraseFlash(datalog.write.pageBase);
        writeFlash(datalog.write.pageBase + DATALOG_PAGE_STATE_OFFSET, pageStateData, DATALOG_PAGE_STATE_SIZE);

        datalog.write.pageOffset = 0;

        return;
    }

    // Advance to next page, erase if necessary
    datalog.write.pageBase = getNextPage(datalog.write.pageBase);
    datalog.write.pageOffset = 0;

    if (!isFlashEmpty(datalog.write.pageBase, DATALOG_PAGE_SIZE))
        eraseFlash(datalog.write.pageBase);
}

static void flushDatalogBuffer(void)
{
    uint32_t alignedLength = alignToFlashWordSize(datalog.write.bufferLength);
    memset(datalog.write.buffer + datalog.write.bufferLength, DATALOG_ENTRY_FILLER, alignedLength - datalog.write.bufferLength);

    writeFlash(datalog.write.pageBase + datalog.write.pageOffset, datalog.write.buffer, alignedLength);

    datalog.write.pageOffset += alignedLength;
    datalog.write.bufferLength = 0;
}

static bool appendDatalogEntryToCurrentPage(const uint8_t *entry, uint32_t count)
{
    // Fail if entry does not fit within page
    if ((datalog.write.pageOffset + datalog.write.bufferLength + count) > DATALOG_PAGE_STATE_OFFSET)
        return false;

    // Append
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

    uint8_t entry[9];
    uint8_t *p = entry;

    *p++ = DATALOG_ENTRY_ABSOLUTE + (settings.loggingMode - 1);
    p += encodeFixedUInt32(p, datalog.write.dose.time);
    p += encodeFixedUInt32(p, datalog.write.dose.pulseCount);

    appendDatalogEntryWithPageRollover(entry, sizeof(entry));

    stopDatalogRead();
}

static bool appendDatalogIncrementalEntry(void)
{
    uint32_t pulseCount = getTubePulseCount();
    uint32_t value = pulseCount - datalog.write.dose.pulseCount;

    uint8_t entry[5];
    uint32_t count = encodeVariableUInt32(entry, value);

    if (!appendDatalogEntryToCurrentPage(entry, count))
        return false;

    datalog.write.dose.pulseCount = pulseCount;
    datalog.write.dose.time += loggingModeIntervals[settings.loggingMode];

    return true;
}

void startDatalog(void)
{
    datalog.write.active = true;

    writeDatalogSessionStart();
    appendDatalogAbsoluteEntry();
}

void stopDatalog(void)
{
    appendDatalogAbsoluteEntry();

    datalog.write.active = false;
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

    stopDatalogRead();
}

void updateDatalog(void)
{
    uint32_t timeFast = getDeviceTimeFast();
    if (timeFast != datalog.previousTimeFast)
    {
        datalog.previousTimeFast = timeFast;

        if (datalog.write.active && !datalog.read.active)
        {
            uint32_t time = getDeviceTime();
            uint32_t elapsedTime = time - datalog.write.dose.time;
            uint32_t loggingInterval = loggingModeIntervals[settings.loggingMode];

            if (elapsedTime == loggingInterval)
            {
                if (!appendDatalogIncrementalEntry())
                    appendDatalogAbsoluteEntry();
            }
            else if (elapsedTime > loggingInterval)
                appendDatalogAbsoluteEntry();
        }
    }
}

// Read functions

static void readPage(void)
{
    datalog.read.page = readFlash(datalog.read.pageBase, DATALOG_PAGE_SIZE);
    datalog.read.pageOffset = 0;
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

static void stopDatalogRead(void)
{
    static const uint8_t datalogEndOfRead = DATALOG_ENTRY_EMPTY;

    datalog.read.active = false;
    datalog.read.page = &datalogEndOfRead;
    datalog.read.pageOffset = 0;
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
        uint32_t count = decodeVariableUInt32(datalog.read.page + datalog.read.pageOffset, &value);
        if (count)
        {
            datalog.read.pageOffset += count;

            record->dose.pulseCount += value;
            record->dose.time += datalog.read.timeInterval;

            return true;
        }
        else
        {
            uint8_t c = datalog.read.page[datalog.read.pageOffset];

            if (c < (DATALOG_ENTRY_ABSOLUTE + (DATALOG_LOGGINGMODE_NUM - 1)))
            {
                // Time interval, absolute timestamp and pulse count value
                uint32_t mode = (c - DATALOG_ENTRY_ABSOLUTE) + 1;
                datalog.read.timeInterval = loggingModeIntervals[mode];
                datalog.read.pageOffset++;
                datalog.read.pageOffset += decodeFixedUInt32(datalog.read.page + datalog.read.pageOffset, &record->dose.time);
                datalog.read.pageOffset += decodeFixedUInt32(datalog.read.page + datalog.read.pageOffset, &record->dose.pulseCount);

                return true;
            }
            else if (c == DATALOG_ENTRY_SESSION_START)
            {
                // Session start
                datalog.read.pageOffset++;
                record->sessionStart = true;
            }
            else if (c < DATALOG_ENTRY_EMPTY)
            {
                // Fill bytes
                datalog.read.pageOffset++;
            }
            else // if (c == DATALOG_ENTRY_EMPTY)
            {
                if (datalog.read.active &&
                    (datalog.read.page[DATALOG_PAGE_STATE_OFFSET] != PAGESTATE_WRITABLE))
                    datalog.read.pageOffset++;
                else
                    break;
            }
        }
    }

    // End of data
    datalog.read.active = false;

    return false;
}

// Common functions

void initDatalog(void)
{
    // Get most recent datalog page
    if (!setDatalogTail())
    {
        eraseFlash(DATALOG_BASE);

        datalog.read.pageBase = DATALOG_BASE;
    }

    // Get most recent record
    readPage();
    uint32_t startPageBase = datalog.read.pageBase;

    DatalogRecord record;
    while (readDatalog(&record))
    {
        // Recover from page rollover
        if (datalog.read.pageBase != startPageBase)
        {
            datalog.read.pageOffset = 0;

            break;
        }
    }

    // Validate rest of page empty
    if (!isFlashEmpty(datalog.read.pageBase + datalog.read.pageOffset, DATALOG_PAGE_SIZE - datalog.read.pageOffset))
    {
        eraseFlash(datalog.read.pageBase);

        datalog.read.pageOffset = 0;
    }

    // Set write head
    datalog.write.pageBase = datalog.read.pageBase;
    datalog.write.pageOffset = alignToFlashWordSize(datalog.read.pageOffset);
}

// Logging mode menu

static cstring loggingModeMenuOptions[] = {
    STRING_OFF,
    STRING_EVERY_HOUR,
    STRING_EVERY10_MINUTES,
    STRING_EVERY_MINUTE,
    STRING_EVERY10_SECONDS,
    STRING_EVERY_SECOND,
};

static const char *onDatalogModeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.loggingMode);

    return getString(loggingModeMenuOptions[index]);
}

static void onDatalogModeMenuSelect(menu_size_t index)
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

static const Menu datalogModeMenu = {
    STRING_LOGGING_MODE,
    &datalogModeMenuState,
    ARRAY_SIZE(loggingModeMenuOptions),
    onDatalogModeMenuGetOption,
    onDatalogModeMenuSelect,
    showDatalogMenu,
};

void showDatalogModeMenuView(void)
{
    showMenu(&datalogModeMenu);
}

// Data log reset confirmation view

static void onDatalogResetConfirmationEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
    case EVENT_KEY_SELECT:
        showDatalogMenu();

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_RESET), getString(STRING_NOTIFICATION_DATALOG_RESET_SUCCESS));

        break;

    default:
        break;
    }
}

// Data log reset alert view

static void onDatalogResetAlertEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        showDatalogMenu();

        break;

    case EVENT_KEY_SELECT:
        resetDatalog();

        showView(onDatalogResetConfirmationEvent);

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_RESET), getString(STRING_NOTIFICATION_DATALOG_RESET_CONFIRM));

        break;

    default:
        break;
    }
}

void showDatalogResetAlertView(void)
{
    showView(onDatalogResetAlertEvent);
}

// Data log menu

static const ViewOption datalogMenuOptions[] = {
    {STRING_LOGGING_MODE, showDatalogModeMenuView},
    {STRING_RESET, showDatalogResetAlertView},
};

static const char *onDatalogMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == 0) ? MENUSTYLE_SUBMENU : 0;

    return getString(datalogMenuOptions[index].title);
}

static void onDatalogMenuSelect(menu_size_t index)
{
    datalogMenuOptions[index].setView();
}

static MenuState datalogMenuState;

static const Menu datalogMenu = {
    STRING_DATALOG,
    &datalogMenuState,
    ARRAY_SIZE((datalogMenuOptions)),
    onDatalogMenuGetOption,
    onDatalogMenuSelect,
    showSettingsMenu,
};

void showDatalogMenu(void)
{
    showMenu(&datalogMenu);
}
