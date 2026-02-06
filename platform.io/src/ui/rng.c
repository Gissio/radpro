/*
 * Rad Pro
 * UI RNG
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../ui/draw.h"
#include "../ui/rng.h"
#include "../ui/system.h"

#define RNG_STRING_LEFT CONTENT_LEFT
#define RNG_STRING_TOP CONTENT_TOP
#define RNG_STRING_WIDTH CONTENT_WIDTH
#define RNG_STRING_HEIGHT ((CONTENT_HEIGHT + FONT_SMALL_LINE_HEIGHT) / 2)
#define RNG_STRING_OFFSETX PADDING_LEFT
#define RNG_STRING_OFFSETY (RNG_STRING_HEIGHT - FONT_SMALL_LINE_HEIGHT)

#define RNG_STATE_LEFT CONTENT_LEFT
#define RNG_STATE_TOP (RNG_STRING_TOP + RNG_STRING_HEIGHT)
#define RNG_STATE_WIDTH CONTENT_WIDTH
#define RNG_STATE_HEIGHT (CONTENT_BOTTOM - RNG_STATE_TOP)
#define RNG_STATE_OFFSETX (CONTENT_WIDTH / 2)
#define RNG_STATE_OFFSETY ((RNG_STATE_HEIGHT - FONT_MEDIUM_LINE_HEIGHT) / 2)

void drawRNG(const char *title, bool isLarge, const char *rngString, const char *infoString)
{
    drawTitleBar(title);

    setStrokeColor(COLOR_INSTRUMENT_ENHANCED_SECONDARY);

    if (!isLarge)
    {
        // RNG string
        setFont(font_small);
        drawText(rngString,
                 &(mr_rectangle_t){RNG_STRING_LEFT, RNG_STRING_TOP, RNG_STRING_WIDTH, RNG_STRING_HEIGHT},
                 &(mr_point_t){RNG_STRING_OFFSETX, RNG_STRING_OFFSETY});

        // State
        setFont(font_medium);
        setStrokeColor(COLOR_ELEMENT_ACTIVE);
        drawCenteredText(infoString,
                         &(mr_rectangle_t){RNG_STATE_LEFT, RNG_STATE_TOP, RNG_STATE_WIDTH, RNG_STATE_HEIGHT},
                         &(mr_point_t){RNG_STATE_OFFSETX, RNG_STATE_OFFSETY});
    }
    else
    {
        // State
        const mr_point_t rngNumberOffset = {
            (CONTENT_WIDTH / 2),
            ((CONTENT_HEIGHT - FONT_LARGE_LINE_HEIGHT) / 2),
        };
        setFont(font_large);
        drawCenteredText(rngString, &contentRectangle, &rngNumberOffset);
    }
}
