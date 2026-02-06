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

    // Container background [global background]
    {mr_get_color(0xf7f7f7),
     mr_get_color(0x1f1f1f),
     mr_get_color(0x000000)},

    // Container global [title bar background]
    {mr_get_color(0xfcfcfc),
     mr_get_color(0x2e2e2e),
     mr_get_color(0x000000)},

    // Container global shadow
    {mr_get_color(0xc9c9c9),
     mr_get_color(0x161616),
     mr_get_color(0x000000)},

    // Amplified enabled background [menu item selected background]
    {mr_get_color(0xcadefc),
     mr_get_color(0x2b3849),
     mr_get_color(0x09140f)},

    // On flat active [menu item selected foreground]
    {mr_get_color(0x1f1f1f),
     mr_get_color(0xe9e9e9),
     mr_get_color(0xeaa75e)},

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
     mr_get_color(0x352618)},

    // Game square white
    {mr_get_color(0xd0b088),
     mr_get_color(0xd0b088),
     mr_get_color(0x48341f)},

    // Game piece black
    {mr_get_color(0x000000),
     mr_get_color(0x000000),
     mr_get_color(0x000000)},

    // Game piece white
    {mr_get_color(0xffffff),
     mr_get_color(0xffffff),
     mr_get_color(0xc78842)},
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

mr_color_t getFillColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_MONOCHROME)
    if ((colorIndex == COLOR_AMPLIFIED_ENABLED_BACKGROUND) ||
        (colorIndex == COLOR_INSTRUMENT_FRAME_TERTIARY) ||
        (colorIndex == COLOR_INSTRUMENT_TICK_MARK_LABEL_SECONDARY) ||
        (colorIndex == COLOR_INSTRUMENT_ENHANCED_SECONDARY) ||
        (colorIndex == COLOR_INSTRUMENT_ENHANCED_TERTIARY) ||
        (colorIndex == COLOR_GAME_SQUARE_BLACK))
        return 0xffff;
    else
        return 0x0000;
#elif defined(DISPLAY_COLOR)
    return displayColors[colorIndex][settings.displayTheme];
#endif
}

void setFillColor(ColorIndex colorIndex)
{
#if defined(DISPLAY_MONOCHROME)
    mr_color_t fillColor = getFillColor(colorIndex);
    mr_color_t strokeColor = fillColor ? 0x0000 : 0xffff;

    mr_set_fill_color(&mr, fillColor);
    mr_set_stroke_color(&mr, strokeColor);
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
    mr_rectangle_t r;
    r.width = 1;
    r.height = 1;

    setFillColor(COLOR_CONTAINER_GLOBAL);

    for (r.y = rectangle->y;
         r.y < (rectangle->y + rectangle->height);
         r.y++)
    {
        for (r.x = rectangle->x;
             r.x < (rectangle->x + rectangle->width);
             r.x++)
        {
            if (dark)
            {
                if ((r.x ^ r.y) & 0b1)
                    continue;
            }
            else
            {
                if (((r.x & r.y) & 0b1) == 0)
                    continue;
            }

            drawRectangle(&r);
        }
    }
}
#endif

void drawImage(const mr_rectangle_t *rectangle, const mr_color_t *imageBuffer)
{
    mr_draw_image(&mr, rectangle, imageBuffer);
}

void setFont(const uint8_t *font)
{
    mr_set_font(&mr, font);
}

uint16_t getTextWidth(const char *s)
{
    return mr_get_utf8_text_width(&mr, (const uint8_t *)s);
}

uint16_t getTextHeight(void)
{
    return mr_get_line_height(&mr);
}

typedef enum
{
    TEXTALIGNMENT_LEFTALIGNED,
    TEXTALIGNMENT_CENTERED,
    TEXTALIGNMENT_RIGHTALIGNED,
} TextAlignment;

static void drawAlignedText(const char *s, const mr_rectangle_t *rectangle, const mr_point_t *offset, TextAlignment textAlignment)
{
    char buffer[128];
    const char *str;
    int16_t width;

    switch (textAlignment)
    {
    case TEXTALIGNMENT_LEFTALIGNED:
        // | x-------->|
        width = rectangle->width - offset->x;

        break;

    case TEXTALIGNMENT_CENTERED:
        // |<----x---->|
        width = rectangle->width;

        break;

    case TEXTALIGNMENT_RIGHTALIGNED:
        // |<--------x |
        width = offset->x;

        break;

    default:
        width = 0;

        break;
    }

    if (getTextWidth(s) <= width)
        str = s;
    else
    {
        strcpy(buffer, s);
        for (uint32_t i = strlen(s); i > 0; i--)
        {
            // Skip UTF-8 continuation bytes
            if ((((uint8_t)buffer[i]) >> 6) == 0x02)
                continue;

            strcpy(buffer + i, getString(STRING_ELLIPSIS));
            if (getTextWidth(buffer) <= width)
                break;
        }
        str = buffer;
    }

    mr_point_t strOffset = *offset;
    switch (textAlignment)
    {
    case TEXTALIGNMENT_CENTERED:
        strOffset.x -= getTextWidth(str) / 2;

        break;

    case TEXTALIGNMENT_RIGHTALIGNED:
        strOffset.x -= getTextWidth(str);

        break;

    default:
        break;
    }

    mr_draw_utf8_text(&mr, (const uint8_t *)str, rectangle, &strOffset);
}

void drawText(const char *s, const mr_rectangle_t *rectangle, const mr_point_t *offset)
{
    drawAlignedText(s, rectangle, offset, TEXTALIGNMENT_LEFTALIGNED);
}

void drawCenteredText(const char *s, const mr_rectangle_t *rectangle, const mr_point_t *offset)
{
    drawAlignedText(s, rectangle, offset, TEXTALIGNMENT_CENTERED);
}

void drawRightAlignedText(const char *s, const mr_rectangle_t *rectangle, const mr_point_t *offset)
{
    drawAlignedText(s, rectangle, offset, TEXTALIGNMENT_RIGHTALIGNED);
}

void drawCenteredMultilineText(const mr_rectangle_t *rectangle, const char *message)
{
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);

    const uint8_t *lineStarts[CONTENT_MULTILINE_MAX];
    uint32_t lineLengths[CONTENT_MULTILINE_MAX];
    uint32_t lineNum = 0;

    const uint8_t *messagePosition = (const uint8_t *)message;
    const uint8_t *messageLine = (const uint8_t *)message;

    char buffer[128];

    while (true)
    {
        // Find next word
        const uint8_t *messageWord = messagePosition;

        if (*messagePosition < ' ')
            messagePosition++;
        else
        {
            if (*messagePosition == ' ')
                messagePosition++;
            while (true)
            {
                uint8_t c = *messagePosition;
                if (c <= ' ')
                    break;
                messagePosition++;
            };
        }

        // Word
        if (*messageWord >= ' ')
        {
            uint32_t n = messagePosition - messageLine;
            memcpy(buffer, messageLine, n);
            buffer[n] = '\0';

            if (getTextWidth(buffer) > (DISPLAY_WIDTH - PADDING_LEFT - PADDING_RIGHT))
            {
                lineStarts[lineNum] = messageLine;
                lineLengths[lineNum++] = messageWord - messageLine;

                messageLine = messageWord + (*messageWord == ' ');
            }
        }
        // End of line
        else if ((*messageWord == '\n') ||
                 (*messageWord == '\0'))
        {
            lineStarts[lineNum] = messageLine;
            lineLengths[lineNum++] = messageWord - messageLine;

            messageLine = messagePosition;

            if (*messageWord == '\0')
                break;
        }
    }

    mr_rectangle_t lineRectangle;
    mr_point_t lineOffset;

    uint32_t linesTop = (rectangle->height - lineNum * FONT_SMALL_LINE_HEIGHT) / 2;

    uint32_t y = rectangle->y;

    for (uint32_t i = 0; i < lineNum; i++)
    {
        lineRectangle = (mr_rectangle_t){rectangle->x, y, rectangle->width, FONT_SMALL_LINE_HEIGHT};
        lineOffset = (mr_point_t){rectangle->width / 2, 0};

        if (i == 0)
        {
            lineRectangle.height += linesTop;
            lineOffset.y += linesTop;
            y += linesTop;
        }
        else if (i == (lineNum - 1))
            lineRectangle.height = rectangle->y + rectangle->height - y;

        uint32_t lineLength = lineLengths[i];

        memcpy(buffer, lineStarts[i], lineLength);
        buffer[lineLength] = '\0';
        if (lineLength == 0)
            setStrokeColor(COLOR_ELEMENT_NEUTRAL);
        drawCenteredText(buffer, &lineRectangle, &lineOffset);

        y += FONT_SMALL_LINE_HEIGHT;
    }
}

void drawRowLeft(const char *buffer, mr_rectangle_t *rectangle)
{
    rectangle->width = rectangle->x - CONTENT_LEFT;
    rectangle->x = CONTENT_LEFT;
    mr_point_t offset = {PADDING_LEFT, (rectangle->height - getTextHeight()) / 2};
    drawText(buffer, rectangle, &offset);
}

void drawRowRight(const char *buffer, mr_rectangle_t *rectangle)
{
    uint32_t width = getTextWidth(buffer) + PADDING_RIGHT;
    rectangle->width = width;
    rectangle->x -= width;
    mr_point_t offset = {0, (rectangle->height - getTextHeight()) / 2};
    drawText(buffer, rectangle, &offset);
}

static const uint8_t chargingIcons[] = {5, 6, 6, 6, 7};

void setupBatteryIcon(char *buffer)
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

    strclr(buffer);
    strcatChar(buffer, '0' + level);
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
