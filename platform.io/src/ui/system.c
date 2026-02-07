/*
 * Rad Pro
 * UI system
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../devices/rtc.h"
#include "../measurements/measurements.h"
#include "../system/settings.h"
#include "../ui/draw.h"
#include "../ui/system.h"

void drawPowerOff(bool displayBatteryIcon)
{
    char buffer[16];

    setFillColor(COLOR_CONTAINER_BACKGROUND);
    if (displayBatteryIcon)
        setupBatteryIcon(buffer);
    else
        strclr(buffer);
    drawCenteredText(buffer,
                     &(mr_rectangle_t){0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT},
                     &(mr_point_t){(DISPLAY_WIDTH / 2), ((DISPLAY_HEIGHT - FONT_SYMBOLS_LINE_HEIGHT) / 2)});
}

static const ColorIndex alertLevelColorIndex[] = {
    COLOR_ELEMENT_NEUTRAL,
    COLOR_WARNING,
    COLOR_ALARM,
};

void drawTitleBar(const char *title)
{
    char buffer[16];
    mr_rectangle_t rectangle = {
        TITLEBAR_WIDTH,
        TITLEBAR_TOP,
        0,
        TITLEBAR_CONTENT_HEIGHT,
    };
    setFillColor(COLOR_CONTAINER_GLOBAL);

    // Time
    RTCDateTime dateTime;
    getDeviceDateTime(&dateTime);
    if (dateTime.year >= RTC_YEAR_MIN)
    {
        strclr(buffer);
        if (settings.rtcTimeFormat == RTC_TIMEFORMAT_24_HOUR)
            strcatUInt32(buffer, dateTime.hour, 2);
        else
        {
            uint32_t hour = dateTime.hour % 12;
            strcatUInt32(buffer, (hour == 0) ? 12 : hour, 1);
        }
        strcatChar(buffer, ':');
        strcatUInt32(buffer, dateTime.minute, 2);

        setFont(font_small);
        setStrokeColor(COLOR_ELEMENT_ACTIVE);
        drawRowRight(buffer, &rectangle);
    }

    // Battery icon
    setupBatteryIcon(buffer);
    drawRowRight(buffer, &rectangle);

    // Alert icon
    AlertLevel alertLevel = getAlertLevel();
    if (isAlertEnabled() || alertLevel)
    {
        bool notificationIconFilled;
        if (!isAlertFlashing())
        {
            alertLevel = ALERTLEVEL_NONE;
            notificationIconFilled = false;
        }
        else
            notificationIconFilled = isAlertPending();

        setStrokeColor(alertLevelColorIndex[alertLevel]);
        drawRowRight(notificationIconFilled ? ";" : ":", &rectangle);
    }

    // Lock icon
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    if (isInLockMode())
        drawRowRight("<", &rectangle);

    // Sound icon
    if (isSoundIconActive())
        drawRowRight(settings.pulseSound ? "9" : "8", &rectangle);

    // Title
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_ACTIVE);
    drawRowLeft(title, &rectangle);

    // Shadow
#if TITLEBAR_SHADOW_HEIGHT > 0
    setFillColor(COLOR_CONTAINER_GLOBAL_SHADOW);
    drawRectangle(&(mr_rectangle_t){TITLEBAR_LEFT, TITLEBAR_BOTTOM - TITLEBAR_SHADOW_HEIGHT, TITLEBAR_WIDTH, 1});
#endif

    // Set background
    setFillColor(COLOR_CONTAINER_BACKGROUND);
}

void drawSplash(const char *message)
{
    setFillColor(COLOR_CONTAINER_GLOBAL);

    drawCenteredMultilineText(&(mr_rectangle_t){0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT},
                              message);
}

void drawNotification(const char *title, const char *message)
{
    drawTitleBar(title);

    drawCenteredMultilineText(&contentRectangle, message);
}
