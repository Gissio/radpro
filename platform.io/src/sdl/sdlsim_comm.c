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

const char *const commId = "Rad Pro simulator;Rad Pro " FIRMWARE_VERSION;

static struct
{
    ser_t *sercomm;
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

    char c;

    while (ser_read(commController.sercomm,
                    &c,
                    1,
                    NULL) == 0)
    {
        if (comm.enabled &&
            (comm.state == COMM_RX))
        {
            if ((c >= ' ') &&
                (comm.bufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.buffer[comm.bufferIndex++] = c;
            else if ((c == '\n') &&
                     (comm.bufferIndex < COMM_BUFFER_SIZE))
            {
                comm.buffer[comm.bufferIndex] = '\0';

                comm.bufferIndex = 0;
                comm.state = COMM_RX_READY;
            }
        }
    }

    while (comm.state == COMM_TX)
    {
        c = comm.buffer[comm.bufferIndex];

        if (c != '\0')
        {
            ser_write(commController.sercomm,
                      &c,
                      1,
                      NULL);

            comm.bufferIndex++;

            if (c == '\n')
            {
                comm.bufferIndex = 0;
                comm.state = COMM_RX;
            }
        }
        else
        {
            comm.bufferIndex = 0;
            comm.state = COMM_TX_READY;
        }
    }
}

#endif
