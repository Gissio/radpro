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

#define COMM_BUFFER_SIZE 128

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

    bool sendingDatalog;
    uint32_t datalogTimeLimit;

#if defined(START_BOOTLOADER)
    bool startBootloader;
#endif
} Comm;

extern Comm comm;

extern const View dataModeMenuView;

void openComm(void);
void closeComm(void);
bool isCommOpen(void);

void transmitComm(void);

void dispatchCommEvents(void);
void updateComm(void);

#endif
