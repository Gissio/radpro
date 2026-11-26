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

// Cumulative pulse counts: the last 64 seconds, plus 64 entries at 16 s
// boundaries (about 17 minutes). Windows above 64 s start on a boundary.
#define INSTANTANEOUS_TIER1_LENGTH 64
#define INSTANTANEOUS_TIER2_LENGTH 64
#define INSTANTANEOUS_TIER2_BLOCK 16

#define INSTANTANEOUS_FAST_PULSE_TARGET 20       // +/-50% confidence
#define INSTANTANEOUS_PRECISION_PULSE_TARGET 100 // +/-20% confidence
#define INSTANTANEOUS_FIXED_WINDOW 60

// Change detection: the last k = 1, 2, 4 ... 512 s are compared with the
// older part of the window by a likelihood ratio (G) test. G > 22 gives
// about 0.01-0.06 false restarts per hour (tests/measurement-sim).
#define INSTANTANEOUS_CHANGE_LENGTH_MAX 512
#define INSTANTANEOUS_CHANGE_THRESHOLD (22.0F / 2) // Compared with G / 2

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

static struct
{
    uint32_t time;
    uint32_t pulseCount;
    uint32_t restartTime;
    uint32_t tier1PulseCounts[INSTANTANEOUS_TIER1_LENGTH];
    uint32_t tier2PulseCounts[INSTANTANEOUS_TIER2_LENGTH];

    Rate rate;

    float maxValue;

    AlertLevel alertLevel;
} instantaneous;

static InstantaneousTab instantaneousTab;

void resetInstantaneousRate(void)
{
    // Settings from 3.1 may hold the removed 30 s and 10 s modes
    if (settings.instantaneousAveraging >= INSTANTANEOUSAVERAGING_NUM)
        settings.instantaneousAveraging = INSTANTANEOUSAVERAGING_ADAPTIVEFAST;

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

static void updateInstantaneousRateAlerts(void)
{
    // Alerts start at the user's threshold. Hysteresis: they end once the
    // reading is below the threshold by more than its confidence interval,
    // i.e. below threshold / (1 + confidence).
    float rateSievertsPerHour = pulseUnits[DOSE_UNITS_SIEVERTS].rate.scale * instantaneous.rate.value;

    AlertLevel raiseLevel = isInstantaneousRateConfidenceGood()
                                ? getMeasurementAlertLevel(rateSievertsPerHour, rateAlerts, settings.rateWarning, settings.rateAlarm)
                                : ALERTLEVEL_NONE;
    AlertLevel holdLevel = getMeasurementAlertLevel(rateSievertsPerHour * (1 + instantaneous.rate.confidence),
                                                    rateAlerts, settings.rateWarning, settings.rateAlarm);

    AlertLevel alertLevel = instantaneous.alertLevel;
    if (alertLevel > holdLevel)
        alertLevel = holdLevel;
    if (alertLevel < raiseLevel)
        alertLevel = raiseLevel;

    updateMeasurementAlertLevel(&instantaneous.alertLevel, alertLevel);
}

static uint32_t getPastPulseCount(uint32_t time)
{
    if ((instantaneous.time - time) <= INSTANTANEOUS_TIER1_LENGTH)
        return instantaneous.tier1PulseCounts[time % INSTANTANEOUS_TIER1_LENGTH];
    else
        return instantaneous.tier2PulseCounts[(time / INSTANTANEOUS_TIER2_BLOCK) % INSTANTANEOUS_TIER2_LENGTH];
}

static uint32_t getWindowDepth(uint32_t time)
{
    // The oldest tier 2 entry is 63 blocks before the latest block boundary
    return (INSTANTANEOUS_TIER2_LENGTH - 1) * INSTANTANEOUS_TIER2_BLOCK +
           (time % INSTANTANEOUS_TIER2_BLOCK);
}

NOINLINE static float getLogLikelihoodTerm(uint32_t pulseCount, float expectedPulseCount)
{
    return pulseCount ? pulseCount * logf(pulseCount / expectedPulseCount) : 0;
}

static float getChangeStatistic(uint32_t recentPulseCount, uint32_t olderPulseCount,
                                uint32_t recentTime, uint32_t olderTime)
{
    // Likelihood ratio (G / 2) against both periods having the same rate
    float pulseCount = recentPulseCount + olderPulseCount;
    float recentExpected = pulseCount * recentTime / (recentTime + olderTime);

    return getLogLikelihoodTerm(recentPulseCount, recentExpected) +
           getLogLikelihoodTerm(olderPulseCount, pulseCount - recentExpected);
}

static void detectInstantaneousRateChange(uint32_t time, uint32_t currentPulseCount)
{
    // Longest window since the last restart (tier 2 windows start on a block
    // boundary)
    uint32_t length = time - instantaneous.restartTime;
    uint32_t depth = getWindowDepth(time);
    if (length > depth)
        length = depth;
    if (length > INSTANTANEOUS_TIER1_LENGTH)
    {
        length -= (length - time) % INSTANTANEOUS_TIER2_BLOCK;
        if (length < INSTANTANEOUS_TIER1_LENGTH)
            length = INSTANTANEOUS_TIER1_LENGTH;
    }
    uint32_t pulseCount = currentPulseCount - getPastPulseCount(time - length);

    // Restart the window at the recent period that differs most, if any
    // differs significantly
    float maxG = INSTANTANEOUS_CHANGE_THRESHOLD;
    for (uint32_t recentTime = 1;
         recentTime <= INSTANTANEOUS_CHANGE_LENGTH_MAX;
         recentTime <<= 1)
    {
        uint32_t recentLength = recentTime;
        if (recentLength > INSTANTANEOUS_TIER1_LENGTH)
            recentLength += (time - recentLength) % INSTANTANEOUS_TIER2_BLOCK;
        if ((2 * recentLength) > length)
            break;

        uint32_t recentPulseCount = currentPulseCount - getPastPulseCount(time - recentLength);
        float g = getChangeStatistic(recentPulseCount, pulseCount - recentPulseCount,
                                     recentLength, length - recentLength);
        if (g > maxG)
        {
            maxG = g;
            instantaneous.restartTime = time - recentLength;
        }
    }
}

void updateInstantaneousRate(uint32_t pulseCount)
{
    // Store cumulative pulse counts
    uint32_t time = instantaneous.time;
    instantaneous.tier1PulseCounts[time % INSTANTANEOUS_TIER1_LENGTH] = instantaneous.pulseCount;
    time++;
    instantaneous.time = time;
    uint32_t currentPulseCount = instantaneous.pulseCount + pulseCount;
    instantaneous.pulseCount = currentPulseCount;
    if (!(time % INSTANTANEOUS_TIER2_BLOCK))
        instantaneous.tier2PulseCounts[(time / INSTANTANEOUS_TIER2_BLOCK) % INSTANTANEOUS_TIER2_LENGTH] = currentPulseCount;

    // Averaging window: the shortest holding the pulse target, else the
    // longest available
    uint32_t pulseTarget = UINT32_MAX;
    uint32_t windowStartMin = 0;
    uint32_t windowMax = getWindowDepth(time);
    if (isTimedAveraging())
    {
        if (windowMax > INSTANTANEOUS_FIXED_WINDOW)
            windowMax = INSTANTANEOUS_FIXED_WINDOW;
    }
    else
    {
        pulseTarget = (settings.instantaneousAveraging == INSTANTANEOUSAVERAGING_ADAPTIVEPRECISION)
                          ? INSTANTANEOUS_PRECISION_PULSE_TARGET
                          : INSTANTANEOUS_FAST_PULSE_TARGET;

        // Adaptive windows start after the last detected rate change
        detectInstantaneousRateChange(time, currentPulseCount);
        windowStartMin = instantaneous.restartTime;
    }
    if (windowMax > (time - windowStartMin))
        windowMax = time - windowStartMin;

    uint32_t start = time;
    uint32_t n = 0;
    while (n < pulseTarget)
    {
        // Start of the next longer window: tier 2 windows start on a block
        // boundary (wraps at 0, ending the loop)
        uint32_t previousStart = start - 1;
        if ((time - start) >= INSTANTANEOUS_TIER1_LENGTH)
            previousStart &= ~(INSTANTANEOUS_TIER2_BLOCK - 1);
        if ((time - previousStart) > windowMax)
            break;

        start = previousStart;
        n = currentPulseCount - getPastPulseCount(start);
    }

    // A window ended by the pulse target is estimated as (n - 1) / W
    if (n >= pulseTarget)
        n--;
    calculateRate(&instantaneous.rate, n, time - start);

    instantaneous.rate.value *= getDeadTimeCompensationFactor(instantaneous.rate.value);

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
