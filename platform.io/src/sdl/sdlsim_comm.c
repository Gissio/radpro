/*
 * Rad Pro
 * Simulator communications
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>
#include <string.h>

#include "../comm.h"
#include "../cstring.h"
#include "../system.h"

const char *const commId = "Rad Pro simulator;Rad Pro " FIRMWARE_VERSION;

void initComm(void)
{
}

#if defined(SIMULATE_COMM)

#include <sercomm/sercomm.h>

#define COMM_SERIAL_BAUDRATE 115200

static ser_t *sercomm;

void openComm(void)
{
    if (comm.enabled)
        return;

    sercomm = ser_create();
    if (sercomm == NULL)
    {
        printf("Could not create serial port instance.\n");

        return;
    }

    ser_opts_t options = {
        "COM1",
        COMM_SERIAL_BAUDRATE,
        SER_BYTESZ_8,
        SER_PAR_NONE,
        SER_STOPB_ONE,
        {0, 0},
    };

    int32_t result = ser_open(sercomm, &options);
    if (result)
    {
        printf("Could not open serial port: %s\n", sererr_last());

        ser_destroy(sercomm);
        sercomm = NULL;

        return;
    }

    resetComm(true);
}

void closeComm(void)
{
    if (!comm.enabled)
        return;

    ser_close(sercomm);
    ser_destroy(sercomm);

    resetComm(false);
}

void transmitComm(void)
{
    comm.state = COMM_TX;
}

void updateComm(void)
{
    char receiveBuffer[COMM_BUFFER_SIZE];
    size_t receivedBytes = 0;

    if (sercomm)
        ser_read(sercomm,
                 receiveBuffer,
                 COMM_BUFFER_SIZE,
                 &receivedBytes);

    if (!comm.enabled)
        return;

    switch (comm.state)
    {
    case COMM_RX:
        for (int32_t i = 0;
             i < receivedBytes;
             i++)
        {
            char c = receiveBuffer[i];

            if ((c >= ' ') &&
                (comm.bufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.buffer[comm.bufferIndex++] = c;
            else if ((c == '\r') ||
                     ((c == '\n') &&
                      (comm.lastChar != '\r')))
            {
                comm.buffer[comm.bufferIndex] = '\0';

                comm.bufferIndex = 0;
                comm.state = COMM_RX_READY;
            }

            comm.lastChar = c;
        }

        break;

    case COMM_TX:
    {
        char *sendBuffer = comm.buffer + comm.bufferIndex;
        size_t sentBytes = 0;

        ser_write(sercomm,
                  sendBuffer,
                  strlen(sendBuffer),
                  &sentBytes);

        comm.bufferIndex += sentBytes;

        if (comm.buffer[comm.bufferIndex] == '\0')
        {
            strclr(comm.buffer);
            comm.bufferIndex = 0;

            if (comm.transmitState == TRANSMIT_DONE)
                comm.state = COMM_RX;
            else
                comm.state = COMM_TX_READY;
        }

        break;
    }
    }
}

#else

void openComm(void)
{
    resetComm(true);
}

void closeComm(void)
{
    resetComm(false);
}

void transmitComm(void)
{
    strclr(comm.buffer);
    comm.bufferIndex = 0;

    if (comm.transmitState == TRANSMIT_DONE)
        comm.state = COMM_RX;
    else
        comm.state = COMM_TX_READY;
}

void updateComm(void)
{
}

#endif

#endif
