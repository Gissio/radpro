/*
 * Rad Pro
 * STM32 firmware check
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if defined(STM32F0) || defined(STM32F1) || defined(STM32G0)

#include <libopencm3/stm32/crc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>

#include "../../settings.h"
#include "../../system.h"

#ifndef FLASH_BASE
#define FLASH_BASE 0x08000000
#endif

#define FIRMWARE_BASE ((uint32_t *)FLASH_BASE)
#define FIRMWARE_SIZE (0x8000 - 0x10)

#define VALID_CRC (*(uint32_t *)(FLASH_BASE + FIRMWARE_SIZE + 0x0c))

bool checkFirmware(void)
{
#ifdef CHECK_FIRMWARE
    rcc_periph_clock_enable(RCC_CRC);

    uint32_t crc = crc_calculate_block(FIRMWARE_BASE, FIRMWARE_SIZE / 4);

    rcc_periph_clock_disable(RCC_CRC);

    return (crc == VALID_CRC);
#else
    return true;
#endif
}

#endif
