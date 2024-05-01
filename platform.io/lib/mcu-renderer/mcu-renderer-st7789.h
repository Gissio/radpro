/*
 * MCU renderer
 * ST7789 driver
 *
 * (C) 2023-2024 Gissio
 *
 * License: MIT
 */

#if !defined(MCURENDERER_ST7789_H)
#define MCURENDERER_ST7789_H

#include "mcu-renderer.h"

#ifdef __cplusplus
extern "C" {
#endif

// ST7789 level 1 commands

#define MR_ST7789_NOP 0x00
#define MR_ST7789_SWRESET 0x01
#define MR_ST7789_RDDID 0x04
#define MR_ST7789_RDDST 0x09
#define MR_ST7789_RDDPM 0x0a
#define MR_ST7789_RDDMADCTL 0x0b
#define MR_ST7789_RDDCOLMOD 0x0c
#define MR_ST7789_RDDDIM 0x0d
#define MR_ST7789_RDDDSP 0x0e
#define MR_ST7789_RDDSDR 0x0f
#define MR_ST7789_SLPIN 0x10
#define MR_ST7789_SLPOUT 0x11
#define MR_ST7789_PTLON 0x12
#define MR_ST7789_NORON 0x13
#define MR_ST7789_INVOFF 0x20
#define MR_ST7789_INVON 0x21
#define MR_ST7789_GAMSET 0x26
#define MR_ST7789_DISPOFF 0x28
#define MR_ST7789_DISPON 0x29
#define MR_ST7789_CASET 0x2a
#define MR_ST7789_RASET 0x2b
#define MR_ST7789_RAMWR 0x2c
#define MR_ST7789_RAMRD 0x2e
#define MR_ST7789_PTLAR 0x30
#define MR_ST7789_VSCRDEF 0x33
#define MR_ST7789_TEOFF 0x34
#define MR_ST7789_TEON 0x35
#define MR_ST7789_MADCTL 0x36
#define MR_ST7789_MADCTL_MY (1 << 7)
#define MR_ST7789_MADCTL_MX (1 << 6)
#define MR_ST7789_MADCTL_MV (1 << 5)
#define MR_ST7789_MADCTL_BGR (1 << 3)
#define MR_ST7789_VSCRSADD 0x37
#define MR_ST7789_IDMOFF 0x38
#define MR_ST7789_IDMON 0x39
#define MR_ST7789_COLMOD 0x3a
#define MR_ST7789_COLMOD_16BPP ((0b101 << 4) | (0b101 << 0))
#define MR_ST7789_RAMWRC 0x3c
#define MR_ST7789_RAMRDC 0x3e
#define MR_ST7789_TESCAN 0x44
#define MR_ST7789_RDTESCAN 0x45
#define MR_ST7789_WRDISBV 0x51
#define MR_ST7789_RDDISBV 0x52
#define MR_ST7789_WRCTRLD 0x53
#define MR_ST7789_RDCTRLD 0x54
#define MR_ST7789_WRCACE 0x55
#define MR_ST7789_RDCABC 0x56
#define MR_ST7789_WRCABCMB 0x5e
#define MR_ST7789_RDCABCMB 0x5f
#define MR_ST7789_RDABCSDR 0x68
#define MR_ST7789_RDID1 0xda
#define MR_ST7789_RDID2 0xdb
#define MR_ST7789_RDID3 0xde

// ST7789 level 2 commands

#define MR_ST7789_RAMCTRL 0xb0
#define MR_ST7789_RGBCTRL 0xb1
#define MR_ST7789_PORCTRL 0xb2
#define MR_ST7789_FRCTRL1 0xb3
#define MR_ST7789_PARCTRL 0xb5
#define MR_ST7789_GCTRL 0xb7
#define MR_ST7789_GTADJ 0xb8
#define MR_ST7789_DGMEN 0xba
#define MR_ST7789_VCOMS 0xbb
#define MR_ST7789_LCMCTRL 0xc0
#define MR_ST7789_IDSET 0xc1
#define MR_ST7789_VDVVRHEN 0xc2
#define MR_ST7789_VRHS 0xc3
#define MR_ST7789_VDVS 0xc4
#define MR_ST7789_VCMOFSET 0xc5
#define MR_ST7789_FRCTRL2 0xc6
#define MR_ST7789_CABCCTRL 0xc7
#define MR_ST7789_REGSEL1 0xc8
#define MR_ST7789_REGSEL2 0xca
#define MR_ST7789_PWMFRSEL 0xcc
#define MR_ST7789_PWCTRL1 0xd0
#define MR_ST7789_VAPVANEN 0xd2
#define MR_ST7789_CMD2EN 0xdf
#define MR_ST7789_PVGAMCTRL 0xe0
#define MR_ST7789_NVGAMCTRL 0xe1
#define MR_ST7789_DGMLUTR 0xe2
#define MR_ST7789_DGMLUTB 0xe3
#define MR_ST7789_GATECTRL 0xe4
#define MR_ST7789_SPI2EN 0xe7
#define MR_ST7789_PWCTRL2 0xe8
#define MR_ST7789_EQCTRL 0xe9
#define MR_ST7789_PROMCTRL 0xec
#define MR_ST7789_PROMEN 0xfa
#define MR_ST7789_NVMSET 0xfc
#define MR_ST7789_PROMACT 0xfe

/**
 * Initializes an mcu-renderer ST7789 instance.
 *
 * @param mr The mcu-renderer instance.
 * @param width The physical display width (e.g. 240).
 * @param height The physical display height (e.g. 320).
 * @param rotation The desired display rotation.
 * @param textbuffer A user-provided buffer for rendering text.
 * @param textbuffer_size  The size of the user-provided buffer.
 * @param sleep_callback A user-provided sleep callback.
 * @param set_reset_callback A user-provided set reset callback.
 * @param set_command_callback A user-provided set command callback.
 * @param send_callback A user-provided send 8-bit data callback.
 * @param send16_callback A user-provided send 16-bit data callback.
 */
void mr_st7789_init(mr_t *mr,
                    int16_t width,
                    int16_t height,
                    mr_display_rotation_t rotation,
                    uint8_t *textbuffer,
                    uint32_t textbuffer_size,
                    mr_sleep_callback_t sleep_callback,
                    mr_set_reset_callback_t set_reset_callback,
                    mr_set_command_callback_t set_command_callback,
                    mr_send_callback_t send_callback,
                    mr_send_callback_t send16_callback);

/**
 * Enables/disables the ST7789 display. Takes 120 ms to finish.
 *
 * @param mr The mcu-renderer instance.
 * @param value Display enable.
 */
void mr_st7789_set_display(mr_t *mr,
                           bool value);

/**
 * Enables/disables ST7789 sleep mode.
 *
 * @param mr The mcu-renderer instance.
 * @param value Display sleep.
 */
void mr_st7789_set_sleep(mr_t *mr,
                           bool value);

#ifdef __cplusplus
}
#endif

#endif
