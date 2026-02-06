/*
 * Rad Pro
 * Average rate
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../devices/voice.h"
#include "../measurements/average.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/measurements.h"
#include "../ui/menu.h"
#include "../ui/system.h"

typedef enum
{
    AVERAGE_TAB_TIME,
    AVERAGE_TAB_RATE,
    AVERAGE_TAB_DOSE,

    AVERAGE_TAB_NUM,

    AVERAGE_TAB_ALERT = AVERAGE_TAB_NUM,
} AverageTab;

static Menu averageMenu;

static void resetAverageRate(void);

static const int32_t averagingTimes[] = {
    30 * 24 * 60 * 60, // Off (actually 30 days)
    24 * 60 * 60,      // 24 hours
    12 * 60 * 60,      // 12 hours
    6 * 60 * 60,       // 6 hours
    3 * 60 * 60,       // 3 hours
    60 * 60,           // 1 hour
    30 * 60,           // 30 minutes
    10 * 60,           // 10 minutes
    5 * 60,            // 5 minutes
    60,                // 1 minute
    30,                // 30 minute
    10,                // 10 seconds
    5,                 // 5 seconds
    1,                 // 1 second
};

static const float averagingConfidences[] = {
    0.505F,  // ±50% confidence
    0.205F,  // ±20% confidence
    0.105F,  // ±10% confidence
    0.0505F, // ±5% confidence
    0.0205F, // ±2% confidence
    0.0105F, // ±1% confidence
};

static struct
{
    PulsePeriod period;
    Rate rate;

    uint32_t timedPulseCount;
    Rate timedRate;
    bool done;
} average;

static AverageTab averageTab;

void setupAverageRate(void)
{
    resetAverageRate();

    averageTab = AVERAGE_TAB_TIME;

    selectMenuItem(&averageMenu, settings.averaging, AVERAGING_NUM);
}

static void resetAverageRate(void)
{
    memset(&average, 0, sizeof(average));

    if (averageTab == AVERAGE_TAB_ALERT)
        averageTab = AVERAGE_TAB_TIME;
}

void updateAverageRate(PulsePeriod *period)
{
    if ((average.rate.time == UINT32_MAX) ||
        (average.period.pulseCount == UINT32_MAX))
        return;

    // Update average rate
    average.rate.time = addClamped(average.rate.time, 1);
    if (period->pulseCount)
    {
        if (!average.period.pulseCount)
            average.period.firstTick = period->firstTick;
        average.period.lastTick = period->lastTick;
        average.period.pulseCount = addClamped(average.period.pulseCount, period->pulseCount);

        calculateRate(&average.rate, &average.period);
    }

    // Timer
    bool done = (settings.averaging < AVERAGING_TIME_NUM)
                    ? (average.rate.time >= averagingTimes[settings.averaging])
                    : (average.rate.confidence != 0.0F) && (average.rate.confidence < averagingConfidences[settings.averaging - AVERAGING_TIME_NUM]);

    if (!done)
    {
        average.timedPulseCount = average.period.pulseCount;
        average.timedRate = average.rate;
    }

    if (!average.done && done)
    {
        average.done = true;

        average.timedPulseCount = average.period.pulseCount;
        average.timedRate = average.rate;

        triggerAlert(true);
    }
    else if (average.done && !done)
        average.done = false;

    if (isTubeFaultAlertTriggered())
        averageTab = AVERAGE_TAB_ALERT;
    else if (averageTab == AVERAGE_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel() && !done)
            averageTab = AVERAGE_TAB_TIME;
    }
}

float getAverageRate(void)
{
    return average.timedRate.value;
}

// Average rate view

static void onAverageRateViewEvent(Event event)
{
    if (onMeasurementViewEvent(event))
        return;

    bool done = (average.timedPulseCount == UINT32_MAX) ||
                average.done;

    const char *alertString;
    if (getTubeFaultAlertLevel())
        alertString = getString(STRING_ALERT_FAULT);
    else if ((average.rate.time == UINT32_MAX) || (average.period.pulseCount == UINT32_MAX))
        alertString = getString(STRING_ALERT_MAX);
    else if (done)
        alertString = getString(STRING_ALERT_DONE);
    else
        alertString = NULL;

    switch (event)
    {
    case EVENT_KEY_BACK:
        averageTab++;
        if (alertString
                ? averageTab >= (AVERAGE_TAB_NUM + 1)
                : averageTab >= AVERAGE_TAB_NUM)
            averageTab = AVERAGE_TAB_TIME;

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetAverageRate();

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceAverageRate();

        break;
#endif

    case EVENT_DRAW:
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString, unitString, average.timedRate.value, &pulseUnits[settings.doseUnits].rate, doseUnitsMinMetricPrefix[settings.doseUnits]);

        if (done)
            style = MEASUREMENTSTYLE_DONE;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_AVERAGE));
        drawMeasurementValue(valueString, unitString, average.timedRate.confidence, style);

        if (averageTab == AVERAGE_TAB_ALERT)
            drawMeasurementAlert(alertString);
        else
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

            switch (averageTab)
            {
            case AVERAGE_TAB_TIME:
                strcatTime(valueString, average.timedRate.time);

                keyString = getString(STRING_TIME);

                break;

            case AVERAGE_TAB_RATE:
                if (average.timedRate.value > 0)
                    buildValueString(valueString, unitString, average.timedRate.value, &pulseUnits[settings.secondaryDoseUnits].rate, doseUnitsMinMetricPrefix[settings.secondaryDoseUnits]);

                keyString = getString(STRING_RATE);

                break;

            case AVERAGE_TAB_DOSE:
                if (average.timedPulseCount > 0)
                    buildValueString(valueString, unitString, average.timedPulseCount, &pulseUnits[DOSE_UNITS_CPM].dose, doseUnitsMinMetricPrefix[DOSE_UNITS_CPM]);

                keyString = getString(STRING_DOSE);

                break;

            default:
                break;
            }

            drawMeasurementInfo(keyString, valueString, unitString, style);
        }

        break;
    }

    default:
        break;
    }
}

View averageRateView = {
    onAverageRateViewEvent,
    NULL,
};

// Average menu

static cstring averageMenuOptions[] = {
    STRING_UNLIMITED,
    STRING_24_HOURS,
    STRING_12_HOURS,
    STRING_6_HOURS,
    STRING_3_HOURS,
    STRING_1_HOUR,
    STRING_30_MINUTES,
    STRING_10_MINUTES,
    STRING_5_MINUTES,
    STRING_1_MINUTE,
    STRING_30_SECONDS,
    STRING_10_SECONDS,
    STRING_5_SECONDS,
    STRING_1_SECOND,
    STRING_50_CONFIDENCE,
    STRING_20_CONFIDENCE,
    STRING_10_CONFIDENCE,
    STRING_5_CONFIDENCE,
    STRING_2_CONFIDENCE,
    STRING_1_CONFIDENCE,
    NULL,
};

static const char *onAverageMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.averaging);

    return getString(averageMenuOptions[index]);
}

static void onAverageMenuSelect(uint32_t index)
{
    settings.averaging = index;
}

static MenuState averageMenuState;

static Menu averageMenu = {
    STRING_AVERAGE,
    &averageMenuState,
    onAverageMenuGetOption,
    onAverageMenuSelect,
    setMeasurementsMenu,
};

View averageMenuView = {
    onMenuEvent,
    &averageMenu,
};
