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

void resetInstantaneousRate(void)
{
    instantaneousTab = INSTANTANEOUS_TAB_BAR;

    clearInstantaneousRate();

    selectMenuItem(&instantaneousMenu, settings.instantaneousAveraging);
}

void clearInstantaneousRate(void)
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
    float rateSievertsPerHour = pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale * instantaneous.rate.value;

    AlertLevel alertLevel = isInstantaneousRateConfidenceGood()
                                ? getMeasurementAlertLevel(rateSievertsPerHour, rateAlerts, settings.rateWarning, settings.rateAlarm)
                                : ALERTLEVEL_NONE;

    updateMeasurementAlertLevel(&instantaneous.alertLevel, alertLevel);
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
    bool timedAveraging = isTimedAveraging();

    instantaneous.rate.time = 0;

    for (uint32_t i = 0; i < instantaneous.periodsLength; i++)
    {
        uint32_t time = ((periodTick - instantaneous.periods[i].firstTick) / SYSTICK_FREQUENCY) + 1;

        // Fixed intervals never include periods outside the averaging window
        bool timeSatisfied = (time > minTime);
        bool pulseSatisfied = timedAveraging ||
                              (averagingPeriod.pulseCount > INSTANTANEOUS_RATE_PULSE_COUNT_MIN);
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

    instantaneousTab = updateMeasurementAlertTab(instantaneousTab, INSTANTANEOUS_TAB_ALERT,
                                                 isTubeFaultAlertTriggered(), getTubeFaultAlertLevel());
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

void advanceInstantaneousRateTab(void)
{
    instantaneousTab = getNextMeasurementTab(instantaneousTab,
                                             INSTANTANEOUS_TAB_NUM + (getTubeFaultAlertLevel() != ALERTLEVEL_NONE));
}

static void drawInstantaneousRateTab(MeasurementStyle style)
{
    switch (instantaneousTab)
    {
    case INSTANTANEOUS_TAB_MAX:
        drawPulseMeasurementInfo(getString(STRING_MAX), instantaneous.maxValue, settings.doseUnits, false, style);

        break;

    case INSTANTANEOUS_TAB_RATE:
        drawPulseMeasurementInfo(getString(STRING_RATE), instantaneous.rate.value, settings.secondaryDoseUnits, false, style);

        break;

    case INSTANTANEOUS_TAB_CUMULATIVE:
        drawPulseMeasurementInfo(getString(STRING_CUMULATIVE), getCumulativeDosePulseCount(), settings.doseUnits, true, style);

        break;

    default:
        break;
    }
}

void drawInstantaneousRateView(void)
{
    MeasurementStyle style = getAlertMeasurementStyle(instantaneous.alertLevel);

    drawPulseMeasurementValue(getString(STRING_INSTANTANEOUS), instantaneous.rate.value, false, instantaneous.rate.confidence, style);

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
        drawInstantaneousRateTab(style);
    else
        drawMeasurementAlert(getString(STRING_ALERT_FAULT));
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
