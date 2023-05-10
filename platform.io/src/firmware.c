/*
 * FS2011 Pro
 * Firmware check
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "events.h"
#include "firmware.h"
#include "main.h"

#define FIRMWARE_BASE FLASH_BASE
#define FIRMWARE_SIZE (0xc000 - 0x4)

#define VALID_CRC (*(uint32_t *)(FIRMWARE_BASE + FIRMWARE_SIZE))

void checkFirmware(void)
{
#ifdef CHECK_FIRMWARE
    rcc_periph_clock_enable(RCC_CRC);

    crc_reset();
    uint32_t crc = crc_calculate_block(FIRMWARE_BASE, FIRMWARE_SIZE / 4);

    rcc_periph_clock_disable(RCC_CRC);

    if (crc != VALID_CRC)
    {
        for (uint32_t i = 0; i < 10; i++)
        {
            setBuzzer(true);
            waitSysTicks(50);
            setBuzzer(false);
            waitSysTicks(50);
        }
    }
#endif
}
