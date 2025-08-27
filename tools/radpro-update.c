/*
 * Rad Pro
 * Update script for OpenWRT routers
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define TTY_DEVICE "/dev/ttyUSB0"
#define TTY_BAUDRATE B115200

#define CHECK_TIME 60
#define CLOCK_SET_TIME 3600

#define LOG_BASEPATH "/www/www/radpro/"

void logError(const char *format, ...)
{
    time_t currentTime = time(NULL);
    const struct tm *currentDateTime = localtime(&currentTime);

    fprintf(stderr,
            "%04d-%02d-%02d %02d:%02d:%02d ",
            currentDateTime->tm_year + 1900,
            currentDateTime->tm_mon + 1,
            currentDateTime->tm_mday,
            currentDateTime->tm_hour,
            currentDateTime->tm_min,
            currentDateTime->tm_sec);

    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

bool openSerialPort(const char *path, int *serialPort)
{
    *serialPort = open(path, O_RDWR | O_NOCTTY);

    return ((*serialPort) >= 0);
}

void closeSerialPort(int serialPort)
{
    close(serialPort);
}

bool configureSerialPort(int serialPort)
{
    struct termios options;
    int result;

    result = tcgetattr(serialPort, &options);
    if (result != 0)
        return false;

    // Turn off options
    options.c_iflag &= ~(INLCR | IGNCR | ICRNL | IXON | IXOFF);
    options.c_oflag &= ~(ONLCR | OCRNL);
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN] = 0;

    cfsetospeed(&options, TTY_BAUDRATE);
    cfsetispeed(&options, TTY_BAUDRATE);

    result = tcsetattr(serialPort, TCSANOW, &options);
    if (result != 0)
        return false;

    return true;
}

bool writeSerialPortLine(int serialPort, char *buffer)
{
    strcat(buffer, "\r\n");

    int bufferSize = strlen(buffer);

    return (write(serialPort, buffer, bufferSize) == bufferSize);
}

bool readSerialPortLine(int serialPort, char *buffer, int bufferSize)
{
    int index = 0;

    for (int i = 0; i < 10; i++)
    {
        size_t n = read(serialPort,
                        buffer + index,
                        bufferSize - index - 1);
        if (n < 0)
            break;

        index += n;

        if ((index > 0) &&
            (buffer[index - 1] == '\n'))
        {
            buffer[index - 1] = '\0';

            return true;
        }
    }

    buffer[0] = '\0';

    return false;
}

bool sendRequest(int serialPort,
                 char *buffer,
                 int bufferSize)
{
    if (!writeSerialPortLine(serialPort, buffer))
    {
        logError("Could not send request.\n");

        return false;
    }

    if (!readSerialPortLine(serialPort, buffer, bufferSize))
    {
        logError("Could not receive response.\n");

        return false;
    }

    if (strncmp(buffer, "OK", 2) != 0)
    {
        logError("Invalid response: %s\n", buffer);

        return false;
    }

    if (strlen(buffer) < 3)
        strcpy(buffer, "");
    else
        strcpy(buffer, buffer + 3);

    return true;
}

void appendLogRecord(time_t recordTime,
                     uint32_t deltaPulseCount)
{
    const struct tm *currentDateTime = localtime(&recordTime);

    char path[256];
    sprintf(path,
            LOG_BASEPATH "%04d-%02d-%02d.log",
            currentDateTime->tm_year + 1900,
            currentDateTime->tm_mon + 1,
            currentDateTime->tm_mday);

    FILE *fp = fopen(path, "at");

    if (fp)
    {
        const struct tm *dateTime = localtime(&recordTime);

        fprintf(fp,
                "%04d-%02d-%02d %02d:%02d:%02d,%u\n",
                dateTime->tm_year + 1900,
                dateTime->tm_mon + 1,
                dateTime->tm_mday,
                dateTime->tm_hour,
                dateTime->tm_min,
                dateTime->tm_sec,
                deltaPulseCount);

        fclose(fp);
    }
}

int main(int argc, char *argv[])
{
    time_t lastCheckTime = 0;
    time_t lastLogTime = 0;
    time_t lastDeviceTime = 0;

    uint32_t lastPulseCount = 0;

    while (true)
    {
        time_t currentTime = time(NULL);
        time_t deltaTime = currentTime - lastCheckTime;

        if (deltaTime >= CHECK_TIME)
        {
            int serialPort;

            if (!openSerialPort(TTY_DEVICE, &serialPort))
                logError("Could not open %s.\n", TTY_DEVICE);
            else
            {
                if (!configureSerialPort(serialPort))
                    logError("Could not configure %s.\n", TTY_DEVICE);
                else
                {
                    char buffer[256];

                    strcpy(buffer, "GET tubePulseCount");

                    if (sendRequest(serialPort,
                                    buffer,
                                    sizeof(buffer)))
                    {
                        uint32_t pulseCount = strtoul(buffer, NULL, 10);

                        if ((lastLogTime > 0) && (deltaTime < (2 * CHECK_TIME)))
                        {
                            uint32_t deltaPulseCount = pulseCount - lastPulseCount;

                            appendLogRecord(currentTime, deltaPulseCount);
                        }

                        lastLogTime = currentTime;
                        lastPulseCount = pulseCount;
                    }

                    if ((currentTime - lastCheckTime) < (2 * CHECK_TIME))
                        lastCheckTime += CHECK_TIME;
                    else
                        lastCheckTime = currentTime;

                    if ((currentTime - lastDeviceTime) >= CLOCK_SET_TIME)
                    {
                        sprintf(buffer,
                                "SET deviceTime %u",
                                (uint32_t)currentTime);

                        sendRequest(serialPort,
                                    buffer,
                                    sizeof(buffer));

                        lastDeviceTime = currentTime;
                    }
                }

                closeSerialPort(serialPort);
            }
        }

        usleep(100000);
    }
}
