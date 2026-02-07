/*
 * Rad Pro
 * UI statistics
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/pulses.h"
#include "../system/cstring.h"
#include "../system/power.h"
#include "../system/system.h"
#include "../ui/draw.h"
#include "../ui/statistics.h"
#include "../ui/system.h"

#define STATISTICS_LINE_HEIGHT FONT_SMALL_LINE_HEIGHT
#define STATISTICS_LINES_HEIGHT (STATISTICS_LINE_NUM * STATISTICS_LINE_HEIGHT)
#define STATISTICS_TOPMARGIN_HEIGHT ((CONTENT_HEIGHT - STATISTICS_LINES_HEIGHT) / 2)
#define STATISTICS_BOTTOMMARGIN_HEIGHT (CONTENT_HEIGHT - STATISTICS_TOPMARGIN_HEIGHT - STATISTICS_LINES_HEIGHT)
#define STATISTICS_KEY_LEFT 0
#if defined(DISPLAY_128X64)
#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2 - FONT_SMALL_LINE_HEIGHT / 2)
#elif defined(DISPLAY_320X240)
#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2 - FONT_SMALL_LINE_HEIGHT / 2)
#elif defined(DISPLAY_240X320)
#if defined(DISPLAY_125PPI)
#define STATISTICS_KEY_WIDTH (CONTENT_WIDTH / 2 - FONT_SMALL_LINE_HEIGHT / 2)
#else
#define STATISTICS_KEY_WIDTH 72
#endif
#endif
#define STATISTICS_KEY_OFFSETX PADDING_LEFT
#define STATISTICS_VALUE_LEFT STATISTICS_KEY_WIDTH
#define STATISTICS_VALUE_WIDTH (CONTENT_WIDTH - STATISTICS_KEY_WIDTH)
#define STATISTICS_VALUE_OFFSETX (FONT_SMALL_LINE_HEIGHT / 2)

static cstring statisticsKeyStrings[] = {
#if defined(DISPLAY_LANDSCAPE)
    STRING_TUBE_LIFE,
    STRING_PULSES,
    STRING_DEAD_TIME,
    STRING_DEVICE_ID,
    STRING_EMPTY,
    STRING_BATTERY,
#elif defined(DISPLAY_PORTRAIT)
    STRING_TUBE,
    STRING_LIFE,
    STRING_PULSES,
    STRING_DEAD_TIME,
    STRING_EMPTY,
    STRING_DEVICE,
    STRING_ID,
    STRING_EMPTY,
    STRING_BATTERY,
#endif
};

enum
{
    STATISTICS_TITLE,
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
    STATISTICS_DEVICEID_1,
    STATISTICS_DEVICEID_2,
    STATISTICS_VOLTAGE,
};

static const uint8_t statisticsType[] = {
#if defined(DISPLAY_LANDSCAPE)
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
    STATISTICS_DEVICEID_1,
    STATISTICS_DEVICEID_2,
    STATISTICS_VOLTAGE,
#elif defined(DISPLAY_PORTRAIT)
    STATISTICS_TITLE,
    STATISTICS_TUBE_TIME,
    STATISTICS_TUBE_PULSES,
    STATISTICS_DEAD_TIME,
    STATISTICS_TITLE,
    STATISTICS_TITLE,
    STATISTICS_DEVICEID_1,
    STATISTICS_DEVICEID_2,
    STATISTICS_VOLTAGE,
#endif
};

#define STATISTICS_LINE_NUM sizeof(statisticsType)

void drawStatistics(void)
{
    drawTitleBar(getString(STRING_STATISTICS));

    setFont(font_small);

    char keyString[64];
    char valueString[32];
    char unitString[32];

    mr_rectangle_t rectangle;
    mr_point_t offset;

    uint32_t y = CONTENT_TOP;

    for (uint32_t i = 0; i < STATISTICS_LINE_NUM; i++)
    {
        strcpy(keyString, getString(statisticsKeyStrings[i]));
        strclr(valueString);
        strclr(unitString);

        switch (statisticsType[i])
        {
        case STATISTICS_TITLE:
            break;

        case STATISTICS_TUBE_TIME:
            strcatTime(valueString, getTubeTime());

            break;

        case STATISTICS_TUBE_PULSES:
            strcatUInt32(valueString, getTubePulseCount(), 0);

            break;

        case STATISTICS_DEAD_TIME:
        {
            float deadTime = getTubeDeadTime();
            if (deadTime >= 0.064F)
                strcpy(valueString, getString(STRING_NONE));
            else
            {
                strcpy(valueString, getString(STRING_LESSTHAN));
                strcatFloat(valueString, 1000000 * deadTime, 1);
                strcatChar(unitString, ' ');
                strcat(unitString, getString(STRING_MICROSECONDS));
            }

            break;
        }

        case STATISTICS_DEVICEID_1:
            getDeviceId(valueString);
            valueString[12] = '\0';

            break;

        case STATISTICS_DEVICEID_2:
            getDeviceId(valueString);
            strcpy(valueString, valueString + 12);

            break;

        case STATISTICS_VOLTAGE:
            strcatFloat(valueString, getBatteryVoltage(), 3);
            strclr(unitString);
            strcatChar(unitString, ' ');
            strcat(unitString, getString(STRING_VOLT_UNIT));

            break;
        }

        // Key
        rectangle = (mr_rectangle_t){
            STATISTICS_KEY_LEFT,
            y,
            STATISTICS_KEY_WIDTH,
            STATISTICS_LINE_HEIGHT,
        };
        if (statisticsType[i] == STATISTICS_TITLE)
            rectangle.width = CONTENT_WIDTH;

        offset = (mr_point_t){
            STATISTICS_KEY_OFFSETX,
            0,
        };
        if (i == 0)
        {
            rectangle.height += STATISTICS_TOPMARGIN_HEIGHT;
            offset.y += STATISTICS_TOPMARGIN_HEIGHT;
            y += STATISTICS_TOPMARGIN_HEIGHT;
        }
        else if (i == (STATISTICS_LINE_NUM - 1))
            rectangle.height += STATISTICS_BOTTOMMARGIN_HEIGHT;

        setStrokeColor((!valueString[0] && !unitString[0]) ? COLOR_ELEMENT_ACTIVE : COLOR_ELEMENT_NEUTRAL);
        drawText(keyString, &rectangle, &offset);

        // Value
        if (statisticsType[i] != STATISTICS_TITLE)
        {
            rectangle.x = STATISTICS_VALUE_LEFT;
            rectangle.width = STATISTICS_VALUE_WIDTH;
            offset.x = STATISTICS_VALUE_OFFSETX;
            drawValueAndUnit(valueString, unitString, &rectangle, &offset, false);
        }

        y += STATISTICS_LINE_HEIGHT;
    }
}
