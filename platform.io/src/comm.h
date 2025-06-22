/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#if !defined(COMM_H)
#define COMM_H

#include <stdbool.h>
#include <stdint.h>

#include "datalog.h"

#define COMM_BUFFER_SIZE 64

extern const char *const commId;

typedef enum
{
    COMM_SERIAL,
    COMM_SWD,
} CommPort;

typedef enum
{
    COMM_RX,
    COMM_RX_READY,
    COMM_TX,
    COMM_TX_READY,
} CommState;

typedef enum
{
    TRANSMIT_DONE,
    TRANSMIT_DEVICEID,
    TRANSMIT_DATALOG,
    TRANSMIT_BOOTLOADER,
} TransmitState;

typedef struct
{
    // vvv These settings should remain fixed for SWD communications.
    volatile CommPort port;
    volatile CommState state;

    volatile uint32_t bufferIndex;
    char buffer[COMM_BUFFER_SIZE];
    // ^^^ These settings should remain fixed for SWD communications.

    volatile bool enabled;
    char lastChar;

    volatile TransmitState transmitState;
    uint32_t datalogTimeLimit;
} Comm;

extern Comm comm;

extern const View dataModeMenuView;

void initComm(void);

void openComm(void);
void closeComm(void);
void resetComm(bool enabled);

void transmitComm(void);

void dispatchCommEvents(void);
void updateComm(void);

#endif
