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

static AlertLevel computeCumulativeAlertLevel(float doseSv)
{
    if (settings.doseAlarm && (doseSv >= doseAlerts[settings.doseAlarm]))
        return ALERTLEVEL_ALARM;
    
    if (settings.doseWarning && (doseSv >= doseAlerts[settings.doseWarning]))
        return ALERTLEVEL_WARNING;

    return ALERTLEVEL_NONE;
}

void updateCumulativeDose(PulsePeriod *period)
{
    if ((cumulative.dose.time == UINT32_MAX) ||
        (cumulative.dose.pulseCount == UINT32_MAX))
        return;

    // Update cumulative dose
    cumulative.dose.time = addClamped(cumulative.dose.time, 1);
    cumulative.dose.pulseCount = addClamped(cumulative.dose.pulseCount, period->pulseCount);

    // Secondary variables
    float doseSv = pulseUnits[DOSE_UNITS_SIEVERTS].dose.scale *
                   cumulative.dose.pulseCount;

    // Alerts
    AlertLevel newAlertLevel = computeCumulativeAlertLevel(doseSv);
    if (newAlertLevel > cumulative.alertLevel)
        setAlertPending(true);
    cumulative.alertLevel = newAlertLevel;

    // Alert view
    if (isTubeFaultAlertTriggered())
        cumulativeTab = CUMULATIVE_TAB_ALERT;
    else if (cumulativeTab == CUMULATIVE_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel())
            cumulativeTab = CUMULATIVE_TAB_TIME;
    }
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

static void onCumulativeDoseViewEvent(Event event)
{
    if (onMeasurementViewEvent(event))
        return;

    bool done = (cumulative.dose.time == UINT32_MAX) || (cumulative.dose.pulseCount == UINT32_MAX);

    const char *alertString;
    if (getTubeFaultAlertLevel())
        alertString = getString(STRING_ALERT_FAULT);
    else
        alertString = NULL;

    switch (event)
    {
    case EVENT_KEY_BACK:
        cumulativeTab++;

        if (alertString ? cumulativeTab >= (CUMULATIVE_TAB_NUM + 1) : cumulativeTab >= CUMULATIVE_TAB_NUM)
            cumulativeTab = 0;

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
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString, unitString, cumulative.dose.pulseCount, &pulseUnits[settings.doseUnits].dose, doseUnitsMinMetricPrefix[settings.doseUnits]);

        if (cumulative.alertLevel == ALERTLEVEL_ALARM)
            style = MEASUREMENTSTYLE_ALARM;
        else if (cumulative.alertLevel == ALERTLEVEL_WARNING)
            style = MEASUREMENTSTYLE_WARNING;
        else if (done)
            style = MEASUREMENTSTYLE_DONE;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_CUMULATIVE));
        drawMeasurementValue(valueString, unitString, 0, style);

        if (cumulativeTab == CUMULATIVE_TAB_ALERT)
            drawMeasurementAlert(alertString);
        else
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

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

View cumulativeDoseView = {
    onCumulativeDoseViewEvent,
    NULL,
};
