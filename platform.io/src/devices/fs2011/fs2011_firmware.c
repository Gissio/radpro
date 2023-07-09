/*
 * Rad Pro
 * FS2011 firmware check
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifdef FS2011

#include <libopencm3/stm32/crc.h>
#include <libopencm3/stm32/rcc.h>

#include "../../firmware.h"

#define FIRMWARE_BASE ((uint32_t *)FLASH_BASE)
#define FIRMWARE_SIZE (0x9000 - 0x4)

#define VALID_CRC (*(uint32_t *)(FLASH_BASE + FIRMWARE_SIZE))

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
