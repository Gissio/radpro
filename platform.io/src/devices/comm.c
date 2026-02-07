/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdio.h>

#include "../devices/display.h"
#include "../devices/comm.h"
#include "../devices/rtc.h"
#include "../devices/tube.h"
#include "../extras/rng.h"
#include "../measurements/datalog.h"
#include "../measurements/electricfield.h"
#include "../measurements/magneticfield.h"
#include "../measurements/instantaneous.h"
#include "../measurements/measurements.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../system/system.h"

#define START_BOOTLOADER_TIME 200

Comm comm;

void initComm(void)
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

static void pushCommString(const char *value)
{
    pushCommOkSpace();
    strcat(comm.buffer, value);
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
    switch (comm.state)
    {
    case COMM_RX_READY:
    {
        const char *s = comm.buffer;
        comm.transmitState = TRANSMIT_ERROR;

        if (parseToken(&s, "GET"))
        {
            enum GetAction
            {
                GET_DEVICE_ID,
                GET_DEVICE_POWER,
                GET_DEVICE_BATTERY_VOLTAGE,
                GET_DEVICE_TIME,
                GET_DEVICE_TIME_ZONE,
                GET_TUBE_TIME,
                GET_TUBE_PULSE_COUNT,
                GET_TUBE_RATE,
                GET_TUBE_DEAD_TIME,
                GET_TUBE_SENSITIVITY,
                GET_TUBE_DEADTIMECOMPENSATION,
#if defined(TUBE_HV_PWM)
                GET_TUBE_HV_FREQUENCY,
                GET_TUBE_HV_DUTY_CYCLE,
#endif
                GET_DATALOG,
                GET_RANDOM_DATA
            };

            const char *getTable[] = {
                "deviceId",
                "devicePower",
                "deviceBatteryVoltage",
                "deviceTime",
                "deviceTimeZone",
                "tubeTime",
                "tubePulseCount",
                "tubeRate",
                "tubeDeadTime",
                "tubeSensitivity",
                "tubeDeadTimeCompensation",
#if defined(TUBE_HV_PWM)
                "tubeHVFrequency",
                "tubeHVDutyCycle",
#endif
                "datalog",
                "randomData"};

            for (size_t i = 0; i < sizeof(getTable) / sizeof(getTable[0]); i++)
            {
                if (!parseToken(&s, getTable[i]))
                    continue;

                switch ((enum GetAction)i)
                {
                case GET_DEVICE_ID:
                    pushCommString(commId);
                    strcatChar(comm.buffer, ';');
                    comm.transmitState = TRANSMIT_DEVICEID;

                    break;

                case GET_DEVICE_POWER:
                    pushCommUInt32(isPoweredOn());

                    break;

                case GET_DEVICE_BATTERY_VOLTAGE:
                    pushCommFloat(getBatteryVoltage(), 3);

                    break;

                case GET_DEVICE_TIME:
                    pushCommUInt32(getDeviceTime());

                    break;

                case GET_DEVICE_TIME_ZONE:
                    pushCommFloat(getDeviceTimeZone(), 1);

                    break;

                case GET_TUBE_TIME:
                    pushCommUInt32(getTubeTime());

                    break;

                case GET_TUBE_PULSE_COUNT:
                    pushCommUInt32(getTubePulseCount());

                    break;

                case GET_TUBE_RATE:
                    pushCommFloat(60.0F * getInstantaneousRate(), 3);

                    break;

                case GET_TUBE_DEAD_TIME:
                    pushCommFloat(getTubeDeadTime(), 7);

                    break;

                case GET_TUBE_SENSITIVITY:
                    pushCommFloat(getTubeSensitivity(), 3);

                    break;

                case GET_TUBE_DEADTIMECOMPENSATION:
                    pushCommFloat(getTubeDeadTimeCompensation(), 7);

                    break;

#if defined(TUBE_HV_PWM)
                case GET_TUBE_HV_FREQUENCY:
                    pushCommFloat(getTubeHVFrequency(), 2);

                    break;

                case GET_TUBE_HV_DUTY_CYCLE:
                    pushCommFloat(getTubeHVDutyCycle(), 5);

                    break;
#endif

                case GET_DATALOG:
                    if (startDatalogDump())
                    {
                        comm.datalogStartTime = 0;
                        comm.datalogEndTime = UINT32_MAX;
                        comm.datalogMaxRecordNum = UINT32_MAX;
                        comm.datalogRecordNum = 0;
                        if (parseUInt32(&s, &comm.datalogStartTime))
                        {
                            if (parseUInt32(&s, &comm.datalogEndTime))
                                parseUInt32(&s, &comm.datalogMaxRecordNum);
                        }
                        pushCommOk();
                        strcat(comm.buffer, " time,tubePulseCount");
                        comm.transmitState = TRANSMIT_DATALOG;
                    }
                    break;

                case GET_RANDOM_DATA:
                    pushCommOk();
                    for (uint32_t j = 0; j < 16; j++)
                    {
                        int32_t value = getRNGByte();
                        if (value < 0)
                            break;
                        if (j == 0)
                            strcatChar(comm.buffer, ' ');
                        strcatUInt8Hex(comm.buffer, value);
                    }

                    break;
                }

                break;
            }
        }
        else if (parseToken(&s, "SET"))
        {
            enum SetAction
            {
                SET_DEVICE_POWER,
                SET_DEVICE_TIME,
                SET_DEVICE_TIMEZONE,
                SET_TUBE_TIME,
                SET_TUBE_PULSECOUNT,
#if defined(TUBE_HV_PWM)
                SET_TUBE_HV_FREQUENCY,
                SET_TUBE_HV_DUTYCYCLE,
#endif
                SET_NONE
            };

            const char *setTable[] = {
                "devicePower",
                "deviceTime",
                "deviceTimeZone",
                "tubeTime",
                "tubePulseCount",
#if defined(TUBE_HV_PWM)
                "tubeHVFrequency",
                "tubeHVDutyCycle",
#endif
            };

            for (size_t i = 0; i < sizeof(setTable) / sizeof(setTable[0]); i++)
            {
                if (!parseToken(&s, setTable[i]))
                    continue;

                uint32_t intValue;
                float floatValue;

                switch ((enum SetAction)i)
                {
                case SET_DEVICE_POWER:
                    if (parseUInt32(&s, &intValue))
                    {
                        if (intValue)
                            powerOn(false);
                        else
                            powerOff(true);
                        pushCommOk();
                    }

                    break;

                case SET_DEVICE_TIME:
                    if (parseUInt32(&s, &intValue) && setDeviceTime(intValue))
                    {
                        logDatalogTimeChange();
                        pushCommOk();
                    }

                    break;

                case SET_DEVICE_TIMEZONE:
                    if (parseFloat(&s, &floatValue) && setDeviceTimeZone(floatValue))
                        pushCommOk();

                    break;

                case SET_TUBE_TIME:
                    if (parseUInt32(&s, &intValue))
                    {
                        setTubeTime(intValue);
                        pushCommOk();
                    }

                    break;

                case SET_TUBE_PULSECOUNT:
                    if (parseUInt32(&s, &intValue))
                    {
                        setTubePulseCount(intValue);
                        pushCommOk();
                    }

                    break;

#if defined(TUBE_HV_PWM)
                case SET_TUBE_HV_FREQUENCY:
                    if (parseFloat(&s, &floatValue) && setTubeHVFrequency(floatValue))
                        pushCommOk();

                    break;

                case SET_TUBE_HV_DUTYCYCLE:
                    if (parseFloat(&s, &floatValue) && setTubeHVDutyCycle(floatValue))
                        pushCommOk();

                    break;
#endif

                default:
                    break;
                }

                break;
            }
        }
        else if (parseToken(&s, "RESET datalog"))
        {
            resetDatalog();
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
            setDisplayEnabled(false);
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
            uint32_t sentRecordNum = 0;
            uint32_t readRecordNum = 0;

            DatalogRecord datalogRecord;

            while ((sentRecordNum < 2) &&
                   (readRecordNum < 1000))
            {
                if (!readDatalog(&datalogRecord))
                {
                    strcat(comm.buffer, "\r\n");
                    comm.transmitState = TRANSMIT_RESPONSE;

                    break;
                }

                if ((datalogRecord.dose.time >= comm.datalogStartTime) &&
                    (datalogRecord.dose.time <= comm.datalogEndTime) &&
                    (comm.datalogRecordNum < comm.datalogMaxRecordNum))
                {
                    if (datalogRecord.sessionStart)
                        strcatChar(comm.buffer, ';');
                    strcatChar(comm.buffer, ';');
                    strcatUInt32(comm.buffer, datalogRecord.dose.time, 0);
                    strcatChar(comm.buffer, ',');
                    strcatUInt32(comm.buffer, datalogRecord.dose.pulseCount, 0);

                    sentRecordNum++;
                    comm.datalogRecordNum++;
                }

                readRecordNum++;
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
