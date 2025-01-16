/*
 * MCU renderer
 * SDL driver
 *
 * (C) 2023-2025 Gissio
 *
 * License: MIT
 */

#if !defined(MCURENDERER_SDL_H)
#define MCURENDERER_SDL_H

#include "mcu-renderer.h"

#ifdef __cplusplus
extern "C" {
#endif

enum mr_sdl_display_type_t
{
    MR_SDL_DISPLAY_TYPE_COLOR,
    MR_SDL_DISPLAY_TYPE_MONOCHROME,
};

/**
 * Initializes an mcu-renderer SDL instance.
 *
 * @param mr The mcu-renderer instance.
 * @param width The desired display width.
 * @param height The desired display height.
 * @param display_type The desired display type.
 * @param upscale The upscaling factor of the SDL window.
 * @param title The SDL window title.
 */
void mr_sdl_init(mr_t *mr,
                 uint32_t width,
                 uint32_t height,
                 enum mr_sdl_display_type_t display_type,
                 uint32_t upscale,
                 const char *title);

/**
 * Frees an mcu-renderer SDL instance.
 *
 * @param mr The mcu-renderer instance.
 */
void mr_sdl_free(mr_t *mr);

/**
 * Sets the SDL window title.
 *
 * @param mr The mcu-renderer instance.
 * @param title The SDL window title.
 */
void mr_sdl_set_title(mr_t *mr,
                      const char *title);

/**
 * Enables/disables the SDL display.
 *
 * @param mr The mcu-renderer instance.
 * @param value Display enable.
 */
void mr_sdl_set_display(mr_t *mr,
                        bool value);

/**
 * Sets the backlight of an mcu-renderer SDL instance.
 *
 * @param mr The mcu-renderer instance.
 * @param value The backlight value (0 to 255).
 */
void mr_sdl_set_backlight(mr_t *mr,
                          uint8_t value);

/**
 * Refreshes the SDL display.
 *
 * @param mr The mcu-renderer instance.
 */
void mr_sdl_refresh_display(mr_t *mr);

#ifdef __cplusplus
}
#endif

#endif
