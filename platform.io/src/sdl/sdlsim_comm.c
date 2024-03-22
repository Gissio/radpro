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

const char *const commId = "Rad Pro simulator;Rad Pro " FIRMWARE_VERSION;

static struct
{
    ser_t *port;
} commController;

void initComm(void)
{
    commController.port = ser_create();
    if (commController.port == NULL)
    {
        printf("Could not create serial port instance.\n");

        return;
    }

    ser_opts_t options = {
        "COM1",
        115200,
        SER_BYTESZ_8,
        SER_PAR_NONE,
        SER_STOPB_ONE,
        {0, 0},
    };

    int32_t result = ser_open(commController.port, &options);
    if (result)
    {
        printf("Could not open serial port: %s\n", sererr_last());

        ser_destroy(commController.port);
        commController.port = NULL;

        return;
    }
}

void transmitComm(void)
{
    comm.receiveBufferIndex = 0;
    comm.sendBufferIndex = 0;
    comm.state = COMM_TX;
}

void updateCommController(void)
{
    if (commController.port == NULL)
        return;

    char c;
    while (ser_read(commController.port,
                    &c,
                    1,
                    NULL) == 0)
    {
        if (comm.enabled)
        {
            if ((c >= ' ') &&
                (comm.receiveBufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.receiveBuffer[comm.receiveBufferIndex++] = c;
            else if ((c == '\n') &&
                     (comm.receiveBufferIndex < COMM_BUFFER_SIZE))
            {
                comm.receiveBuffer[comm.receiveBufferIndex++] = '\0';
                comm.state = COMM_RX_READY;
            }
        }
    }

    while (comm.state == COMM_TX)
    {
        if (comm.sendBuffer[comm.sendBufferIndex] != '\0')
            ser_write(commController.port,
                      &comm.sendBuffer[comm.sendBufferIndex++],
                      1,
                      NULL);
        else
            comm.state = COMM_TX_READY;
    }
}

#endif
