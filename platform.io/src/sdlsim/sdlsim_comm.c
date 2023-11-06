/*
 * Rad Pro
 * SDLSim communications
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(SDLSIM)

#include <stdio.h>

#include <sercomm/sercomm.h>

#include "../comm.h"
#include "../system.h"

const char *const commId = "SDLSim;Rad Pro " FIRMWARE_VERSION;

struct
{
    ser_t *port;
} commHardware;

void initComm(void)
{
    commHardware.port = ser_create();

    if (commHardware.port == NULL)
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

    int32_t result = ser_open(commHardware.port, &options);
    if (result)
    {
        printf("Could not open serial port: %s\n", sererr_last());
        return;
    }
}

void transmitComm(void)
{
    comm.state = COMM_TX;
}

void updateCommHardware(void)
{
    char c;

    while (ser_read(commHardware.port, &c, 1, NULL) == 0)
    {
        comm.port = COMM_UART;

        if (comm.state == COMM_RX)
        {
            if ((c >= ' ') && (comm.bufferIndex < (COMM_BUFFER_SIZE - 1)))
                comm.buffer[comm.bufferIndex++] = c;
            else if (c == '\n')
            {
                comm.buffer[comm.bufferIndex] = '\0';
                comm.bufferIndex = 0;

                comm.state = COMM_RX_READY;
            }
        }
    }

    while ((comm.port == COMM_UART) && (comm.state == COMM_TX))
    {
        if (comm.buffer[comm.bufferIndex] != '\0')
            ser_write(commHardware.port, &comm.buffer[comm.bufferIndex++], 1, NULL);
        else
        {
            comm.bufferIndex = '\0';

            comm.state = COMM_TX_READY;
        }
    }
}

#endif
