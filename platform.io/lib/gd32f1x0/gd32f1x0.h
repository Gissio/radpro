/*
 * Rad Pro
 * GD32 firmware library fake header
 *
 * Parts: Copyright (c) 2022, GigaDevice Semiconductor Inc.
 */

#ifndef GD32F1X0_H
#define GD32F1X0_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    DISABLE = 0,
    ENABLE = !DISABLE
} EventStatus,
    ControlStatus;

enum
{
    FALSE = 0,
    TRUE = !FALSE
};

typedef enum
{
    RESET = 0,
    SET = !RESET
} FlagStatus;

typedef enum
{
    ERROR = 0,
    SUCCESS = !ERROR
} ErrStatus;

#define REG32(addr) (*(volatile uint32_t *)(uint32_t)(addr))
#define REG16(addr) (*(volatile uint16_t *)(uint32_t)(addr))
#define REG8(addr) (*(volatile uint8_t *)(uint32_t)(addr))
#define BIT(x) ((uint32_t)((uint32_t)0x01U << (x)))
#define BITS(start, end) ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end))))
#define GET_BITS(regval, start, end) (((regval)&BITS((start), (end))) >> (start))

#ifndef ADC_BASE
#define ADC_BASE 0x40012400
#endif

bool isGD32(void);

#endif
