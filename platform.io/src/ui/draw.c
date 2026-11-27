/*
 * Rad Pro
 * UI draw
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include <mcu-renderer.h>

#include FONT_SYMBOLS
#include FONT_LARGE
#include FONT_MEDIUM
#include FONT_SMALL

#include "../system/cmath.h"
#include "../system/cstring.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/draw.h"

#if defined(DISPLAY_COLOR)

// Online color blender: https://pinetools.com/blend-colors
// Online RGB565 color picker: https://rgbcolorpicker.com/565

static const mr_color_t displayColors[][3] = {
    // Element active
    {mr_get_color(0x1f1f1f),
     mr_get_color(0xe9e9e9),
     mr_get_color(0xeaa75e)},

    // Element neutral
    {mr_get_color(0x535353),
     mr_get_color(0xb0b0b0),
     mr_get_color(0xc78842)},

    // Container global (title bar background)
    {mr_get_color(0xfcfcfc),
     mr_get_color(0x2e2e2e),
     mr_get_color(0x000000)},

    // Container global shadow
    {mr_get_color(0xc9c9c9),
     mr_get_color(0x161616),
     mr_get_color(0x000000)},

    // Container background (content background)
    {mr_get_color(0xf7f7f7),
     mr_get_color(0x1f1f1f),
     mr_get_color(0x000000)},

    // On flat active (menu item foreground)
    {mr_get_color(0x1f1f1f),
     mr_get_color(0xe9e9e9),
     mr_get_color(0xeaa75e)},

    // Amplified enabled background (selected menu item background)
    {mr_get_color(0xb2cff9),
     mr_get_color(0x3b4f69),
     mr_get_color(0x10211a)},

    // On amplified active (selected menu item foreground)
    {mr_get_color(0x1d3c67),
     mr_get_color(0xcae4ff),
     mr_get_color(0x58c8a2)},

    // Running (green)
    {mr_get_color(0x008300),
     mr_get_color(0x198100),
     mr_get_color(0x008800)},

    // Warning (orange)
    {mr_get_color(0xfe9413),
     mr_get_color(0xfa7d00),
     mr_get_color(0xe67800)},

    // Alarm (red)
    {mr_get_color(0xe30019),
     mr_get_color(0xdb0007),
     mr_get_color(0xe90f20)},

    // Instrument frame primary
    {mr_get_color(0xffffff),
     mr_get_color(0x363636),
     mr_get_color(0x000000)},

    // Instrument frame primary, warning zone: frame primary darkened in day
    // and dusk by two RGB565 red/blue steps (green: four, so it stays grey);
    // none at night, as frame primary is already black (a lighter zone reads
    // as the opposite of a darker one)
    {mr_get_color(0xececec),
     mr_get_color(0x242424),
     mr_get_color(0x000000)},

    // Instrument frame primary, alarm zone: twice the warning zone's steps
    {mr_get_color(0xdcdcdc),
     mr_get_color(0x141414),
     mr_get_color(0x000000)},

    // Instrument frame tertiary
    {mr_get_color(0xbebebe),
     mr_get_color(0x5e5e5e),
     mr_get_color(0x78532c)},

    // Instrument tick mark label secondary
    {mr_get_color(0x707070),
     mr_get_color(0x8c8c8c),
     mr_get_color(0x9c6a34)},

    // Instrument enhanced secondary
    {mr_get_color(0x2d548b),
     mr_get_color(0x84b3ef),
     mr_get_color(0x38a784)},

    // Instrument enhanced tertiary
    {mr_get_color(0x9cc1f5),
     mr_get_color(0x3d526d),
     mr_get_color(0x10211a)},

    // Instrument enhanced secondary warning
    {mr_get_color(0xfe9413),
     mr_get_color(0xfa7d00),
     mr_get_color(0xe67800)},

    // Instrument enhanced tertiary warning
    {mr_get_color(0xffd4af),
     mr_get_color(0x7a3d00),
     mr_get_color(0x351400)},

    // Instrument enhanced secondary alarm
    {mr_get_color(0xe30019),
     mr_get_color(0xe1000e),
     mr_get_color(0xe90f20)},

    // Instrument enhanced tertiary alarm
    {mr_get_color(0xffbdb3),
     mr_get_color(0x7A020A),
     mr_get_color(0x460000)},

#if defined(GAME)
    // Selected enabled background [game square selected]
    {mr_get_color(0x2d548b),
     mr_get_color(0x4c6c9a),
     mr_get_color(0x1e3e32)},

    // Game square black
    {mr_get_color(0xb89070),
     mr_get_color(0xb89070),
     mr_get_color(0x48341f)},

    // Game square white
    {mr_get_color(0xd0b088),
     mr_get_color(0xd0b088),
     mr_get_color(0x78532c)},

    // Game piece black
    {mr_get_color(0x000000),
     mr_get_color(0x000000),
     mr_get_color(0x000000)},

    // Game piece white
    {mr_get_color(0xffffff),
     mr_get_color(0xffffff),
     mr_get_color(0xeaa75e)},
#endif
};

#endif

// Definitions

const mr_rectangle_t contentRectangle = {
    CONTENT_LEFT,
    CONTENT_TOP,
    CONTENT_WIDTH,
    CONTENT_HEIGHT,
};

mr_t mr;

// Low-level functions

#if defined(DISPLAY_MONOCHROME)
#define MONOCHROME_SET_COLORS ((1 << COLOR_AMPLIFIED_ENABLED_BACKGROUND) |         \
                               (1 << COLOR_INSTRUMENT_FRAME_TERTIARY) |            \
                               (1 << COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY) | \
                               (1 << COLOR_INSTRUMENT_ENHANCED_SECONDARY) |        \
                               (1 << COLOR_INSTRUMENT_ENHANCED_TERTIARY) |         \
                               (1 << COLOR_GAME_SQUARE_BLACK))
#endif

NOINLINE mr_color_t getFillColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_MONOCHROME)
    // 0xffff for the colors in MONOCHROME_SET_COLORS, else 0x0000
    return -((MONOCHROME_SET_COLORS >> colorIndex) & 1);
#elif defined(DISPLAY_COLOR)
    return displayColors[colorIndex][settings.displayTheme];
#endif
}

void setFillColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_MONOCHROME)
    mr_color_t fillColor = getFillColor(colorIndex);

    mr_set_fill_color(&mr, fillColor);
    mr_set_stroke_color(&mr, ~fillColor);
#elif defined(DISPLAY_COLOR)
    mr_set_fill_color(&mr, getFillColor(colorIndex));
#endif
}

void setStrokeColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_COLOR)
    mr_set_stroke_color(&mr, getFillColor(colorIndex));
#endif
}

void drawRectangle(const mr_rectangle_t *rectangle)
{
    mr_draw_rectangle(&mr, rectangle);
}

#if defined(DISPLAY_MONOCHROME)
void ditherRectangle(const mr_rectangle_t *rectangle, bool dark)
{
    // Dark: checkerboard (x + y even). Light: odd rows, odd columns.
    mr_rectangle_t r = {0, 0, 1, 1};
    int32_t right = rectangle->x + rectangle->width;
    int32_t bottom = rectangle->y + rectangle->height;

    setFillColor(COLOR_CONTAINER_GLOBAL);

    for (r.y = rectangle->y; r.y < bottom; r.y++)
        if (dark || (r.y & 1))
            for (r.x = rectangle->x + ((rectangle->x ^ r.y) & 1); r.x < right; r.x += 2)
                drawRectangle(&r);
}
#endif

void drawImage(const mr_rectangle_t *rectangle, const mr_color_t *imageBuffer)
{
    mr_draw_image(&mr, rectangle, imageBuffer);
}

NOINLINE void fillImageBuffer(mr_color_t *imageBuffer, int32_t start, int32_t end, ColorIndex colorIndex)
{
    mr_color_t color = getFillColor(colorIndex);

    for (; start < end; start++)
        imageBuffer[start] = color;
}

void setFont(const uint8_t *font)
{
    mr_set_font(&mr, font);
}

uint16_t getTextWidth(const char *text)
{
    return mr_get_utf8_text_width(&mr, (const uint8_t *)text);
}

typedef enum
{
    TEXTALIGNMENT_LEFTALIGNED,
    TEXTALIGNMENT_CENTERED,
    TEXTALIGNMENT_RIGHTALIGNED,
} TextAlignment;

#define TEXT_BUFFER_SIZE 128

static void drawAlignedText(const char *text, const mr_rectangle_t *rectangle, const mr_point_t *offset, TextAlignment textAlignment)
{
    char buffer[TEXT_BUFFER_SIZE];

    // Available width:
    //   left aligned  | x-------->|
    //   centered      |<----x---->|
    //   right aligned |<--------x |
    int32_t width = rectangle->width;
    if (textAlignment == TEXTALIGNMENT_LEFTALIGNED)
        width -= offset->x;
    else if (textAlignment == TEXTALIGNMENT_RIGHTALIGNED)
        width = offset->x;

    const char *str = text;
    int32_t textWidth = getTextWidth(text);
    if (textWidth > width)
    {
        // Leave room for the ellipsis and the terminator
        const int32_t maxLength = sizeof(buffer) - 1 - strlen(getString(STRING_ELLIPSIS));
        int32_t length = strlen(text);
        if (length > maxLength)
            length = maxLength;

        memcpy(buffer, text, length);
        buffer[length] = '\0';

        // The full text does not fit, so start by dropping its last character
        for (int32_t i = length - 1; i > 0; i--)
        {
            // Skip UTF-8 continuation bytes
            if ((((uint8_t)buffer[i]) >> 6) == 0x02)
                continue;

            strcpy(buffer + i, getString(STRING_ELLIPSIS));
            textWidth = getTextWidth(buffer);
            if (textWidth <= width)
                break;
        }
        str = buffer;
    }

    // Shift left by 0, 1/2 or 1 text width (TextAlignment is 0, 1, 2)
    mr_point_t strOffset = {
        offset->x - ((textWidth * textAlignment) >> 1),
        offset->y,
    };
    mr_draw_utf8_text(&mr, (const uint8_t *)str, rectangle, &strOffset);
}

void drawText(const char *text, const mr_rectangle_t *rectangle, const mr_point_t *offset)
{
    drawAlignedText(text, rectangle, offset, TEXTALIGNMENT_LEFTALIGNED);
}

void drawCenteredText(const char *text, const mr_rectangle_t *rectangle, const mr_point_t *offset)
{
    drawAlignedText(text, rectangle, offset, TEXTALIGNMENT_CENTERED);
}

void drawRightAlignedText(const char *text, const mr_rectangle_t *rectangle, const mr_point_t *offset)
{
    drawAlignedText(text, rectangle, offset, TEXTALIGNMENT_RIGHTALIGNED);
}

static const char *getWrappedTextLine(const char *src, char *line)
{
    bool firstWord = true;
    char *dest = line;

    while (true)
    {
        uint8_t c = (uint8_t)*src;

        if (c == '\0')
            break;

        if (c == '\n')
        {
            src++;
            break;
        }

        if (c <= ' ')
        {
            src++;
            continue;
        }

        // Word
        const char *srcWord = src;
        char *destWord = dest;

        while (*(const uint8_t *)srcWord > ' ')
            srcWord++;

        size_t wordLength = srcWord - src;

        // Keep the line within its buffer: move a later word (which also
        // needs its separating space) to the next line, cut a first word
        // that is longer than the whole buffer
        size_t available = (line + TEXT_BUFFER_SIZE - 1) - dest;
        if (!firstWord)
        {
            if (wordLength >= available)
                break;

            *destWord++ = ' ';
        }
        else if (wordLength > available)
            wordLength = available;

        memcpy(destWord, src, wordLength);
        destWord += wordLength;
        *destWord = '\0';

        if (!firstWord && getTextWidth(line) > CONTENT_MULTILINE_WIDTH)
            break;

        src = srcWord;
        dest = destWord;
        firstWord = false;
    }

    dest[0] = '\0';

    return src;
}

void drawCenteredMultilineText(const mr_rectangle_t *rectangle, const char *text)
{
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);

    char line[TEXT_BUFFER_SIZE];

    // Count the lines, stopping when the next one would not fit
    const char *src = text;
    int32_t lineNum = 0;
    do
    {
        src = getWrappedTextLine(src, line);
        lineNum++;
    } while ((src[0] != '\0') &&
             (lineNum < CONTENT_MULTILINE_MAX) &&
             ((lineNum + 1) * FONT_SMALL_LINE_HEIGHT <= rectangle->height));

    // Each line's rectangle reaches down to the next line, so the first and
    // last lines also clear the margins above and below the text
    int32_t bottom = rectangle->y + rectangle->height;
    int32_t lineTop = rectangle->y;
    int32_t textTop = lineTop + (rectangle->height - lineNum * FONT_SMALL_LINE_HEIGHT) / 2;

    src = text;
    for (int32_t i = 0; i < lineNum; i++)
    {
        src = getWrappedTextLine(src, line);

        int32_t lineBottom = (i == (lineNum - 1)) ? bottom : textTop + FONT_SMALL_LINE_HEIGHT;
        mr_rectangle_t lineRectangle = {rectangle->x, lineTop, rectangle->width, lineBottom - lineTop};
        mr_point_t lineOffset = {rectangle->width / 2, textTop - lineTop};

        // Lines after an empty line are secondary
        if (line[0] == '\0')
            setStrokeColor(COLOR_ELEMENT_NEUTRAL);

        drawCenteredText(line, &lineRectangle, &lineOffset);

        lineTop = lineBottom;
        textTop += FONT_SMALL_LINE_HEIGHT;
    }
}

// Draws text vertically centered in a row
static NOINLINE void drawRow(const char *text, const mr_rectangle_t *rectangle, int16_t offsetX)
{
    mr_point_t offset = {offsetX, (rectangle->height - mr_get_line_height(&mr)) / 2};
    drawText(text, rectangle, &offset);
}

void drawRowLeft(const char *text, mr_rectangle_t *rectangle)
{
    rectangle->width = rectangle->x - CONTENT_LEFT;
    rectangle->x = CONTENT_LEFT;
    drawRow(text, rectangle, PADDING_LEFT);
}

void drawRowRight(const char *text, mr_rectangle_t *rectangle)
{
    int16_t width = getTextWidth(text) + PADDING_RIGHT;
    rectangle->width = width;
    rectangle->x -= width;
    drawRow(text, rectangle, 0);
}

static const uint8_t chargingIcons[] = {5, 6, 6, 6, 7};

void setupBatteryIcon(char *text)
{
    ColorIndex colorIndex;
    uint8_t level = getBatteryLevel();

    if (isBatteryCharging())
    {
        colorIndex = COLOR_RUNNING;
        level = chargingIcons[level];
    }
    else
    {
        if (isUSBPowered())
            colorIndex = COLOR_RUNNING;
        else if (level == 0)
            colorIndex = COLOR_ALARM;
        else
            colorIndex = COLOR_ELEMENT_NEUTRAL;
    }

    text[0] = '0' + level;
    text[1] = '\0';
    setFont(font_symbols);
    setStrokeColor(colorIndex);
}

void drawValueAndUnit(const char *valueString, const char *unitString, const mr_rectangle_t *rectangle, const mr_point_t *offset, bool isHold)
{
    mr_rectangle_t partialRectangle = *rectangle;
    partialRectangle.width = offset->x + getTextWidth(valueString);
    setStrokeColor(isHold ? COLOR_ELEMENT_ACTIVE : COLOR_INSTRUMENT_ENHANCED_SECONDARY);
    drawText(valueString, &partialRectangle, offset);

    partialRectangle.x += partialRectangle.width;
    partialRectangle.width = rectangle->width - partialRectangle.width;
    mr_point_t partialOffset = {0, offset->y};
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    drawText(unitString, &partialRectangle, &partialOffset);
}
