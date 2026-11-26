/*
 * Rad Pro
 * UI RNG
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include "../ui/draw.h"
#include "../ui/rng.h"
#include "../ui/system.h"

#define RNG_STRING_LEFT CONTENT_LEFT
#define RNG_STRING_TOP CONTENT_TOP
#define RNG_STRING_WIDTH CONTENT_WIDTH
#define RNG_STRING_HEIGHT ((CONTENT_HEIGHT + FONT_SMALL_LINE_HEIGHT) / 2)
#define RNG_STRING_OFFSETX PADDING_LEFT
#define RNG_STRING_OFFSETY (RNG_STRING_HEIGHT - FONT_SMALL_LINE_HEIGHT)
#define RNG_STRING_TEXTWIDTH (RNG_STRING_WIDTH - RNG_STRING_OFFSETX)

#define RNG_STATE_LEFT CONTENT_LEFT
#define RNG_STATE_TOP (RNG_STRING_TOP + RNG_STRING_HEIGHT)
#define RNG_STATE_WIDTH CONTENT_WIDTH
#define RNG_STATE_HEIGHT (CONTENT_BOTTOM - RNG_STATE_TOP)
#define RNG_STATE_OFFSETX (CONTENT_WIDTH / 2)
#define RNG_STATE_OFFSETY ((RNG_STATE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)

// Large number: center the digits (cap height), not the font line
#define RNG_NUMBER_OFFSETX (CONTENT_WIDTH / 2)
#define RNG_NUMBER_OFFSETY (-(FONT_LARGE_ASCENT - FONT_LARGE_CAP_HEIGHT) + (CONTENT_HEIGHT - FONT_LARGE_CAP_HEIGHT) / 2)

void drawRNG(const char *title, bool isLarge, const char *rngString, const char *stateString)
{
    drawTitleBar(title);

    setStrokeColor(COLOR_INSTRUMENT_ENHANCED_SECONDARY);

    if (!isLarge)
    {
        // RNG string
        setFont(font_small);

#if defined(DISPLAY_COLOR)
        // Proportional fonts: a full-length string may not fit on one line,
        // so the longest prefix that fits moves to a line above. The string
        // is ASCII (so it may be split at any byte), at most 16 characters.
        char firstLine[32];
        strcpy(firstLine, rngString);

        uint32_t length = strlen(firstLine);
        while (getTextWidth(firstLine) > RNG_STRING_TEXTWIDTH)
            firstLine[--length] = '\0';

        // Everything fits: the string goes on the second line
        const char *secondLine = rngString + length;
        if (!*secondLine)
        {
            secondLine = rngString;
            firstLine[0] = '\0';
        }

        static const mr_rectangle_t firstLineRectangle = {RNG_STRING_LEFT, RNG_STRING_TOP, RNG_STRING_WIDTH, RNG_STRING_HEIGHT - FONT_SMALL_LINE_HEIGHT};
        static const mr_point_t firstLineOffset = {RNG_STRING_OFFSETX, RNG_STRING_OFFSETY - FONT_SMALL_LINE_HEIGHT};
        static const mr_rectangle_t secondLineRectangle = {RNG_STRING_LEFT, RNG_STRING_TOP + RNG_STRING_OFFSETY, RNG_STRING_WIDTH, FONT_SMALL_LINE_HEIGHT};
        static const mr_point_t secondLineOffset = {RNG_STRING_OFFSETX, 0};
        drawText(firstLine, &firstLineRectangle, &firstLineOffset);
        drawText(secondLine, &secondLineRectangle, &secondLineOffset);
#else
        // Monochrome fonts: 16 characters always fit
        static const mr_rectangle_t stringRectangle = {RNG_STRING_LEFT, RNG_STRING_TOP, RNG_STRING_WIDTH, RNG_STRING_HEIGHT};
        static const mr_point_t stringOffset = {RNG_STRING_OFFSETX, RNG_STRING_OFFSETY};
        drawText(rngString, &stringRectangle, &stringOffset);
#endif

        // State
        static const mr_rectangle_t stateRectangle = {RNG_STATE_LEFT, RNG_STATE_TOP, RNG_STATE_WIDTH, RNG_STATE_HEIGHT};
        static const mr_point_t stateOffset = {RNG_STATE_OFFSETX, RNG_STATE_OFFSETY};
        setStrokeColor(COLOR_ELEMENT_ACTIVE);
        drawCenteredText(stateString, &stateRectangle, &stateOffset);
    }
    else
    {
        // Number
        static const mr_point_t numberOffset = {RNG_NUMBER_OFFSETX, RNG_NUMBER_OFFSETY};
        setFont(font_large);
        drawCenteredText(rngString, &contentRectangle, &numberOffset);
    }
}
