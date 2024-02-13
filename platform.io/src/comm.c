/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>
#include <string.h>

#include <stdio.h>

#include "adc.h"
#include "comm.h"
#include "cstring.h"
#include "events.h"
#include "measurements.h"
#include "rng.h"
#include "rtc.h"
#include "system.h"
#include "tube.h"

Comm comm;

void setCommEnabled(bool value)
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

static bool matchCommCommandWithUInt32(const char *command, uint32_t *value)
{
    if (!matchCommCommand(command))
        return false;

    return (strlen(comm.buffer) > (strlen(command) + 1)) &&
           parseUInt32(comm.buffer + strlen(command) + 1, value);
}

static void sendCommOk(void)
{
    strcpy(comm.buffer, "OK");
}

static void sendCommOkWithString(const char *value)
{
    sendCommOk();
    strcat(comm.buffer, " ");
    strcat(comm.buffer, value);
}

static void sendCommOkWithUInt32(uint32_t value)
{
    sendCommOk();
    strcat(comm.buffer, " ");
    strcatUInt32(comm.buffer, value, 0);
}

static void sendCommOkWithFloat(float value, uint32_t fractionalDecimals)
{
    sendCommOk();
    strcat(comm.buffer, " ");
    strcatFloat(comm.buffer, value, fractionalDecimals);
}

static void sendCommError(void)
{
    strcpy(comm.buffer, "ERROR");
}

static void strcatDatalogEntry(char *buffer, const Dose *entry)
{
    strcatUInt32(buffer, entry->time, 0);
    strcat(buffer, ",");
    strcatUInt32(buffer, entry->pulseCount, 0);
}

static void startDatalogDump(void)
{
    sendCommOk();

    strcat(comm.buffer, " time,tubePulseCount");

    stopDatalog();
    initDatalogRead();
    comm.sendingDatalog = true;

    transmitComm();
}

void dispatchCommEvents(void)
{
    updateCommHardware();

    if (comm.state == COMM_RX_READY)
    {
        uint32_t value;

        if (matchCommCommand("GET deviceId"))
        {
            sendCommOkWithString(commId);
            strcat(comm.buffer, ";");
            strcatUInt32Hex(comm.buffer, getDeviceId());
        }
        else if (matchCommCommand("GET deviceTemperature"))
            sendCommOkWithFloat(getDeviceTemperature(), 1);
        else if (matchCommCommand("GET deviceTime"))
            sendCommOkWithUInt32(getRTCTime());
        else if (matchCommCommandWithUInt32("SET deviceTime", &value))
        {
            setRTCTime(value);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubeTime"))
            sendCommOkWithUInt32(getTubeTime());
        else if (matchCommCommandWithUInt32("SET tubeTime", &value))
        {
            setTubeTime(value);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubePulseCount"))
            sendCommOkWithUInt32(getTubePulseCount());
        else if (matchCommCommandWithUInt32("SET tubePulseCount", &value))
        {
            setTubePulseCount(value);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubeRate"))
            sendCommOkWithFloat(60.0F * getInstantaneousRate(), 3);
        else if (matchCommCommandWithUInt32("GET datalog", &comm.datalogTimeLimit))
        {
            startDatalogDump();

            return;
        }
        else if (matchCommCommand("GET datalog"))
        {
            comm.datalogTimeLimit = 0;

            startDatalogDump();

            return;
        }
        else if (matchCommCommand("GET tubeConversionFactor"))
            sendCommOkWithFloat(getTubeConversionFactor(), 3);
        else if (matchCommCommand("GET tubeDeadTimeCompensation"))
            sendCommOkWithFloat(getTubeDeadTimeCompensation(), 7);
        else if (matchCommCommand("GET tubePWMFrequency"))
            sendCommOkWithFloat(getTubePWMFrequency(), 0);
        else if (matchCommCommand("GET tubePWMDutyCycle"))
            sendCommOkWithFloat(getTubePWMDutyCycle(), 3);
        else if (matchCommCommand("GET tubeDeadTime"))
            sendCommOkWithFloat(getDeadTime(), 7);
        else if (matchCommCommand("GET randomData"))
        {
            sendCommOk();

            for (int i = 0; i < 16; i++)
            {
                int32_t randomData = getRandomData();
                if (randomData < 0)
                    break;

                if (i == 0)
                    strcat(comm.buffer, " ");

                strcatUInt8Hex(comm.buffer, randomData);
            }
        }
#if defined(START_BOOTLOADER_SUPPORT)
        else if (matchCommCommand("START bootloader"))
        {
            comm.startBootloader = true;

            sendCommOk();
        }
#endif
        else
            sendCommError();

        strcat(comm.buffer, "\n");

        transmitComm();
    }
    else if (comm.state == COMM_TX_READY)
    {
        if (comm.sendingDatalog)
        {
            strcpy(comm.buffer, "");

            for (uint32_t i = 0; i < 2;)
            {
                Dose dose;

                if (!readDatalog(&dose))
                {
                    comm.sendingDatalog = false;
                    startDatalog();

                    strcat(comm.buffer, "\n");

                    break;
                }

                if (dose.time > comm.datalogTimeLimit)
                {
                    strcat(comm.buffer, ";");
                    strcatDatalogEntry(comm.buffer, &dose);

                    i++;
                }
            }

            transmitComm();
        }
        else
        {
#if defined(START_BOOTLOADER_SUPPORT)

            if (comm.startBootloader)
                startBootloader();

#endif

            comm.state = COMM_RX;
        }
    }
}
