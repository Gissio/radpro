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
#include "display.h"
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
        char c1 = comm.receiveBuffer[index];
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

    return (strlen(comm.receiveBuffer) > (strlen(command) + 1)) &&
           parseUInt32(comm.receiveBuffer + strlen(command) + 1, value);
}

static void sendCommOk(void)
{
    strcpy(comm.sendBuffer, "OK");
}

static void sendCommOkWithString(const char *value)
{
    sendCommOk();
    strcat(comm.sendBuffer, " ");
    strcat(comm.sendBuffer, value);
}

static void sendCommOkWithUInt32(uint32_t value)
{
    sendCommOk();
    strcat(comm.sendBuffer, " ");
    strcatUInt32(comm.sendBuffer, value, 0);
}

static void sendCommOkWithFloat(float value, uint32_t fractionalDecimals)
{
    sendCommOk();
    strcat(comm.sendBuffer, " ");
    strcatFloat(comm.sendBuffer, value, fractionalDecimals);
}

static void sendCommError(void)
{
    strcpy(comm.sendBuffer, "ERROR");
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

    strcat(comm.sendBuffer, " time,tubePulseCount");

    startDatalogDownload();
    comm.sendingDatalog = true;

    transmitComm();
}

void dispatchCommEvents(void)
{
    updateCommController();

    if (comm.state == COMM_RX_READY)
    {
        uint32_t value;

        if (matchCommCommand("GET deviceId"))
        {
            sendCommOkWithString(commId);
            strcat(comm.sendBuffer, ";");
            strcatUInt32Hex(comm.sendBuffer, getDeviceId());
        }
        else if (matchCommCommand("GET deviceBatteryVoltage"))
            sendCommOkWithFloat(getDeviceBatteryVoltage(), 3);
        else if (matchCommCommand("GET deviceTime"))
            sendCommOkWithUInt32(getDeviceTime());
        else if (matchCommCommandWithUInt32("SET deviceTime", &value))
        {
            setDeviceTime(value);

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
        else if (matchCommCommand("GET tubeDeadTime"))
            sendCommOkWithFloat(getTubeDeadTime(), 7);
        else if (matchCommCommand("GET tubeConversionFactor"))
            sendCommOkWithFloat(getTubeConversionFactor(), 3);
        else if (matchCommCommand("GET tubeDeadTimeCompensation"))
            sendCommOkWithFloat(getTubeDeadTimeCompensation(), 7);
        else if (matchCommCommand("GET tubeHVFrequency"))
            sendCommOkWithFloat(getTubeHVFrequency(), 2);
        else if (matchCommCommand("GET tubeHVDutyCycle"))
            sendCommOkWithFloat(getTubeHVDutyCycle(), 4);
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
        else if (matchCommCommand("GET randomData"))
        {
            sendCommOk();

            for (int i = 0; i < 16; i++)
            {
                int32_t randomData = getRandomData();
                if (randomData < 0)
                    break;

                if (i == 0)
                    strcat(comm.sendBuffer, " ");

                strcatUInt8Hex(comm.sendBuffer, randomData);
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

        strcat(comm.sendBuffer, "\n");

        transmitComm();
    }
    else if (comm.state == COMM_TX_READY)
    {
        if (comm.sendingDatalog)
        {
            strcpy(comm.sendBuffer, "");

            uint32_t i = 0;
            while (i < 2)
            {
                Dose dose;

                if (!getDatalogDownloadEntry(&dose))
                {
                    strcat(comm.sendBuffer, "\n");

                    comm.sendingDatalog = false;

                    break;
                }

                if (dose.time >= comm.datalogTimeLimit)
                {
                    strcat(comm.sendBuffer, ";");
                    strcatDatalogEntry(comm.sendBuffer, &dose);

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

            comm.receiveBufferIndex = 0;
            comm.state = COMM_RX;
        }
    }
}

// USB connection mode

#if defined(USB_MODE)

bool commUSBMode;

bool isUSBMode(void)
{
    return commUSBMode;
}

static void onUSBModeEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        commUSBMode = false;

        onSettingsSubMenuBack(NULL);

        break;

    case EVENT_DRAW:
        commUSBMode = true;

        drawNotification("USB mode",
                         "USB connection enabled.",
                         false);

        break;

    default:
        break;
    }
}

const View usbModeView = {
    onUSBModeEvent,
    NULL,
};

#endif
