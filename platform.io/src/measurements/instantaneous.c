/*
 * Rad Pro
 * Instantaneous rate
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/instantaneous.h"
#include "../peripherals/tube.h"
#include "../peripherals/voice.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/measurements.h"
#include "../ui/menu.h"
#include "../ui/system.h"

#define INSTANTANEOUS_RATE_PERIODS_NUM 60
#define INSTANTANEOUS_RATE_PULSE_COUNT_MIN 19 // For 50% confidence interval

typedef enum
{
    INSTANTANEOUS_TAB_BAR,
    INSTANTANEOUS_TAB_MAX,
    INSTANTANEOUS_TAB_RATE,
    INSTANTANEOUS_TAB_CUMULATIVE,

    INSTANTANEOUS_TAB_NUM,

    INSTANTANEOUS_TAB_ALERT = INSTANTANEOUS_TAB_NUM,
} InstantaneousTab;

static Menu instantaneousMenu;

static void resetInstantaneousRate(void);

static const uint8_t instantaneousMinTimes[] = {0, 5, 60, 30, 10};

static struct
{
    PulsePeriod periods[INSTANTANEOUS_RATE_PERIODS_NUM];
    uint32_t periodsLength;

    Rate rate;

    float compensationFactor;

    float maxValue;

    AlertLevel alertLevel;
} instantaneous;

static InstantaneousTab instantaneousTab;

void setupInstantaneousRate(void)
{
    resetInstantaneousRate();

    instantaneousTab = INSTANTANEOUS_TAB_BAR;

    selectMenuItem(&instantaneousMenu, settings.instantaneousAveraging, INSTANTANEOUSAVERAGING_NUM);
}

static void resetInstantaneousRate(void)
{
    memset(&instantaneous, 0, sizeof(instantaneous));

    if (instantaneousTab == INSTANTANEOUS_TAB_ALERT)
        instantaneousTab = INSTANTANEOUS_TAB_BAR;
}

void updateInstantaneousRate(uint32_t periodTick, PulsePeriod *period)
{
    // Enqueue period
    if (period->pulseCount)
    {
        for (uint32_t i = (INSTANTANEOUS_RATE_PERIODS_NUM - 1); i > 0; i--)
            instantaneous.periods[i] = instantaneous.periods[i - 1];
        instantaneous.periods[0] = *period;
        if (instantaneous.periodsLength < INSTANTANEOUS_RATE_PERIODS_NUM)
            instantaneous.periodsLength++;
    }

    // Instantaneous rate
    PulsePeriod averagingPeriod = {0, 0, 0};

    uint32_t minTime = instantaneousMinTimes[settings.instantaneousAveraging];
    uint32_t minPulseCount =
        (settings.instantaneousAveraging <= INSTANTANEOUSAVERAGING_ADAPTIVEPRECISION) ? INSTANTANEOUS_RATE_PULSE_COUNT_MIN : 0;

    instantaneous.rate.time = 0;

    for (uint32_t i = 0; i < instantaneous.periodsLength; i++)
    {
        uint32_t time = ((periodTick - instantaneous.periods[i].firstTick) / SYSTICK_FREQUENCY) + 1;

        if ((time > minTime) && (averagingPeriod.pulseCount > minPulseCount))
            break;

        averagingPeriod.firstTick = instantaneous.periods[i].firstTick;
        if (!averagingPeriod.pulseCount)
            averagingPeriod.lastTick = instantaneous.periods[i].lastTick;
        averagingPeriod.pulseCount += instantaneous.periods[i].pulseCount;

        instantaneous.rate.time = time;
    }

    calculateRate(&instantaneous.rate, &averagingPeriod);

    // Dead-time compensation
    if (!settings.tubeDeadTimeCompensation)
        instantaneous.compensationFactor = 1.0F;
    else
    {
        float denominator = 1 - instantaneous.rate.value * getTubeDeadTimeCompensation();
        if (denominator < 0.1F)
            denominator = 0.1F;
        instantaneous.compensationFactor = 1.0F / denominator;

        instantaneous.rate.value *= instantaneous.compensationFactor;
    }

    // Pulse threshold
    updatePulseThresholdExceeded();

    // Max value
    bool confidenceGood = isInstantaneousRateConfidenceGood();
    if (confidenceGood && (instantaneous.rate.value > instantaneous.maxValue))
        instantaneous.maxValue = instantaneous.rate.value;

    // Alerts
    AlertLevel alertLevel = ALERTLEVEL_NONE;
    float rateSvH = pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale * instantaneous.rate.value;
    if (confidenceGood)
    {
        if (settings.rateAlarm && (rateSvH >= rateAlerts[settings.rateAlarm]))
            alertLevel = ALERTLEVEL_ALARM;
        else if (settings.rateWarning && (rateSvH >= rateAlerts[settings.rateWarning]))
            alertLevel = ALERTLEVEL_WARNING;
    }
    bool alertTriggered = (alertLevel > instantaneous.alertLevel);
    instantaneous.alertLevel = alertLevel;

    if (alertTriggered)
        setAlertPending(true);

    // Alert view
    if (isTubeFaultAlertTriggered())
        instantaneousTab = INSTANTANEOUS_TAB_ALERT;
    else if (instantaneousTab == INSTANTANEOUS_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel())
            instantaneousTab = INSTANTANEOUS_TAB_BAR;
    }
}

float getInstantaneousRate(void)
{
    return instantaneous.rate.value;
}

float getDeadTimeCompensationFactor(void)
{
    return instantaneous.compensationFactor;
}

bool isInstantaneousRateConfidenceGood(void)
{
    return (settings.instantaneousAveraging >= INSTANTANEOUSAVERAGING_60_SECONDS) ||
           ((instantaneous.rate.confidence > 0.0F) &&
            (instantaneous.rate.confidence < 0.75F));
}

AlertLevel getInstantaneousRateAlertLevel(void)
{
    return instantaneous.alertLevel;
}

// Instantaneous rate view

static void onInstantaneousRateViewEvent(Event event)
{
    if (onMeasurementViewEvent(event))
        return;

    const char *alertString;
    if (getTubeFaultAlertLevel())
        alertString = getString(STRING_ALERT_FAULT);
    else
        alertString = NULL;

    switch (event)
    {
    case EVENT_KEY_BACK:
        instantaneousTab++;
        if (instantaneousTab >= (alertString ? (INSTANTANEOUS_TAB_NUM + 1) : INSTANTANEOUS_TAB_NUM))
            instantaneousTab = INSTANTANEOUS_TAB_BAR;

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetInstantaneousRate();

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceInstantaneousRate();

        break;
#endif

    case EVENT_DRAW:
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString, unitString, instantaneous.rate.value, &pulseUnits[settings.doseUnits].rate, doseUnitsMinMetricPrefix[settings.doseUnits]);

        if (instantaneous.alertLevel == ALERTLEVEL_ALARM)
            style = MEASUREMENTSTYLE_ALARM;
        else if (instantaneous.alertLevel == ALERTLEVEL_WARNING)
            style = MEASUREMENTSTYLE_WARNING;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_INSTANTANEOUS));
        drawMeasurementValue(valueString, unitString, instantaneous.rate.confidence, style);

        if (instantaneousTab == INSTANTANEOUS_TAB_ALERT)
            drawMeasurementAlert(getString(STRING_ALERT_FAULT));
        else if (instantaneousTab != INSTANTANEOUS_TAB_BAR)
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

            switch (instantaneousTab)
            {
            case INSTANTANEOUS_TAB_MAX:
                buildValueString(valueString, unitString, instantaneous.maxValue, &pulseUnits[settings.doseUnits].rate, doseUnitsMinMetricPrefix[settings.doseUnits]);

                keyString = getString(STRING_MAX);

                break;

            case INSTANTANEOUS_TAB_RATE:
                buildValueString(valueString, unitString, instantaneous.rate.value, &pulseUnits[settings.secondaryDoseUnits].rate, doseUnitsMinMetricPrefix[settings.secondaryDoseUnits]);

                keyString = getString(STRING_RATE);

                break;

            case INSTANTANEOUS_TAB_CUMULATIVE:
                buildValueString(valueString, unitString, getCumulativeDosePulseCount(), &pulseUnits[settings.doseUnits].dose, doseUnitsMinMetricPrefix[settings.doseUnits]);

                keyString = getString(STRING_CUMULATIVE);

                break;

            default:
                break;
            }

            drawMeasurementInfo(keyString, valueString, unitString, style);
        }
        else
        {
            float scale = pulseUnits[settings.doseUnits].rate.scale;
            float uSvHScale = scale / pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale;
            drawMeasurementBar(instantaneous.rate.value * scale, doseUnitsBarMinExponent[settings.doseUnits], rateAlerts[settings.rateWarning] * uSvHScale, rateAlerts[settings.rateAlarm] * uSvHScale);
        }

        break;
    }

    default:
        break;
    }
}

View instantaneousRateView = {
    onInstantaneousRateViewEvent,
    NULL,
};

// Instantaneous menu

static cstring instantaneousMenuOptions[] = {
    STRING_ADAPTIVE_FAST,
    STRING_ADAPTIVE_PRECISION,
    STRING_1_MINUTE,
    STRING_30_SECONDS,
    STRING_10_SECONDS,
    NULL,
};

static const char *onInstantaneousMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.instantaneousAveraging);

    return getString(instantaneousMenuOptions[index]);
}

static void onInstantaneousMenuSelect(uint32_t index)
{
    settings.instantaneousAveraging = index;
}

static MenuState instantaneousMenuState;

static Menu instantaneousMenu = {
    STRING_INSTANTANEOUS,
    &instantaneousMenuState,
    onInstantaneousMenuGetOption,
    onInstantaneousMenuSelect,
    setMeasurementsMenu,
};

View instantaneousMenuView = {
    onMenuEvent,
    &instantaneousMenu,
};
