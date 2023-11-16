/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(COMM_H)

#define COMM_H

#include <stdbool.h>
#include <stdint.h>

#include "datalog.h"

#define COMM_BAUDRATE 115200

#define COMM_BUFFER_SIZE 64

extern const char *const commId;

enum CommPort
{
    COMM_UART,
    COMM_SWD,
};

enum CommState
{
    COMM_RX,
    COMM_RX_READY,
    COMM_TX,
    COMM_TX_READY,
};

struct Comm
{
    volatile enum CommPort port;
    volatile enum CommState state;

    volatile uint32_t bufferIndex;
    char buffer[COMM_BUFFER_SIZE];

    bool sendingDatalog;
    uint32_t datalogTimeLimit;

    bool startBootloader;
};

extern struct Comm comm;

void initComm(void);

void transmitComm(void);

void updateComm(void);
void updateCommHardware(void);

#endif
