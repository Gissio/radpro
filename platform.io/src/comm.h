/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2024 Gissio
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
    COMM_SERIAL,
    COMM_SWD,
};

enum CommState
{
    COMM_RX,
    COMM_RX_READY,
    COMM_TX,
    COMM_TX_READY,
};

typedef struct
{
    // vvv These settings should remain fixed for SWD communications.
    volatile enum CommPort port;
    volatile enum CommState state;

    volatile uint32_t bufferIndex;
    char buffer[COMM_BUFFER_SIZE];
    // ^^^ These settings should remain fixed for SWD communications.

    bool enabled;

    bool sendingDatalog;
    uint32_t datalogTimeLimit;

#if defined(START_BOOTLOADER_SUPPORT)
    bool startBootloader;
#endif
} Comm;

extern Comm comm;

void initComm(void);

void setCommEnabled(bool value);

void transmitComm(void);

void dispatchCommEvents(void);
void updateCommHardware(void);

#endif
