/*
 * FS2011 Pro
 * GD32 firmware library fake header
 *
 * Parts: Copyright (c) 2022, GigaDevice Semiconductor Inc.
 */

#include "gd32f1x0.h"

#define OB_RDP (*((uint32_t *)0x1ffff800))

bool isGD32(void)
{
    return (OB_RDP & 0xff) == 0xa5;
}
