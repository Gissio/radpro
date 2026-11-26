/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../extras/rng.h"
#include "../measurements/datalog.h"
#include "../measurements/electricfield.h"
#include "../measurements/magneticfield.h"
#include "../measurements/instantaneous.h"
#include "../measurements/measurements.h"
#include "../peripherals/display.h"
#include "../peripherals/comm.h"
#include "../peripherals/rtc.h"
#include "../peripherals/tube.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../system/system.h"

#define START_BOOTLOADER_TIME_MS 200

#define DATALOG_MAX_RECORDS_PER_TX 2
#define DATALOG_MAX_SCAN_PER_TX 1000

#define RANDOM_DATA_MAX_BYTES 16

Comm comm;

void initComm(void)
{
    initCommHardware();
}

NOINLINE void clearComm(bool open)
{
    // Abort interrupted datalog transfer, so logging resumes
    if (comm.transmitState == TRANSMIT_DATALOG)
    {
        stopDatalogRead();

        comm.transmitState = TRANSMIT_RESPONSE;
    }

    comm.state = COMM_RX;

    strclr(comm.buffer);

    comm.bufferIndex = 0;
    comm.previousChar = '\0';
    comm.open = open;
}

// Receive

void invalidateCommCommand(void)
{
    // A full buffer starting with '\0' is an invalid command: further
    // characters are discarded until the end of the command, which then
    // yields an empty request (answered with ERROR)
    comm.buffer[0] = '\0';
    comm.bufferIndex = COMM_BUFFER_SIZE - 1;
}

bool receiveCommChar(char c)
{
    bool endOfCommand;

    if (c >= ' ')
    {
        if (comm.bufferIndex < (COMM_BUFFER_SIZE - 1))
            comm.buffer[comm.bufferIndex++] = c;
        else
            invalidateCommCommand();

#if defined(GMC800)
        // GQ GMC commands end with ">>" instead of a line ending
        endOfCommand = (c == '>') && (comm.previousChar == '>');
#else
        endOfCommand = false;
#endif
    }
    else
        endOfCommand = (c == '\r') ||
                       ((c == '\n') && (comm.previousChar != '\r'));

    comm.previousChar = c;

    if (endOfCommand)
    {
        comm.buffer[comm.bufferIndex] = '\0';

        comm.bufferIndex = 0;
        comm.state = COMM_RX_READY;
    }

    return endOfCommand;
}

// Transmit

void endCommTransmit(void)
{
    strclr(comm.buffer);
    comm.bufferIndex = 0;

    comm.state = (comm.transmitState == TRANSMIT_RESPONSE)
                     ? COMM_RX
                     : COMM_TX_READY;
}

// Responses

static void pushCommOk(void)
{
    strcpy(comm.buffer, "OK");
    comm.transmitState = TRANSMIT_RESPONSE;
}

static void pushCommOkSpace(void)
{
    pushCommOk();
    strcatChar(comm.buffer, ' ');
}

static void endCommResponse(void)
{
    strcat(comm.buffer, "\r\n");
    comm.transmitState = TRANSMIT_RESPONSE;
}

// Parsing

static bool isCommEnd(const char *s)
{
    while (*s == ' ')
        s++;

    return (*s == '\0');
}

// Properties

typedef enum
{
    PROPERTY_DEVICE_ID,
    PROPERTY_DEVICE_POWER,
    PROPERTY_DEVICE_TIME,
    PROPERTY_DEVICE_TIME_ZONE,
    PROPERTY_TUBE_TIME,
    PROPERTY_TUBE_PULSE_COUNT,
#if defined(TUBE_HV_PWM)
    PROPERTY_TUBE_HV_FREQUENCY,
    PROPERTY_TUBE_HV_DUTY_CYCLE,
#endif
    PROPERTY_DEVICE_BATTERY_VOLTAGE,
    PROPERTY_TUBE_TYPE,
    PROPERTY_TUBE_RATE,
    PROPERTY_TUBE_DEAD_TIME,
    PROPERTY_TUBE_SENSITIVITY,
    PROPERTY_TUBE_DEAD_TIME_COMPENSATION,
#if defined(EMFMETER)
    PROPERTY_ELECTRIC_FIELD,
    PROPERTY_MAGNETIC_FIELD,
#endif
    PROPERTY_DATALOG,
    PROPERTY_RANDOM_DATA,

    PROPERTY_NUM,
} Property;

// Must match the order of Property
static const char *const commPropertyNames[] = {
    "deviceId",
    "devicePower",
    "deviceTime",
    "deviceTimeZone",
    "tubeTime",
    "tubePulseCount",
#if defined(TUBE_HV_PWM)
    "tubeHVFrequency",
    "tubeHVDutyCycle",
#endif
    "deviceBatteryVoltage",
    "tubeType",
    "tubeRate",
    "tubeDeadTime",
    "tubeSensitivity",
    "tubeDeadTimeCompensation",
#if defined(EMFMETER)
    "electricField",
    "magneticField",
#endif
    "datalog",
    "randomData",
};

_Static_assert(ARRAY_SIZE(commPropertyNames) == PROPERTY_NUM,
               "commPropertyNames must match Property");

static Property parseCommProperty(const char **s)
{
    Property property = 0;

    while ((property < PROPERTY_NUM) &&
           !parseToken(s, commPropertyNames[property]))
        property++;

    return property;
}

static void getCommDatalog(const char *s)
{
    // All arguments are optional
    comm.datalogStartTime = 0;
    comm.datalogEndTime = UINT32_MAX;
    comm.datalogMaxRecordNum = UINT32_MAX;
    comm.datalogRecordNum = 0;

    parseUInt32(&s, &comm.datalogStartTime);
    parseUInt32(&s, &comm.datalogEndTime);
    parseUInt32(&s, &comm.datalogMaxRecordNum);

    if (isCommEnd(s) && startDatalogRead())
    {
        pushCommOk();
        strcat(comm.buffer, " time,tubePulseCount");
        comm.transmitState = TRANSMIT_DATALOG;
    }
}

static void getCommRandomData(void)
{
    pushCommOk();

    for (uint32_t i = 0; i < RANDOM_DATA_MAX_BYTES; i++)
    {
        int32_t value = getRNGByte();
        if (value < 0)
            break;

        if (i == 0)
            strcatChar(comm.buffer, ' ');
        strcatUInt8Hex(comm.buffer, value);
    }
}

static void pushCommUInt32(uint32_t value)
{
    pushCommOkSpace();
    strcatUInt32(comm.buffer, value, 0);
}

static void pushCommFloat(float value, uint32_t fractionalDecimals)
{
    pushCommOkSpace();
    strcatFloat(comm.buffer, value, fractionalDecimals);
}

static void processCommGet(const char *s)
{
    Property property = parseCommProperty(&s);

    // Only datalog takes arguments
    if (property == PROPERTY_DATALOG)
    {
        getCommDatalog(s);

        return;
    }

    if (!isCommEnd(s))
        return;

    switch (property)
    {
    case PROPERTY_DEVICE_ID:
        // The device id is appended in a second transmission
        pushCommOkSpace();
        strcat(comm.buffer, commId);
        strcatChar(comm.buffer, ';');
        comm.transmitState = TRANSMIT_DEVICEID;

        break;

    case PROPERTY_DEVICE_POWER:
        pushCommUInt32(isPoweredOn());

        break;

    case PROPERTY_DEVICE_TIME:
        pushCommUInt32(getDeviceTime());

        break;

    case PROPERTY_DEVICE_TIME_ZONE:
        pushCommFloat(getDeviceTimeZone(), 1);

        break;

    case PROPERTY_TUBE_TIME:
        pushCommUInt32(getTubeTime());

        break;

    case PROPERTY_TUBE_PULSE_COUNT:
        pushCommUInt32(getTubePulseCount());

        break;

#if defined(TUBE_HV_PWM)
    case PROPERTY_TUBE_HV_FREQUENCY:
        pushCommFloat(getTubeHVFrequency(), 2);

        break;

    case PROPERTY_TUBE_HV_DUTY_CYCLE:
        pushCommFloat(getTubeHVDutyCycle(), 5);

        break;
#endif

    case PROPERTY_DEVICE_BATTERY_VOLTAGE:
        pushCommFloat(getBatteryVoltage(), 3);

        break;

    case PROPERTY_TUBE_TYPE:
        pushCommOkSpace();
        strcat(comm.buffer, getTubeType());

        break;

    case PROPERTY_TUBE_RATE:
        pushCommFloat(60.0F * getInstantaneousRate(), 3);

        break;

    case PROPERTY_TUBE_DEAD_TIME:
        pushCommFloat(getTubeDeadTime(), 7);

        break;

    case PROPERTY_TUBE_SENSITIVITY:
        pushCommFloat(getTubeSensitivity(), 3);

        break;

    case PROPERTY_TUBE_DEAD_TIME_COMPENSATION:
        pushCommFloat(getTubeDeadTimeCompensation(), 7);

        break;

#if defined(EMFMETER)
    case PROPERTY_ELECTRIC_FIELD:
        pushCommFloat(getElectricField(), 3);

        break;

    case PROPERTY_MAGNETIC_FIELD:
        pushCommFloat(getMagneticField(), 9);

        break;
#endif

    case PROPERTY_RANDOM_DATA:
        getCommRandomData();

        break;

    default:
        break;
    }
}

static void processCommSet(const char *s)
{
    Property property = parseCommProperty(&s);

    // Settable properties take a uint32 value, except these float ones
    bool isFloat = (property == PROPERTY_DEVICE_TIME_ZONE);
#if defined(TUBE_HV_PWM)
    isFloat |= (property == PROPERTY_TUBE_HV_FREQUENCY) ||
               (property == PROPERTY_TUBE_HV_DUTY_CYCLE);
#endif

    uint32_t intValue;
    float floatValue;

    if (!(isFloat
              ? parseFloat(&s, &floatValue)
              : parseUInt32(&s, &intValue)) ||
        !isCommEnd(s))
        return;

    switch (property)
    {
    case PROPERTY_DEVICE_POWER:
        if (intValue)
            powerOn();
        else
            powerOff();

        break;

    case PROPERTY_DEVICE_TIME:
        if (!setDeviceTime(intValue))
            return;

        writeDatalogTimeChange();

        break;

    case PROPERTY_DEVICE_TIME_ZONE:
        if (!setDeviceTimeZone(floatValue))
            return;

        break;

    case PROPERTY_TUBE_TIME:
        setTubeTime(intValue);

        break;

    case PROPERTY_TUBE_PULSE_COUNT:
        setTubePulseCount(intValue);

        // Record new pulse count base in datalog
        writeDatalogTimeChange();

        break;

#if defined(TUBE_HV_PWM)
    case PROPERTY_TUBE_HV_FREQUENCY:
        if (!setTubeHVFrequency(floatValue))
            return;

        break;

    case PROPERTY_TUBE_HV_DUTY_CYCLE:
        if (!setTubeHVDutyCycle(floatValue))
            return;

        break;
#endif

    default:
        return;
    }

    pushCommOk();
}

// Commands

static void processCommCommand(void)
{
    const char *s = comm.buffer;
    comm.transmitState = TRANSMIT_ERROR;

    if (parseToken(&s, "GET"))
        processCommGet(s);
    else if (parseToken(&s, "SET"))
        processCommSet(s);
    else if (parseToken(&s, "RESET datalog") && isCommEnd(s))
    {
        clearDatalog();
        pushCommOk();
    }
#if defined(BOOTLOADER)
    else if (parseToken(&s, "START bootloader") && isCommEnd(s))
    {
        pushCommOk();
        comm.transmitState = TRANSMIT_BOOTLOADER;
    }
#endif
#if defined(GMC800)
    else if (parseToken(&s, "<GETVER>>"))
    {
        strcpy(comm.buffer, "GMC-800Re RADP");
        comm.transmitState = TRANSMIT_RAW;
    }
    else if (parseToken(&s, "<GETSERIAL>>"))
    {
        strcpy(comm.buffer, "\xde\xca\xfb\xad\xc0\xff\xee");
        comm.transmitState = TRANSMIT_RAW;
    }
#if defined(BOOTLOADER)
    else if (parseToken(&s, "<BOOTLOADER1>>"))
    {
        strcpy(comm.buffer, "BOOTLOADER");
        comm.transmitState = TRANSMIT_BOOTLOADER;
    }
#endif
#endif

    switch (comm.transmitState)
    {
    case TRANSMIT_RESPONSE:
    case TRANSMIT_BOOTLOADER:
        strcat(comm.buffer, "\r\n");

        break;

    case TRANSMIT_RAW:
        comm.transmitState = TRANSMIT_RESPONSE;

        break;

    case TRANSMIT_ERROR:
        strcpy(comm.buffer, "ERROR");
        endCommResponse();

        break;

    default:
        break;
    }

    transmitComm();
}

// Multi-part responses

static void transmitCommDatalog(void)
{
    if (!isDatalogReadActive())
    {
        // A datalog write (clock change, logging mode change, reset, power
        // on or off) or the history load stopped the read: flag the
        // transfer as incomplete
        strcpy(comm.buffer, ";ERROR");
        endCommResponse();
    }
    else
    {
        uint32_t sentRecordNum = 0;

        for (uint32_t readRecordNum = 0;
             (sentRecordNum < DATALOG_MAX_RECORDS_PER_TX) &&
             (readRecordNum < DATALOG_MAX_SCAN_PER_TX);
             readRecordNum++)
        {
            // Stop reading once enough records were sent, so logging resumes
            if (comm.datalogRecordNum >= comm.datalogMaxRecordNum)
                stopDatalogRead();

            if (!readDatalog(&comm.datalogRecord))
            {
                endCommResponse();

                break;
            }

            const Dose *dose = &comm.datalogRecord.dose;

            if ((dose->time >= comm.datalogStartTime) &&
                (dose->time <= comm.datalogEndTime))
            {
                if (comm.datalogRecord.sessionStart)
                    strcatChar(comm.buffer, ';');
                strcatChar(comm.buffer, ';');
                strcatUInt32(comm.buffer, dose->time, 0);
                strcatChar(comm.buffer, ',');
                strcatUInt32(comm.buffer, dose->pulseCount, 0);

                sentRecordNum++;
                comm.datalogRecordNum++;
            }
        }
    }

    transmitComm();
}

static void processCommTransmitReady(void)
{
    switch (comm.transmitState)
    {
#if defined(BOOTLOADER)
    case TRANSMIT_BOOTLOADER:
        sleep(START_BOOTLOADER_TIME_MS);

        powerOff();

        // Turn off the display immediately; powerOff() alone would leave it on until next event loop.
        setBacklight(false);
        setDisplayEnabled(false);

        startBootloader();

        comm.transmitState = TRANSMIT_RESPONSE;

        break;
#endif

    case TRANSMIT_DEVICEID:
        getDeviceId(comm.buffer);
        endCommResponse();
        transmitComm();

        break;

    case TRANSMIT_DATALOG:
        transmitCommDatalog();

        break;

    default:
        comm.state = COMM_RX;

        break;
    }
}

// Update comm

void updateComm(void)
{
    // Update comm state
#if defined(DATA_MODE)
    bool commShouldBeOpen = settings.dataMode;
#else
    bool commShouldBeOpen = isUSBPowered();
#endif
    if (!comm.open && commShouldBeOpen)
        openComm();
    else if (comm.open && !commShouldBeOpen)
        closeComm();

    // Poll comm hardware
    pollComm();

    // Process comm events
    if (comm.state == COMM_RX_READY)
        processCommCommand();
    else if (comm.state == COMM_TX_READY)
        processCommTransmitReady();
}
