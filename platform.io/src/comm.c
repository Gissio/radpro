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

#if defined(DATA_MODE)
static const Menu dataModeMenu;
#endif

void initComm(void)
{
#if !defined(DATA_MODE)
    openComm();
#else
    if (settings.dataMode)
        openComm();
#endif
}

void resetComm(void)
{
#if defined(DATA_MODE)
    selectMenuItem(&dataModeMenu,
                   settings.dataMode,
                   DATAMODE_NUM);
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

static bool matchCommCommandWithNumber(const char *command,
                                       uint32_t *mantissa,
                                       uint32_t *factor)
{
    if (!matchCommCommand(command))
        return false;

    return (strlen(comm.buffer) > (strlen(command) + 1)) &&
           parseNumber(comm.buffer + strlen(command) + 1,
                       mantissa,
                       factor);
}

static void sendCommOk(void)
{
    strcpy(comm.buffer, "OK");
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
    strcpy(comm.buffer, "ERROR");
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
        uint32_t mantissa;
        uint32_t factor;

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
        else if (matchCommCommandWithNumber("SET deviceTime",
                                            &mantissa,
                                            &factor))
        {
            setDeviceTime(mantissa / factor);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubeTime"))
            sendCommOkWithUInt32(getTubeTime());
        else if (matchCommCommandWithNumber("SET tubeTime",
                                            &mantissa,
                                            &factor))
        {
            setTubeTime(mantissa / factor);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubePulseCount"))
            sendCommOkWithUInt32(getTubePulseCount());
        else if (matchCommCommandWithNumber("SET tubePulseCount",
                                            &mantissa,
                                            &factor))
        {
            setTubePulseCount(mantissa / factor);

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
        else if (matchCommCommand("GET tubeBackgroundCompensation"))
            sendCommOkWithFloat(60.0F * getTubeBackgroundCompensation(), 3);
#if defined(TUBE_HV_PWM)
        else if (matchCommCommand("GET tubeHVFrequency"))
            sendCommOkWithFloat(getTubeHVFrequency(), 2);
        else if (matchCommCommandWithNumber("SET tubeHVFrequency",
                                            &mantissa,
                                            &factor))
        {
            setTubeHVFrequency((float)mantissa / (float)factor);

            sendCommOk();
        }
        else if (matchCommCommand("GET tubeHVDutyCycle"))
            sendCommOkWithFloat(getTubeHVDutyCycle(), 5);
        else if (matchCommCommandWithNumber("SET tubeHVDutyCycle",
                                            &mantissa,
                                            &factor))
        {
            setTubeHVDutyCycle((float)mantissa / (float)factor);

            sendCommOk();
        }
#endif
        else if (matchCommCommandWithNumber("GET datalog",
                                            &mantissa,
                                            &factor))
        {
            comm.datalogTimeLimit = mantissa / factor;

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

// Communications mode

#if defined(DATA_MODE)

static const char *const dataModeMenuOptions[] = {
    "Off",
    "On",
    NULL,
};

static const char *onDataModeMenuGetOption(const Menu *menu,
                                           uint32_t index,
                                           MenuStyle *menuStyle)
{
    *menuStyle = (index == isCommOpen());

    return dataModeMenuOptions[index];
}

static void onDataModeMenuSelect(const Menu *menu)
{
    uint32_t selectedIndex = menu->state->selectedIndex;

    settings.dataMode = selectedIndex;

    if (selectedIndex)
        openComm();
    else
        closeComm();
}

static MenuState dataModeMenuState;

static const Menu dataModeMenu = {
    "Data mode",
    &dataModeMenuState,
    onDataModeMenuGetOption,
    onDataModeMenuSelect,
    onSettingsSubMenuBack,
};

const View dataModeMenuView = {
    onMenuEvent,
    &dataModeMenu,
};

#endif
