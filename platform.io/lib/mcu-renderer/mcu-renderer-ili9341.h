/*
 * MCU renderer
 * ILI9341 driver
 *
 * (C) 2023-2024 Gissio
 *
 * License: MIT
 */

#if !defined(MCURENDERER_ILI9341_H)
#define MCURENDERER_ILI9341_H

#include "mcu-renderer-st7789.h"

// ILI9341 level 1 commands

#define MR_ILI9341_NOP 0x00
#define MR_ILI9341_SWRESET 0x01
#define MR_ILI9341_RDDID 0x04
#define MR_ILI9341_RDDST 0x09
#define MR_ILI9341_RDDPM 0x0a
#define MR_ILI9341_RDDMADCTL 0x0b
#define MR_ILI9341_RDDCOLMOD 0x0c
#define MR_ILI9341_RDDDIM 0x0d
#define MR_ILI9341_RDDDSP 0x0e
#define MR_ILI9341_RDDSDR 0x0f
#define MR_ILI9341_SLPIN 0x10
#define MR_ILI9341_SLPOUT 0x11
#define MR_ILI9341_PTLON 0x12
#define MR_ILI9341_NORON 0x13
#define MR_ILI9341_INVOFF 0x20
#define MR_ILI9341_INVON 0x21
#define MR_ILI9341_GAMSET 0x26
#define MR_ILI9341_DISPOFF 0x28
#define MR_ILI9341_DISPON 0x29
#define MR_ILI9341_CASET 0x2a
#define MR_ILI9341_RASET 0x2b
#define MR_ILI9341_RAMWR 0x2c
#define MR_ILI9341_RAMRD 0x2e
#define MR_ILI9341_PTLAR 0x30
#define MR_ILI9341_VSCRDEF 0x33
#define MR_ILI9341_TEOFF 0x34
#define MR_ILI9341_TEON 0x35
#define MR_ILI9341_MADCTL 0x36
#define MR_ILI9341_MADCTL_MY (1 << 7)
#define MR_ILI9341_MADCTL_MX (1 << 6)
#define MR_ILI9341_MADCTL_MV (1 << 5)
#define MR_ILI9341_MADCTL_BGR (1 << 3)
#define MR_ILI9341_VSCRSADD 0x37
#define MR_ILI9341_IDMOFF 0x38
#define MR_ILI9341_IDMON 0x39
#define MR_ILI9341_COLMOD 0x3a
#define MR_ILI9341_COLMOD_16BPP ((0b101 << 4) | (0b101 << 0))
#define MR_ILI9341_RAMWRC 0x3c
#define MR_ILI9341_RAMRDC 0x3e
#define MR_ILI9341_TESCAN 0x44
#define MR_ILI9341_RDTESCAN 0x45
#define MR_ILI9341_WRDISBV 0x51
#define MR_ILI9341_RDDISBV 0x52
#define MR_ILI9341_WRCTRLD 0x53
#define MR_ILI9341_RDCTRLD 0x54
#define MR_ILI9341_WRCACE 0x55
#define MR_ILI9341_RDCABC 0x56
#define MR_ILI9341_WRCABCMB 0x5e
#define MR_ILI9341_RDCABCMB 0x5f
#define MR_ILI9341_RDABCSDR 0x68
#define MR_ILI9341_RDID1 0xda
#define MR_ILI9341_RDID2 0xdb
#define MR_ILI9341_RDID3 0xde

// ILI9341 Level 2 commands

#define MR_ILI9341_RGBCTRL 0xb0
#define MR_ILI9341_FRCNORMAL 0xb1
#define MR_ILI9341_FRCIDLE 0xb2
#define MR_ILI9341_FRCPARTIAL 0xb3
#define MR_ILI9341_DISPINVCTRL 0xb4
#define MR_ILI9341_PORCTRL 0xb5
#define MR_ILI9341_DISPFUNCCTRL 0xb6
#define MR_ILI9341_ENTRYMODESET 0xb7
#define MR_ILI9341_BACKLIGHTCTRL1 0xb8
#define MR_ILI9341_BACKLIGHTCTRL2 0xb9
#define MR_ILI9341_BACKLIGHTCTRL3 0xba
#define MR_ILI9341_BACKLIGHTCTRL4 0xbb
#define MR_ILI9341_BACKLIGHTCTRL5 0xbc
#define MR_ILI9341_BACKLIGHTCTRL6 0xbd
#define MR_ILI9341_BACKLIGHTCTRL7 0xbe
#define MR_ILI9341_BACKLIGHTCTRL8 0xbf
#define MR_ILI9341_POWERCTRL1 0xc0
#define MR_ILI9341_POWERCTRL2 0xc1
#define MR_ILI9341_VCOMCTRL1 0xc5
#define MR_ILI9341_VCOMCTRL2 0xc7
#define MR_ILI9341_NVMEMWR 0xd0
#define MR_ILI9341_NVMEMPROT 0xd1
#define MR_ILI9341_NVMEMRD 0xd2
#define MR_ILI9341_ID4RD 0xd3
#define MR_ILI9341_POSGAMMACORR 0xe0
#define MR_ILI9341_NEGGAMMACORR 0xe1
#define MR_ILI9341_DIGGAMMACTRL1 0xe2
#define MR_ILI9341_DIGGAMMACTRL2 0xe3
#define MR_ILI9341_IFCTRL 0xf6

// ILI9341 extended register commands

#define MR_ILI9341_POWCTRLA 0xcb
#define MR_ILI9341_POWCTRLB 0xcf
#define MR_ILI9341_TIMINGCTRLA1 0xe8
#define MR_ILI9341_TIMINGCTRLA2 0xe9
#define MR_ILI9341_POWERONSEQCTRL 0xed
#define MR_ILI9341_TIMINGCTRLB 0xea
#define MR_ILI9341_ENABLE3G 0xf2
#define MR_ILI9341_PUMPRATIOCTRL 0xf7

/**
 * Initializes an mcu-renderer ILI9341 instance.
 *
 * @param mr The mcu-renderer instance.
 * @param width The physical pixel width of the display (e.g. 240).
 * @param height The physical pixel height of the display (e.g. 320).
 * @param rotation The desired display rotation.
 * @param textbuffer A user-provided buffer for rendering text.
 * @param textbuffer_size  The size of the user-provided buffer.
 * @param sleep_callback A user-provided sleep callback.
 * @param set_reset_callback A user-provided set reset callback.
 * @param set_command_callback A user-provided set command callback.
 * @param send_callback A user-provided send 8-bit data callback.
 * @param send_callback A user-provided send 16-bit data callback.
 */
static inline void mr_ili9341_init(mr_t *mr,
                                   int16_t width,
                                   int16_t height,
                                   mr_display_rotation_t rotation,
                                   uint8_t *textbuffer,
                                   uint32_t textbuffer_size,
                                   mr_sleep_callback_t sleep_callback,
                                   mr_set_reset_callback_t set_reset_callback,
                                   mr_set_command_callback_t set_command_callback,
                                   mr_send_callback_t send_callback,
                                   mr_send_callback_t send16_callback)
{
    mr_st7789_init(mr,
                   width,
                   height,
                   rotation,
                   textbuffer,
                   textbuffer_size,
                   sleep_callback,
                   set_reset_callback,
                   set_command_callback,
                   send_callback,
                   send16_callback);
}

/**
 * Enables/disables the ILI9341 display.
 *
 * @param mr The mcu-renderer instance.
 * @param value Display enable.
 */
static inline void mr_ili9341_set_display(mr_t *mr,
                                          bool value)
{
    mr_st7789_set_display(mr, value);
}

/**
 * Enables/disables ILI9341 sleep mode. Takes 120 ms to finish.
 *
 * @param mr The mcu-renderer instance.
 * @param value Display enable.
 */
static inline void mr_ili9341_set_sleep(mr_t *mr,
                                        bool value)
{
    mr_st7789_set_sleep(mr, value);
}

#endif