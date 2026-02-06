/*
 * MCU renderer
 * SDL driver
 *
 * (C) 2023-2025 Gissio
 *
 * License: MIT
 */

#if defined(MCURENDERER_SDL)

#include <stdio.h>
#include <stdlib.h>

#include <SDL.h>

#include "mcu-renderer-sdl.h"

typedef struct
{
    enum mr_sdl_display_type_t display_type;

    bool enabled;

    uint8_t backlight;

    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    SDL_Texture *sdl_texture;
    SDL_Color *sdl_buffer;
} mr_sdl_display_t;

static SDL_Color mr_sdl_monochrome_palette[][2] = {
    {
        {57, 61, 46, 255},
        {24, 27, 18, 255},
    },
    {
        {154, 171, 237, 255},
        {37, 35, 146, 255},
    },
};

void mr_sdl_init(mr_t *mr,
                 uint32_t width,
                 uint32_t height,
                 enum mr_sdl_display_type_t display_type,
                 uint32_t upscale,
                 const char *title)
{
    mr_init(mr);

    mr->draw_rectangle_callback = mr_draw_rectangle_framebuffer_color;
    mr->draw_string_callback = mr_draw_string_framebuffer_color;
#if defined(MCURENDERER_BITMAP_SUPPORT)
    mr->draw_bitmap_callback = mr_draw_bitmap_framebuffer_color;
#endif
#if defined(MCURENDERER_IMAGE_SUPPORT)
    mr->draw_image_callback = mr_draw_image_framebuffer_color;
#endif

    mr_sdl_display_t *display = calloc(sizeof(mr_sdl_display_t), 1);
    mr->display = display;

    mr->display_width = width;
    mr->display_height = height;

    mr->buffer = calloc(sizeof(mr_color_t) * width * height, 1);

    display->display_type = display_type;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Unable to initialize SDL:  %s\n", SDL_GetError());

        exit(1);
    }

    atexit(SDL_Quit);

    display->sdl_window = SDL_CreateWindow(title,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           width * upscale,
                                           height * upscale,
                                           0);
    if (!display->sdl_window)
    {
        printf("Could not create SDL window: %s\n", SDL_GetError());

        exit(1);
    }

    display->sdl_renderer = SDL_CreateRenderer(display->sdl_window,
                                               -1,
                                               SDL_RENDERER_PRESENTVSYNC);
    if (!display->sdl_renderer)
    {
        printf("Could not create SDL renderer: %s\n", SDL_GetError());

        exit(1);
    }

#if !defined(__EMSCRIPTEN__)
    SDL_RenderSetLogicalSize(display->sdl_renderer,
                             width * upscale,
                             height * upscale);
#endif

    display->sdl_texture = SDL_CreateTexture(display->sdl_renderer,
                                             SDL_PIXELFORMAT_ABGR8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width,
                                             height);
    if (!display->sdl_texture)
    {
        printf("Could not create SDL texture: %s\n", SDL_GetError());

        exit(1);
    }

    display->sdl_buffer = malloc(sizeof(SDL_Color) * width * height);
}

void mr_sdl_free(mr_t *mr)
{
    mr_sdl_display_t *display = (mr_sdl_display_t *)mr->display;

    free(display->sdl_buffer);

    SDL_DestroyTexture(display->sdl_texture);
    SDL_DestroyRenderer(display->sdl_renderer);
    SDL_DestroyWindow(display->sdl_window);

    SDL_Quit();

    free(display);

    free(mr->buffer);
}

void mr_sdl_set_title(mr_t *mr,
                      const char *title)
{
    mr_sdl_display_t *display = (mr_sdl_display_t *)mr->display;
    SDL_SetWindowTitle(display->sdl_window, title);
}

void mr_sdl_set_display(mr_t *mr,
                        bool value)
{
    mr_sdl_display_t *display = (mr_sdl_display_t *)mr->display;

    display->enabled = value;
}

void mr_sdl_set_backlight(mr_t *mr,
                          uint8_t value)
{
    mr_sdl_display_t *display = (mr_sdl_display_t *)mr->display;

    display->backlight = value;
}

static SDL_Color mr_sdl_blend_colors(SDL_Color foreground,
                                     SDL_Color background,
                                     int32_t alpha)
{
    return (SDL_Color){
        background.r + ((foreground.r - background.r) * (alpha + 1)) / 256,
        background.g + ((foreground.g - background.g) * (alpha + 1)) / 256,
        background.b + ((foreground.b - background.b) * (alpha + 1)) / 256,
        0xff};
}

static SDL_Color mr_sdl_get_color(mr_sdl_display_t *display,
                                  mr_color_t color)
{
    if (display->display_type == MR_SDL_DISPLAY_TYPE_COLOR)
    {
        uint32_t r5 = (color >> 11) & 0x1f;
        uint32_t g6 = (color >> 5) & 0x3f;
        uint32_t b5 = (color >> 0) & 0x1f;

        return mr_sdl_blend_colors(
            (SDL_Color){
#if defined(MCURENDERER_SDL_R0B0_ZERO)
                ((r5 << 3) | (r5 >> 2)) & 0b11111011,
                ((g6 << 2) | (g6 >> 4)) & 0b11111101,
                ((b5 << 3) | (b5 >> 2)) & 0b11111011,
#elif defined(MCURENDERER_SDL_R0B0_ONE)
                ((r5 << 3) | (r5 >> 2)) | 0b00000100,
                ((g6 << 2) | (g6 >> 4)) | 0b00000010,
                ((b5 << 3) | (b5 >> 2)) | 0b00000100,
#elif defined(MCURENDERER_SDL_R0B0_FROM_R5_B5)
                (r5 << 3) | (r5 >> 2),
                (g6 << 2) | (g6 >> 4),
                (b5 << 3) | (b5 >> 2),
#else // MCURENDERER_SDL_R0B0_FROM_G5
                (r5 << 3) | ((g6 & 0b1) << 2) | (r5 >> 3),
                (g6 << 2) | (g6 >> 4),
                (b5 << 3) | ((g6 & 0b1) << 2) | (b5 >> 3),
#endif
                0xff},
            (SDL_Color){
                0,
                0,
                0,
                0xff},
            display->backlight);
    }
    else
    {
        return mr_sdl_blend_colors(
            mr_sdl_monochrome_palette[1][color >> 15],
            mr_sdl_monochrome_palette[0][color >> 15],
            display->backlight);
    }
}

void mr_sdl_refresh_display(mr_t *mr)
{
    mr_sdl_display_t *display = (mr_sdl_display_t *)mr->display;

    mr_color_t *framebuffer = (mr_color_t *)mr->buffer;
    SDL_Color *dest = display->sdl_buffer;

    if (display->enabled)
    {
        for (uint32_t i = 0; i < mr->display_width * mr->display_height; i++)
            *dest++ = mr_sdl_get_color(display, *framebuffer++);
    }
    else
    {
        for (uint32_t i = 0; i < mr->display_width * mr->display_height; i++)
            *dest++ = mr_sdl_get_color(display, 0x0000);
    }

    SDL_UpdateTexture(display->sdl_texture,
                      NULL,
                      display->sdl_buffer,
                      mr->display_width * sizeof(SDL_Color));
    SDL_RenderCopy(display->sdl_renderer,
                   display->sdl_texture,
                   NULL,
                   NULL);
    SDL_RenderPresent(display->sdl_renderer);
}

#endif
