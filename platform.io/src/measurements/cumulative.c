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

static struct
{
    Dose dose;

    AlertLevel alertLevel;
} cumulative;

static CumulativeTab cumulativeTab;

void resetCumulativeDose(void)
{
    cumulativeTab = CUMULATIVE_TAB_TIME;

    Dose tubeDose = cumulative.dose;
    clearCumulativeDose();
    cumulative.dose = tubeDose;
}

void clearCumulativeDose(void)
{
    memset(&cumulative, 0, sizeof(cumulative));

    if (cumulativeTab == CUMULATIVE_TAB_ALERT)
        cumulativeTab = CUMULATIVE_TAB_TIME;
}

static bool isCumulativeDoseSaturated(void)
{
    return (cumulative.dose.time == UINT32_MAX) || (cumulative.dose.pulseCount == UINT32_MAX);
}

void updateCumulativeDose(uint32_t pulseCount)
{
    if (isCumulativeDoseSaturated())
        return;

    // Update cumulative dose
    cumulative.dose.time = addClamped(cumulative.dose.time, 1);
    cumulative.dose.pulseCount = addClamped(cumulative.dose.pulseCount, pulseCount);

    // Alerts
    float doseSieverts = pulseUnits[DOSE_UNITS_SIEVERTS].dose.scale * cumulative.dose.pulseCount;
    updateMeasurementAlertLevel(&cumulative.alertLevel,
                                getMeasurementAlertLevel(doseSieverts, doseAlerts, settings.doseWarning, settings.doseAlarm));

    cumulativeTab = updateMeasurementAlertTab(cumulativeTab, CUMULATIVE_TAB_ALERT,
                                              isTubeFaultAlertTriggered(), getTubeFaultAlertLevel());
}

Dose *getCumulativeDose(void)
{
    return &cumulative.dose;
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

void advanceCumulativeDoseTab(void)
{
    cumulativeTab = getNextMeasurementTab(cumulativeTab,
                                          CUMULATIVE_TAB_NUM + (getTubeFaultAlertLevel() != ALERTLEVEL_NONE));
}

void drawCumulativeDoseView(void)
{
    MeasurementStyle style = (!cumulative.alertLevel && isCumulativeDoseSaturated())
                                 ? MEASUREMENTSTYLE_DONE
                                 : getAlertMeasurementStyle(cumulative.alertLevel);

    drawPulseMeasurementValue(getString(STRING_CUMULATIVE), cumulative.dose.pulseCount, true, 0, style);

    switch (cumulativeTab)
    {
    case CUMULATIVE_TAB_TIME:
        drawMeasurementInfoTime(getString(STRING_TIME), cumulative.dose.time, style);

        break;

    case CUMULATIVE_TAB_DOSE:
        drawPulseMeasurementInfo(getString(STRING_DOSE), cumulative.dose.pulseCount, settings.secondaryDoseUnits, true, style);

        break;

    case CUMULATIVE_TAB_INSTANTANEOUS:
        drawPulseMeasurementInfo(getString(STRING_INSTANTANEOUS), getInstantaneousRate(), settings.doseUnits, false, style);

        break;

    default:
        drawMeasurementAlert(getString(STRING_ALERT_FAULT));

        break;
    }
}

