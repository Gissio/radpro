/*
 * Rad Pro
 * Electric field
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(EMFMETER)

#include "../devices/adc.h"
#include "../devices/emf.h"
#include "../measurements/electricfield.h"
#include "../measurements/magneticfield.h"
#include "../measurements/measurements.h"
#include "../measurements/pulses.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/measurements.h"
#include "../ui/menu.h"
#include "../ui/system.h"

typedef enum
{
    ELECTRIC_FIELD_TAB_MAX,
    ELECTRIC_FIELD_TAB_MAGNETIC,

    ELECTRIC_FIELD_TAB_NUM,

    ELECTRIC_FIELD_TAB_ALERT = ELECTRIC_FIELD_TAB_NUM,
} ElectricFieldTab;

static void resetElectricField(void);

const Unit electricFieldUnits = {STRING_VOLT_PER_METER_UNIT, 1};

const int32_t electricFieldMinMetricPrefix = 0;

static struct
{
    float value;

    float maxValue;

    AlertLevel alertLevel;
} electricField;

static ElectricFieldTab electricFieldTab;

void setupElectricField(void)
{
    resetElectricField();

    if (isEMFMeterAvailable())
        electricFieldTab = ELECTRIC_FIELD_TAB_MAX;
    else
        electricFieldTab = ELECTRIC_FIELD_TAB_ALERT;
}

static void resetElectricField(void)
{
    memset(&electricField, 0, sizeof(electricField));

    if (electricFieldTab == ELECTRIC_FIELD_TAB_ALERT)
        electricFieldTab = ELECTRIC_FIELD_TAB_MAX;
}

void updateElectricField(void)
{
    // Value
    if (isEMFMeterAvailable())
    {
        electricField.value = readElectricFieldStrength();
        if (electricField.value > electricField.maxValue)
            electricField.maxValue = electricField.value;
    }

    // Alerts
    AlertLevel alertLevel = ALERTLEVEL_NONE;
    if (settings.electricFieldAlarm && (electricField.value >= electricFieldAlerts[settings.electricFieldAlarm]))
        alertLevel = ALERTLEVEL_ALARM;

    bool alertTriggered = (alertLevel > electricField.alertLevel);
    electricField.alertLevel = alertLevel;

    if (alertTriggered)
        setAlertPending(true);

    if (isTubeFaultAlertTriggered() || isEMFMeterUnavailableTriggered())
        electricFieldTab = ELECTRIC_FIELD_TAB_ALERT;
    else if (electricFieldTab == ELECTRIC_FIELD_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel() && isEMFMeterAvailable())
            electricFieldTab = ELECTRIC_FIELD_TAB_MAX;
    }
}

float getElectricField(void)
{
    return electricField.value;
}

AlertLevel getElectricFieldAlertLevel(void)
{
    return electricField.alertLevel;
}

// Electric field view

static void onElectricFieldViewEvent(Event event)
{
    if (onMeasurementViewEvent(event))
        return;

    const char *alertString;
    if (getTubeFaultAlertLevel())
        alertString = getString(STRING_ALERT_FAULT);
    else if (!isEMFMeterAvailable())
        alertString = getString(STRING_ALERT_UNAVAILABLE);
    else
        alertString = NULL;

    switch (event)
    {
    case EVENT_KEY_BACK:
        electricFieldTab++;
        if (alertString
                ? electricFieldTab >= (ELECTRIC_FIELD_TAB_NUM + 1)
                : electricFieldTab >= ELECTRIC_FIELD_TAB_NUM)
            electricFieldTab = ELECTRIC_FIELD_TAB_MAX;

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetElectricField();

        requestViewUpdate();

        break;

    case EVENT_DRAW:
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString, unitString, electricField.value, &electricFieldUnits, electricFieldMinMetricPrefix);

        if (electricField.alertLevel == ALERTLEVEL_ALARM)
            style = MEASUREMENTSTYLE_ALARM;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_ELECTRIC_FIELD));
        drawMeasurementValue(valueString, unitString, 0, style);

        if (electricFieldTab == ELECTRIC_FIELD_TAB_ALERT)
            drawMeasurementAlert(alertString);
        else
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

            switch (electricFieldTab)
            {
            case ELECTRIC_FIELD_TAB_MAX:
                if (electricField.maxValue != 0)
                    buildValueString(valueString, unitString, electricField.maxValue, &electricFieldUnits, electricFieldMinMetricPrefix);

                keyString = getString(STRING_MAX);

                break;

            case ELECTRIC_FIELD_TAB_MAGNETIC:
            {
                float magneticField = getMagneticField();
                if (magneticField)
                    buildValueString(valueString, unitString, magneticField, &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);

                keyString = getString(STRING_MAGNETIC_FIELD);

                break;
            }

            default:
                break;
            }

            drawMeasurementInfo(keyString, valueString, unitString, MEASUREMENTSTYLE_NORMAL);

            break;
        }
    }

    default:
        break;
    }
}

View electricFieldView = {
    onElectricFieldViewEvent,
    NULL,
};

// Electric field alarm view

const float electricFieldAlerts[] = {
    0,
    50,
    100,
    200,
    500,
    1000,
    2000,
    5000,
};

static const char *onElectricFieldAlarmMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.electricFieldAlarm);

    if (index == 0)
        return getString(STRING_OFF);
    else if (index < ELECTRIC_FIELD_NUM)
    {
        char unitString[16];

        strclr(menuOption);
        strcpy(unitString, " ");

        buildValueString(menuOption, unitString, electricFieldAlerts[index], &electricFieldUnits, electricFieldMinMetricPrefix);
        strcat(menuOption, unitString);

        return menuOption;
    }
    else
        return NULL;
}

static void onElectricFieldAlarmMenuSelect(uint32_t index)
{
    settings.electricFieldAlarm = index;
}

static MenuState electricFieldAlarmMenuState;

static Menu electricFieldAlarmMenu = {
    STRING_ELECTRIC_FIELD_ALARM,
    &electricFieldAlarmMenuState,
    onElectricFieldAlarmMenuGetOption,
    onElectricFieldAlarmMenuSelect,
    setAlertsMenu,
};

View electricFieldAlarmMenuView = {
    onMenuEvent,
    &electricFieldAlarmMenu,
};

#endif
