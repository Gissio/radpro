/*
 * Rad Pro
 * Simulator communications
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <stdio.h>
#include <string.h>

#include "../peripherals/comm.h"
#include "../system/system.h"

#if defined(SIMULATOR_COMM)
#include <sercomm/sercomm.h>

#define COMM_SERIAL_PORT "COM1"
#define COMM_SERIAL_BAUDRATE 115200

static ser_t *sercomm;
#endif

const char *const commId = "Rad Pro simulator;Rad Pro " FIRMWARE_VERSION "/" LANGUAGE;

void initCommHardware(void)
{
}

// Comm is opened even without a serial port (responses are then
// discarded), so updateComm() does not retry the open on every loop
void openComm(void)
{
    clearComm(true);

#if defined(SIMULATOR_COMM)
    sercomm = ser_create();
    if (!sercomm)
    {
        printf("error: could not create serial port instance.\n");

        return;
    }

    ser_opts_t options = {
        COMM_SERIAL_PORT,
        COMM_SERIAL_BAUDRATE,
        SER_BYTESZ_8,
        SER_PAR_NONE,
        SER_STOPB_ONE,
        {0, 0},
    };

    if (ser_open(sercomm, &options))
    {
        printf("error: could not open serial port: %s\n", sererr_last());

        ser_destroy(sercomm);
        sercomm = NULL;
    }
#endif
}

void closeComm(void)
{
#if defined(SIMULATOR_COMM)
    if (sercomm)
    {
        ser_close(sercomm);
        ser_destroy(sercomm);
        sercomm = NULL;
    }
#endif

    clearComm(false);
}

void transmitComm(void)
{
#if defined(SIMULATOR_COMM)
    if (sercomm)
    {
        comm.state = COMM_TX;

        return;
    }
#endif

    // No serial port: discard the response
    endCommTransmit();
}

void pollComm(void)
{
#if defined(SIMULATOR_COMM)
    // sercomm is only set while comm is open
    if (!sercomm)
        return;

    char receiveBuffer[COMM_BUFFER_SIZE];
    size_t receivedBytes = 0;

    // Always read, so data arriving outside COMM_RX is dropped
    ser_read(sercomm,
             receiveBuffer,
             sizeof(receiveBuffer),
             &receivedBytes);

    if (comm.state == COMM_RX)
    {
        // Discard the rest of the data, so it does not overwrite the command
        for (size_t i = 0; i < receivedBytes; i++)
            if (receiveCommChar(receiveBuffer[i]))
                break;
    }
    else if (comm.state == COMM_TX)
    {
        const char *sendBuffer = comm.buffer + comm.bufferIndex;
        size_t sentBytes = 0;

        ser_write(sercomm,
                  sendBuffer,
                  strlen(sendBuffer),
                  &sentBytes);

        comm.bufferIndex += sentBytes;

        if (comm.buffer[comm.bufferIndex] == '\0')
            endCommTransmit();
    }
#endif
}

#endif
