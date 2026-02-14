/*
 * Rad Pro
 * Communications
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(COMM_H)
#define COMM_H

#include <stdbool.h>
#include <stdint.h>

#include "../measurements/datalog.h"

#define COMM_BUFFER_SIZE 64

extern const char *const commId;

typedef enum
{
    COMM_RX,
    COMM_RX_READY,
    COMM_TX,
    COMM_TX_READY,
} CommState;

typedef enum
{
    TRANSMIT_RESPONSE = 0,
    TRANSMIT_BOOTLOADER = 1,
    TRANSMIT_DEVICEID = 2,
    TRANSMIT_DATALOG = 3,
    TRANSMIT_RAW = 4,
    TRANSMIT_ERROR = 5,
} TransmitState;

typedef struct
{
    volatile CommState state;

    volatile uint32_t bufferIndex;
    char buffer[COMM_BUFFER_SIZE];

    volatile bool open;
    char lastChar;

    volatile TransmitState transmitState;
    uint32_t datalogStartTime;
    uint32_t datalogEndTime;
    uint32_t datalogMaxRecordNum;
    uint32_t datalogRecordNum;
    DatalogRecord datalogRecord;
} Comm;

extern Comm comm;

void initComm(void);
void initCommHardware(void);

void openComm(void);
void closeComm(void);
void resetComm(bool open);

void transmitComm(void);

void updateComm(void);
void pollComm(void);

#endif
