/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#include "comm.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "power.h"
#include "rng.h"
#include "rtc.h"
#include "settings.h"
#include "system.h"
#include "tube.h"

#define START_BOOTLOADER_TIME 200

Comm comm;

void initComm()
{
    initCommHardware();
}

void resetComm(bool open)
{
    comm.state = COMM_RX;

    strclr(comm.buffer);

    comm.bufferIndex = 0;
    comm.open = open;
}

static bool parseTokenAndUInt32(const char **s,
                                const char *match,
                                uint32_t *value)
{
    if (!parseToken(s, match))
        return false;

    return parseUInt32(s, value);
}

static bool parseTokenAndFloat(const char **s,
                               const char *match,
                               float *value)
{
    if (!parseToken(s, match))
        return false;

    return parseFloat(s, value);
}

static void pushCommOk(void)
{
    strcpy(comm.buffer, "OK");
    comm.transmitState = TRANSMIT_RESPONSE;
}

static void pushCommString(const char *value)
{
    pushCommOk();
    strcatChar(comm.buffer, ' ');
    strcat(comm.buffer, value);
}

static void pushCommUInt32(uint32_t value)
{
    pushCommOk();
    strcatChar(comm.buffer, ' ');
    strcatUInt32(comm.buffer, value, 0);
}

static void pushCommFloat(float value, uint32_t fractionalDecimals)
{
    pushCommOk();
    strcatChar(comm.buffer, ' ');
    strcatFloat(comm.buffer, value, fractionalDecimals);
}

static void pushCommDatalogEntry(char *buffer, const Dose *entry)
{
    strcatUInt32(buffer, entry->time, 0);
    strcatChar(comm.buffer, ',');
    strcatUInt32(buffer, entry->pulseCount, 0);
}

void dispatchCommEvents(void)
{
    // Update comm state
#if defined(DATA_MODE)
    bool commShouldBeOpen = !isDeviceOff() && settings.dataMode;
#elif defined(PWR_USB)
    bool commShouldBeOpen = !isDeviceOff() && isUSBPowered();
#else
    bool commShouldBeOpen = !isDeviceOff();
#endif
    if (!comm.open && commShouldBeOpen)
        openComm();
    else if (comm.open && !commShouldBeOpen)
        closeComm();

    // Polled comms
    updateComm();

    // Process comm events
    switch (comm.state)
    {
    case COMM_RX_READY:
    {
        const char *s = comm.buffer;
        comm.transmitState = TRANSMIT_ERROR;

        if (parseToken(&s, "GET"))
        {
            if (parseToken(&s, "deviceId"))
            {
                pushCommString(commId);
                strcatChar(comm.buffer, ';');
                comm.transmitState = TRANSMIT_DEVICEID;
            }
            else if (parseToken(&s, "deviceBatteryVoltage"))
                pushCommFloat(getBatteryVoltage(), 3);
            else if (parseToken(&s, "deviceTime"))
                pushCommUInt32(getDeviceTime());
            else if (parseToken(&s, "deviceTimeZone"))
                pushCommFloat(getDeviceTimeZone(), 1);
            else if (parseToken(&s, "tubeTime"))
                pushCommUInt32(getTubeTime());
            else if (parseToken(&s, "tubePulseCount"))
                pushCommUInt32(getTubePulseCount());
            else if (parseToken(&s, "tubeRate"))
                pushCommFloat(60.0F * getInstantaneousRate(), 3);
            else if (parseToken(&s, "tubeDeadTime"))
                pushCommFloat(getTubeDeadTime(), 7);
            else if (parseToken(&s, "tubeSensitivity"))
                pushCommFloat(getTubeSensitivity(), 3);
            else if (parseToken(&s, "tubeDeadTimeCompensation"))
                pushCommFloat(getTubeDeadTimeCompensation(), 7);
#if defined(TUBE_HV_PWM)
            else if (parseToken(&s, "tubeHVFrequency"))
                pushCommFloat(getTubeHVFrequency(), 2);
            else if (parseToken(&s, "tubeHVDutyCycle"))
                pushCommFloat(getTubeHVDutyCycle(), 5);
#endif
            else if (parseToken(&s, "datalog"))
            {
                if (openDatalogRead())
                {
                    comm.datalogStartTimestamp = 0;
                    comm.datalogEndTimestamp = UINT32_MAX;
                    comm.datalogMaxEntryNum = UINT32_MAX;
                    comm.datalogEntryNum = 0;
                    if (parseUInt32(&s, &comm.datalogStartTimestamp))
                    {
                        if (parseUInt32(&s, &comm.datalogEndTimestamp))
                            parseUInt32(&s, &comm.datalogMaxEntryNum);
                    }
                    pushCommOk();
                    strcat(comm.buffer, " time,tubePulseCount");
                    comm.transmitState = TRANSMIT_DATALOG;
                }
            }
            else if (parseToken(&s, "randomData"))
            {
                pushCommOk();
                for (uint32_t i = 0; i < 16; i++)
                {
                    int32_t randomData = getRandomData();
                    if (randomData < 0)
                        break;
                    if (i == 0)
                        strcatChar(comm.buffer, ' ');
                    strcatUInt8Hex(comm.buffer, randomData);
                }
            }
        }
        else if (parseToken(&s, "SET"))
        {
            uint32_t intValue;
            float floatValue;

            if (parseTokenAndUInt32(&s, "deviceTime", &intValue))
            {
                setDeviceTime(intValue);
                pushCommOk();
            }
            else if (parseTokenAndFloat(&s, "deviceTimeZone", &floatValue))
            {
                if (setDeviceTimeZone(floatValue))
                    pushCommOk();
            }
            else if (parseTokenAndUInt32(&s, "tubeTime", &intValue))
            {
                setTubeTime(intValue);
                pushCommOk();
            }
            else if (parseTokenAndUInt32(&s, "tubePulseCount", &intValue))
            {
                setTubePulseCount(intValue);
                pushCommOk();
            }
#if defined(TUBE_HV_PWM)
            else if (parseTokenAndFloat(&s, "tubeHVFrequency", &floatValue))
            {
                if (setTubeHVFrequency(floatValue))
                    pushCommOk();
            }
            else if (parseTokenAndFloat(&s, "tubeHVDutyCycle", &floatValue))
            {
                if (setTubeHVDutyCycle(floatValue))
                    pushCommOk();
            }
#endif
        }
        else if (parseToken(&s, "RESET datalog"))
        {
            writeDatalogReset();
            pushCommOk();
        }
#if defined(BOOTLOADER)
        else if (parseToken(&s, "START bootloader"))
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
        else if (parseToken(&s, "<BOOTLOADER1>>"))
        {
            strcpy(comm.buffer, "BOOTLOADER");
            comm.transmitState = TRANSMIT_BOOTLOADER;
        }
#endif

        if (comm.transmitState <= TRANSMIT_BOOTLOADER)
            strcat(comm.buffer, "\r\n");
        else if (comm.transmitState == TRANSMIT_RAW)
            comm.transmitState = TRANSMIT_RESPONSE;
        else if (comm.transmitState == TRANSMIT_ERROR)
        {
            strcpy(comm.buffer, "ERROR\r\n");
            comm.transmitState = TRANSMIT_RESPONSE;
        }

        transmitComm();

        break;
    }

    case COMM_TX_READY:
        switch (comm.transmitState)
        {
#if defined(BOOTLOADER)
        case TRANSMIT_BOOTLOADER:
        {
            sleep(START_BOOTLOADER_TIME);
            powerOff(false);
            setBacklight(false);
            enableDisplay(false);
            startBootloader();

            comm.transmitState = TRANSMIT_RESPONSE;

            break;
        }
#endif

        case TRANSMIT_DEVICEID:
        {
            char deviceId[32];
            getDeviceId(deviceId);
            strcat(comm.buffer, deviceId);
            strcat(comm.buffer, "\r\n");
            comm.transmitState = TRANSMIT_RESPONSE;

            transmitComm();

            break;
        }

        case TRANSMIT_DATALOG:
        {
            uint32_t sentEntryNum = 0;
            uint32_t checkedEntryNum = 0;

            while ((sentEntryNum < 2) &&
                   (checkedEntryNum < 1000))
            {
                Dose dose;

                if (!readDatalog(&dose))
                {
                    closeDatalogRead();

                    strcat(comm.buffer, "\r\n");
                    comm.transmitState = TRANSMIT_RESPONSE;

                    break;
                }

                if ((dose.time >= comm.datalogStartTimestamp) &&
                    (dose.time <= comm.datalogEndTimestamp) &&
                    (comm.datalogEntryNum < comm.datalogMaxEntryNum))
                {
                    strcatChar(comm.buffer, ';');
                    pushCommDatalogEntry(comm.buffer, &dose);

                    sentEntryNum++;
                    comm.datalogEntryNum++;
                }

                checkedEntryNum++;
            }

            transmitComm();

            break;
        }

        default:
        {
            comm.state = COMM_RX;

            break;
        }

        break;
        }

    default:
        break;
    }
}
