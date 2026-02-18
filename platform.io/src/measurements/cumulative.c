/*
 * Rad Pro
 * Cumulative dose
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include "../measurements/cumulative.h"
#include "../measurements/instantaneous.h"
#include "../peripherals/voice.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/measurements.h"
#include "../ui/system.h"

typedef enum
{
    CUMULATIVE_TAB_TIME,
    CUMULATIVE_TAB_DOSE,
    CUMULATIVE_TAB_INSTANTANEOUS,

    CUMULATIVE_TAB_NUM,

    CUMULATIVE_TAB_ALERT = CUMULATIVE_TAB_NUM,
} CumulativeTab;

static void resetCumulativeDose(void);

static struct
{
    Dose dose;

    AlertLevel alertLevel;
} cumulative;

static CumulativeTab cumulativeTab;

void setupCumulativeDose(void)
{
    cumulativeTab = CUMULATIVE_TAB_TIME;

    Dose tubeDose = cumulative.dose;
    resetCumulativeDose();
    cumulative.dose = tubeDose;
}

static void resetCumulativeDose(void)
{
    memset(&cumulative, 0, sizeof(cumulative));

    if (cumulativeTab == CUMULATIVE_TAB_ALERT)
        cumulativeTab = CUMULATIVE_TAB_TIME;
}

static AlertLevel computeCumulativeDoseAlertLevel(float doseSieverts)
{
    if (settings.doseAlarm && (doseSieverts >= doseAlerts[settings.doseAlarm]))
        return ALERTLEVEL_ALARM;

    if (settings.doseWarning && (doseSieverts >= doseAlerts[settings.doseWarning]))
        return ALERTLEVEL_WARNING;

    return ALERTLEVEL_NONE;
}

static bool isCumulativeDoseSaturated(void)
{
    return (cumulative.dose.time == UINT32_MAX) || (cumulative.dose.pulseCount == UINT32_MAX);
}

static void updateCumulativeDoseAlerts(void)
{
    float doseSieverts = pulseUnits[DOSE_UNITS_SIEVERTS].dose.scale * cumulative.dose.pulseCount;
    AlertLevel alertLevel = computeCumulativeDoseAlertLevel(doseSieverts);

    AlertLevel previousAlertLevel = cumulative.alertLevel;
    cumulative.alertLevel = alertLevel;

    bool alertTriggered = (alertLevel > previousAlertLevel);
    if (alertTriggered)
        setAlertPending(true);
}

static void updateCumulativeDoseTab(void)
{
    if (isTubeFaultAlertTriggered())
        cumulativeTab = CUMULATIVE_TAB_ALERT;
    else if (cumulativeTab == CUMULATIVE_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel())
            cumulativeTab = CUMULATIVE_TAB_TIME;
    }
}

void updateCumulativeDose(PulsePeriod *period)
{
    if (isCumulativeDoseSaturated())
        return;

    // Update cumulative dose
    cumulative.dose.time = addClamped(cumulative.dose.time, 1);
    cumulative.dose.pulseCount = addClamped(cumulative.dose.pulseCount, period->pulseCount);

    updateCumulativeDoseAlerts();

    updateCumulativeDoseTab();
}

void setCumulativeDoseTime(uint32_t value)
{
    cumulative.dose.time = value;
}

uint32_t getCumulativeDoseTime(void)
{
    return cumulative.dose.time;
}

void setCumulativeDosePulseCount(uint32_t value)
{
    cumulative.dose.pulseCount = value;
}

uint32_t getCumulativeDosePulseCount(void)
{
    return cumulative.dose.pulseCount;
}

AlertLevel getCumulativeDoseAlertLevel(void)
{
    return cumulative.alertLevel;
}

// Cumulative dose view

static void advanceCumulativeDoseTab(void)
{
    uint32_t tabNum = getTubeFaultAlertLevel() ? (CUMULATIVE_TAB_NUM + 1) : CUMULATIVE_TAB_NUM;

    cumulativeTab++;
    if (cumulativeTab >= tabNum)
        cumulativeTab = 0;
}

static MeasurementStyle getCumulativeDoseMeasurementStyle(void)
{
    if (cumulative.alertLevel == ALERTLEVEL_ALARM)
        return MEASUREMENTSTYLE_ALARM;

    if (cumulative.alertLevel == ALERTLEVEL_WARNING)
        return MEASUREMENTSTYLE_WARNING;

    if (isCumulativeDoseSaturated())
        return MEASUREMENTSTYLE_DONE;

    return MEASUREMENTSTYLE_NORMAL;
}

static void drawCumulativeDoseValue(void)
{
    char valueString[32] = "";
    char unitString[32] = "";
    buildValueString(valueString, unitString, cumulative.dose.pulseCount, &pulseUnits[settings.doseUnits].dose, doseUnitsMinMetricPrefix[settings.doseUnits]);

    drawTitleBar(getString(STRING_CUMULATIVE));
    drawMeasurementValue(valueString, unitString, 0, getCumulativeDoseMeasurementStyle());
}

static void drawCumulativeDoseTab(void)
{
    char valueString[32] = "";
    char unitString[32] = " ";
    const char *keyString = NULL;

    switch (cumulativeTab)
    {
    case CUMULATIVE_TAB_TIME:
        strcatTime(valueString, cumulative.dose.time);

        keyString = getString(STRING_TIME);

        break;

    case CUMULATIVE_TAB_DOSE:
        buildValueString(valueString, unitString, cumulative.dose.pulseCount, &pulseUnits[settings.secondaryDoseUnits].dose, doseUnitsMinMetricPrefix[settings.secondaryDoseUnits]);

        keyString = getString(STRING_DOSE);

        break;

    case CUMULATIVE_TAB_INSTANTANEOUS:
        buildValueString(valueString, unitString, getInstantaneousRate(), &pulseUnits[settings.doseUnits].rate, doseUnitsMinMetricPrefix[settings.doseUnits]);

        keyString = getString(STRING_INSTANTANEOUS);

        break;

    default:
        return;
    }

    drawMeasurementInfo(keyString, valueString, unitString, getCumulativeDoseMeasurementStyle());
}

static void drawCumulativeDoseView(void)
{
    drawCumulativeDoseValue();

    if (cumulativeTab == CUMULATIVE_TAB_ALERT)
        drawMeasurementAlert(getString(STRING_ALERT_FAULT));
    else
        drawCumulativeDoseTab();
}

void onCumulativeDoseViewEvent(ViewEvent event)
{
    if (onMeasurementViewEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        advanceCumulativeDoseTab();

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetCumulativeDose();

        requestViewUpdate();

        break;

#if defined(VOICE)
    case EVENT_KEY_PLAY:
        playVoiceCumulativeDose();

        break;
#endif

    case EVENT_DRAW:
        drawCumulativeDoseView();

        break;

    default:
        break;
    }
}
