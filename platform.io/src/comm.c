/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>

#include <stdio.h>

#include "adc.h"
#include "comm.h"
#include "cstring.h"
#include "display.h"
#include "events.h"
#include "measurements.h"
#include "rng.h"
#include "rtc.h"
#include "system.h"
#include "tube.h"

Comm comm;

void initComm(void)
{
#if !defined(DATA_MODE)
    openComm();
#else
    if (settings.dataMode)
        openComm();
#endif
}

void enableComm(bool value)
{
    comm.enabled = value;
}

static bool matchCommCommand(const char *command)
{
    uint32_t index = 0;

    while (true)
    {
        char c1 = comm.buffer[index];
        char c2 = command[index];
        index++;

        if (c2 == '\0')
            return (c1 <= ' ');

        if (c1 != c2)
            return false;
    }
}

static char *matchCommCommandWithParameter(const char *command)
{
    if (!matchCommCommand(command))
        return NULL;

    if (strlen(comm.buffer) <= (strlen(command) + 1))
        return NULL;

    return comm.buffer + strlen(command) + 1;
}

static bool matchCommCommandWithUint32(const char *command,
                                       uint32_t *value)
{
    char *parameter = matchCommCommandWithParameter(command);
    if (!parameter)
        return false;

    return parseUInt32(parameter, value);
}

static bool matchCommCommandWithFloat(const char *command,
                                      float *value)
{
    char *parameter = matchCommCommandWithParameter(command);
    if (!parameter)
        return false;

    return parseFloat(parameter, value);
}

static void sendCommResponse(bool success)
{
    strcpy(comm.buffer, success ? "OK" : "ERROR");
}

static void sendCommOk(void)
{
    sendCommResponse(true);
}

static void sendCommOkWithString(const char *value)
{
    sendCommOk();
    strcatChar(comm.buffer, ' ');
    strcat(comm.buffer, value);
}

static void sendCommOkWithUInt32(uint32_t value)
{
    sendCommOk();
    strcatChar(comm.buffer, ' ');
    strcatUInt32(comm.buffer, value, 0);
}

static void sendCommOkWithFloat(float value, uint32_t fractionalDecimals)
{
    sendCommOk();
    strcatChar(comm.buffer, ' ');
    strcatFloat(comm.buffer, value, fractionalDecimals);
}

static void sendCommError(void)
{
    sendCommResponse(false);
}

static void strcatDatalogEntry(char *buffer, const Dose *entry)
{
    strcatUInt32(buffer, entry->time, 0);
    strcatChar(comm.buffer, ',');
    strcatUInt32(buffer, entry->pulseCount, 0);
}

static void startDatalogDump(void)
{
    sendCommOk();

    strcat(comm.buffer, " time,tubePulseCount");

    startDatalogDownload();
    comm.sendingDatalog = true;

    transmitComm();
}

void dispatchCommEvents(void)
{
    updateCommController();

    if (comm.state == COMM_RX_READY)
    {
        uint32_t uint32Value;
        float floatValue;

        if (matchCommCommand("GET deviceId"))
        {
            sendCommOkWithString(commId);
            strcatChar(comm.buffer, ';');
            strcatUInt32Hex(comm.buffer, getDeviceId());
        }
        else if (matchCommCommand("GET deviceBatteryVoltage"))
            sendCommOkWithFloat(getBatteryVoltage(), 3);
        else if (matchCommCommand("GET deviceTime"))
            sendCommOkWithUInt32(getDeviceTime());
        else if (matchCommCommandWithUint32("SET deviceTime",
                                            &uint32Value))
        {
            setDeviceTime(uint32Value);

            sendCommOk();
        }
        else if (matchCommCommand("GET deviceTimeZone"))
            sendCommOkWithFloat(getDeviceTimeZone(), 1);
        else if (matchCommCommandWithFloat("SET deviceTimeZone",
                                           &floatValue))
        {
            bool success = setDeviceTimeZone(floatValue);

            sendCommResponse(success);
        }
        else if (matchCommCommand("GET tubeTime"))
            sendCommOkWithUInt32(getTubeTime());
        else if (matchCommCommandWithUint32("SET tubeTime",
                                            &uint32Value))
        {
            setTubeTime(uint32Value);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubePulseCount"))
            sendCommOkWithUInt32(getTubePulseCount());
        else if (matchCommCommandWithUint32("SET tubePulseCount",
                                            &uint32Value))
        {
            setTubePulseCount(uint32Value);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubeRate"))
            sendCommOkWithFloat(60.0F * getInstantaneousRate(), 3);
        else if (matchCommCommand("GET tubeDeadTime"))
            sendCommOkWithFloat(getTubeDeadTime(), 7);
        else if (matchCommCommand("GET tubeSensitivity"))
            sendCommOkWithFloat(getTubeSensitivity(), 3);
        else if (matchCommCommand("GET tubeDeadTimeCompensation"))
            sendCommOkWithFloat(getTubeDeadTimeCompensation(), 7);
#if defined(TUBE_HV_PWM)
        else if (matchCommCommand("GET tubeHVFrequency"))
            sendCommOkWithFloat(getTubeHVFrequency(), 2);
        else if (matchCommCommandWithFloat("SET tubeHVFrequency",
                                           &floatValue))
        {
            bool success = setTubeHVFrequency(floatValue);

            sendCommResponse(success);
        }
        else if (matchCommCommand("GET tubeHVDutyCycle"))
            sendCommOkWithFloat(getTubeHVDutyCycle(), 5);
        else if (matchCommCommandWithFloat("SET tubeHVDutyCycle",
                                           &floatValue))
        {
            bool success = setTubeHVDutyCycle(floatValue);

            sendCommResponse(success);
        }
#endif
        else if (matchCommCommandWithUint32("GET datalog",
                                            &uint32Value))
        {
            comm.datalogTimeLimit = uint32Value;

            startDatalogDump();

            return;
        }
        else if (matchCommCommand("GET datalog"))
        {
            comm.datalogTimeLimit = 0;

            startDatalogDump();

            return;
        }
        else if (matchCommCommand("RESET datalog"))
        {
            writeDatalogReset();

            sendCommOk();
        }
        else if (matchCommCommand("GET randomData"))
        {
            sendCommOk();

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
        else
            sendCommError();

        strcat(comm.buffer, "\r\n");

        transmitComm();
    }
    else if (comm.state == COMM_TX_READY)
    {
        if (comm.sendingDatalog)
        {
            strclr(comm.buffer);

            uint32_t sentEntriesNum = 0;
            uint32_t checkedEntriesNum = 0;

            while ((sentEntriesNum < 2) &&
                   (checkedEntriesNum < 1000))
            {
                Dose dose;

                if (!getDatalogDownloadEntry(&dose))
                {
                    strcat(comm.buffer, "\r\n");

                    comm.sendingDatalog = false;

                    break;
                }

                if (dose.time >= comm.datalogTimeLimit)
                {
                    strcatChar(comm.buffer, ';');
                    strcatDatalogEntry(comm.buffer, &dose);

                    sentEntriesNum++;
                }

                checkedEntriesNum++;
            }

            transmitComm();
        }
        else
        {
            comm.state = COMM_RX;
        }
    }
}
