/*
 * Rad Pro
 * Simulator communications
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>

#include <sercomm/sercomm.h>

#include "../comm.h"
#include "../system.h"

#define COMM_SERIAL_BAUDRATE 115200
#define COMM_BUFFER_SIZE 64

const char *const commId = "Rad Pro simulator;Rad Pro " FIRMWARE_VERSION;

static struct
{
    ser_t *sercomm;
    char lastChar;
} commController;

void startComm(void)
{
    if (commController.sercomm)
        return;

    commController.sercomm = ser_create();
    if (commController.sercomm == NULL)
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

    int32_t result = ser_open(commController.sercomm, &options);
    if (result)
    {
        printf("Could not open serial port: %s\n", sererr_last());

        ser_destroy(commController.sercomm);
        commController.sercomm = NULL;

        return;
    }
}

void stopComm(void)
{
    ser_close(commController.sercomm);
    ser_destroy(commController.sercomm);

    commController.sercomm = NULL;
}

bool isCommStarted(void)
{
    return (commController.sercomm != NULL);
}

void transmitComm(void)
{
    comm.state = COMM_TX;
}

void updateCommController(void)
{
    if (commController.sercomm == NULL)
        return;

    char receiveBuffer[COMM_BUFFER_SIZE];
    size_t receivedBytes = 0;

    ser_read(commController.sercomm,
             receiveBuffer,
             COMM_BUFFER_SIZE,
             &receivedBytes);

    if (comm.enabled &&
        (comm.state == COMM_RX))
    {
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
                      (commController.lastChar != '\r')))
            {
                comm.buffer[comm.bufferIndex] = '\0';

                comm.bufferIndex = 0;
                comm.state = COMM_RX_READY;
            }

            commController.lastChar = c;
        }
    }

    if (comm.state == COMM_TX)
    {
        char *sendBuffer = comm.buffer + comm.bufferIndex;
        size_t sentBytes = 0;

        ser_write(commController.sercomm,
                  sendBuffer,
                  strlen(sendBuffer),
                  &sentBytes);

        comm.bufferIndex += sentBytes;

        if ((comm.bufferIndex > 0) &&
            (comm.buffer[comm.bufferIndex - 1] == '\n'))
        {
            comm.bufferIndex = 0;
            comm.state = COMM_RX;
        }
        else if (comm.buffer[comm.bufferIndex] == '\0')
        {
            comm.bufferIndex = 0;
            comm.state = COMM_TX_READY;
        }
    }
}

#endif
