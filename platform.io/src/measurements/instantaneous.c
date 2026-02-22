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
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/measurements.h"
#include "../ui/menu.h"
#include "../ui/system.h"

#define INSTANTANEOUS_RATE_PERIODS_NUM 60
#define INSTANTANEOUS_RATE_PULSE_COUNT_MIN 19 // For 50% confidence interval

#define INSTANTANEOUS_RATE_CONFIDENCE_THRESHOLD 0.75F

typedef enum
{
    INSTANTANEOUS_TAB_BAR,
    INSTANTANEOUS_TAB_MAX,
    INSTANTANEOUS_TAB_RATE,
    INSTANTANEOUS_TAB_CUMULATIVE,

    INSTANTANEOUS_TAB_NUM,

    INSTANTANEOUS_TAB_ALERT = INSTANTANEOUS_TAB_NUM,
} InstantaneousTab;

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
    instantaneousTab = INSTANTANEOUS_TAB_BAR;

    resetInstantaneousRate();

    selectMenuItem(&instantaneousMenu, settings.instantaneousAveraging);
}

static void resetInstantaneousRate(void)
{
    memset(&instantaneous, 0, sizeof(instantaneous));

    if (instantaneousTab == INSTANTANEOUS_TAB_ALERT)
        instantaneousTab = INSTANTANEOUS_TAB_BAR;
}

static bool isTimedAveraging(void)
{
    return (settings.instantaneousAveraging >= INSTANTANEOUSAVERAGING_60_SECONDS);
}

static void applyDeadTimeCompensation(void)
{
    if (!settings.tubeDeadTimeCompensation)
    {
        instantaneous.compensationFactor = 1.0F;

        return;
    }

    float tau = getTubeDeadTimeCompensation();
    float denominator = 1.0F - instantaneous.rate.value * tau;

    if (denominator < 0.1F)
        denominator = 0.1F;

    instantaneous.compensationFactor = 1.0F / denominator;
    instantaneous.rate.value *= instantaneous.compensationFactor;
}

static void updateInstantaneousRateAlerts(void)
{
    AlertLevel alertLevel = ALERTLEVEL_NONE;
    float rateSievertsPerHour = pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale * instantaneous.rate.value;

    if (isInstantaneousRateConfidenceGood())
    {
        if (settings.rateAlarm && (rateSievertsPerHour >= rateAlerts[settings.rateAlarm]))
            alertLevel = ALERTLEVEL_ALARM;
        else if (settings.rateWarning && (rateSievertsPerHour >= rateAlerts[settings.rateWarning]))
            alertLevel = ALERTLEVEL_WARNING;
    }

    AlertLevel previousAlertLevel = instantaneous.alertLevel;
    instantaneous.alertLevel = alertLevel;

    bool alertTriggered = (alertLevel > previousAlertLevel);
    if (alertTriggered)
        setAlertPending(true);
}

static void updateInstantaneousRateTab(void)
{
    if (isTubeFaultAlertTriggered())
        instantaneousTab = INSTANTANEOUS_TAB_ALERT;
    else if (instantaneousTab == INSTANTANEOUS_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel())
            instantaneousTab = INSTANTANEOUS_TAB_BAR;
    }
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

        bool timeSatisfied = (time > minTime);
        bool pulseSatisfied = (averagingPeriod.pulseCount > minPulseCount);
        if (timeSatisfied && pulseSatisfied)
            break;

        averagingPeriod.firstTick = instantaneous.periods[i].firstTick;
        if (!averagingPeriod.pulseCount)
            averagingPeriod.lastTick = instantaneous.periods[i].lastTick;
        averagingPeriod.pulseCount += instantaneous.periods[i].pulseCount;

        instantaneous.rate.time = time;
    }

    calculateRate(&instantaneous.rate, &averagingPeriod);

    applyDeadTimeCompensation();

    updatePulseThresholdExceeded();

    // Max value
    if (isInstantaneousRateConfidenceGood() &&
        (instantaneous.rate.value > instantaneous.maxValue))
        instantaneous.maxValue = instantaneous.rate.value;

    updateInstantaneousRateAlerts();

    updateInstantaneousRateTab();
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
    return isTimedAveraging() ||
           ((instantaneous.rate.confidence > 0.0F) && (instantaneous.rate.confidence < INSTANTANEOUS_RATE_CONFIDENCE_THRESHOLD));
}

AlertLevel getInstantaneousRateAlertLevel(void)
{
    return instantaneous.alertLevel;
}

// Instantaneous rate view

static void advanceInstantaneousRateTab(void)
{
    uint32_t tabNum = getTubeFaultAlertLevel() ? (INSTANTANEOUS_TAB_NUM + 1) : INSTANTANEOUS_TAB_NUM;

    instantaneousTab++;
    if (instantaneousTab >= tabNum)
        instantaneousTab = INSTANTANEOUS_TAB_BAR;
}

static MeasurementStyle getInstantaneousRateMeasurementStyle(void)
{
    if (instantaneous.alertLevel == ALERTLEVEL_ALARM)
        return MEASUREMENTSTYLE_ALARM;
    else if (instantaneous.alertLevel == ALERTLEVEL_WARNING)
        return MEASUREMENTSTYLE_WARNING;
    else
        return MEASUREMENTSTYLE_NORMAL;
}

static void drawInstantaneousRateValue(void)
{
    char valueString[32] = "";
    char unitString[32] = "";
    buildValueString(valueString, unitString, instantaneous.rate.value, &pulseUnits[settings.doseUnits].rate, doseUnitsMinMetricPrefix[settings.doseUnits]);

    drawTitleBar(getString(STRING_INSTANTANEOUS));
    drawMeasurementValue(valueString, unitString, instantaneous.rate.confidence, getInstantaneousRateMeasurementStyle());
}

static void drawInstantaneousRateTab(void)
{
    char valueString[32] = "";
    char unitString[32] = " ";
    const char *keyString = NULL;

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

    drawMeasurementInfo(keyString, valueString, unitString, getInstantaneousRateMeasurementStyle());
}

static void drawInstantaneousRateView(void)
{
    drawInstantaneousRateValue();

    if (instantaneousTab == INSTANTANEOUS_TAB_BAR)
    {
        float scale = pulseUnits[settings.doseUnits].rate.scale;
        float microSievertPerHourScale = scale / pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale;

        drawMeasurementBar(instantaneous.rate.value * scale,
                           doseUnitsBarMinExponent[settings.doseUnits],
                           rateAlerts[settings.rateWarning] * microSievertPerHourScale,
                           rateAlerts[settings.rateAlarm] * microSievertPerHourScale);
    }
    else if (instantaneousTab != INSTANTANEOUS_TAB_ALERT)
        drawInstantaneousRateTab();
    else
        drawMeasurementAlert(getString(STRING_ALERT_FAULT));
}

void onInstantaneousRateViewEvent(ViewEvent event)
{
    if (onMeasurementViewEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        advanceInstantaneousRateTab();

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
        drawInstantaneousRateView();

        break;

    default:
        break;
    }
}

// Instantaneous menu

static cstring instantaneousMenuOptions[] = {
    STRING_ADAPTIVE_FAST,
    STRING_ADAPTIVE_PRECISION,
    STRING_1_MINUTE,
    STRING_30_SECONDS,
    STRING_10_SECONDS,
};

static const char *onInstantaneousMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.instantaneousAveraging);

    return getString(instantaneousMenuOptions[index]);
}

static void onInstantaneousMenuSelect(menu_size_t index)
{
    settings.instantaneousAveraging = index;
}

static MenuState instantaneousMenuState;

const Menu instantaneousMenu = {
    STRING_INSTANTANEOUS,
    &instantaneousMenuState,
    ARRAY_SIZE(instantaneousMenuOptions),
    onInstantaneousMenuGetOption,
    onInstantaneousMenuSelect,
    showMeasurementsMenu,
};
