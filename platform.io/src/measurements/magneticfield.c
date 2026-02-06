/*
 * Rad Pro
 * Magnetic field
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
#include "../measurements/pulses.h"
#include "../system/events.h"
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/measurements.h"
#include "../ui/menu.h"
#include "../ui/system.h"

typedef enum
{
    MAGNETIC_FIELD_TAB_MAX,
    MAGNETIC_FIELD_TAB_ELECTRIC,
    MAGNETIC_FIELD_TAB_POWERDENSITY,

    MAGNETIC_FIELD_TAB_NUM,

    MAGNETIC_FIELD_TAB_ALERT = MAGNETIC_FIELD_TAB_NUM,
} MagneticFieldTab;

static Menu magneticFieldUnitsMenu;

static void resetMagneticField(void);

const Unit magneticFieldUnits[] = {
    {STRING_TESLA_UNIT, 1},
    {STRING_GAUSS_UNIT, 1E4F},
};

const int32_t magneticFieldMinMetricPrefix[] = {-2, -1};

static struct
{
    float value;

    float maxValue;

    AlertLevel alertLevel;
} magneticField;

static MagneticFieldTab magneticFieldTab;

void setupMagneticField(void)
{
    resetMagneticField();

    if (isEMFMeterAvailable())
        magneticFieldTab = MAGNETIC_FIELD_TAB_MAX;
    else
        magneticFieldTab = MAGNETIC_FIELD_TAB_ALERT;

    selectMenuItem(&magneticFieldUnitsMenu, settings.magneticFieldUnits, MAGNETIC_FIELD_UNITS_NUM);
}

static void resetMagneticField(void)
{
    memset(&magneticField, 0, sizeof(magneticField));

    if (magneticFieldTab == MAGNETIC_FIELD_TAB_ALERT)
        magneticFieldTab = MAGNETIC_FIELD_TAB_MAX;
}

void updateMagneticField(void)
{
    // Value
    if (isEMFMeterAvailable())
    {
        magneticField.value = readMagneticFieldStrength();
        if (magneticField.value > magneticField.maxValue)
            magneticField.maxValue = magneticField.value;
    }

    // Alerts
    AlertLevel alertLevel = ALERTLEVEL_NONE;
    if (settings.magneticFieldAlarm && (magneticField.value >= magneticFieldAlerts[settings.magneticFieldAlarm]))
        alertLevel = ALERTLEVEL_ALARM;

    bool alertTriggered = (alertLevel > magneticField.alertLevel);
    magneticField.alertLevel = alertLevel;

    if (alertTriggered)
        setAlertPending(true);

    if (isTubeFaultAlertTriggered() || isEMFMeterUnavailableTriggered())
        magneticFieldTab = MAGNETIC_FIELD_TAB_ALERT;
    else if (magneticFieldTab == MAGNETIC_FIELD_TAB_ALERT)
    {
        if (!getTubeFaultAlertLevel() && isEMFMeterAvailable())
            magneticFieldTab = MAGNETIC_FIELD_TAB_MAX;
    }
}

float getMagneticField(void)
{
    return magneticField.value;
}

AlertLevel getMagneticFieldAlertLevel(void)
{
    return magneticField.alertLevel;
}

// Magnetic field view

static void onMagneticFieldViewEvent(Event event)
{
    if (onMeasurementViewEvent(event))
        return;

    const char *alertString;
    if (getTubeFaultAlertLevel() > ALERTLEVEL_NONE)
        alertString = getString(STRING_ALERT_FAULT);
    else if (!isEMFMeterAvailable())
        alertString = getString(STRING_ALERT_UNAVAILABLE);
    else
        alertString = NULL;

    switch (event)
    {
    case EVENT_KEY_BACK:
        magneticFieldTab++;
        if (alertString
                ? magneticFieldTab >= (MAGNETIC_FIELD_TAB_NUM + 1)
                : magneticFieldTab >= MAGNETIC_FIELD_TAB_NUM)
            magneticFieldTab = MAGNETIC_FIELD_TAB_MAX;

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetMagneticField();

        requestViewUpdate();

        break;

    case EVENT_DRAW:
    {
        char valueString[32];
        char unitString[32];
        MeasurementStyle style;

        strclr(valueString);
        strclr(unitString);
        buildValueString(valueString, unitString, magneticField.value, &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);

        if (magneticField.alertLevel == ALERTLEVEL_ALARM)
            style = MEASUREMENTSTYLE_ALARM;
        else
            style = MEASUREMENTSTYLE_NORMAL;

        drawTitleBar(getString(STRING_MAGNETIC_FIELD));
        drawMeasurementValue(valueString, unitString, 0, style);

        if (magneticFieldTab == MAGNETIC_FIELD_TAB_ALERT)
            drawMeasurementAlert(alertString);
        else
        {
            const char *keyString = NULL;
            strclr(valueString);
            strcpy(unitString, " ");

            switch (magneticFieldTab)
            {
            case MAGNETIC_FIELD_TAB_MAX:
                if (magneticField.maxValue != 0)
                    buildValueString(valueString, unitString, magneticField.maxValue, &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);

                keyString = getString(STRING_MAX);

                break;

            case MAGNETIC_FIELD_TAB_ELECTRIC:
            {
                float electricField = getElectricField();
                if (electricField)
                    buildValueString(valueString, unitString, electricField, &electricFieldUnits, electricFieldMinMetricPrefix);

                keyString = getString(STRING_ELECTRIC_FIELD);

                break;
            }

            case MAGNETIC_FIELD_TAB_POWERDENSITY:
            {
                float powerDensity = getPowerDensity();
                if (powerDensity)
                    buildValueString(valueString, unitString, powerDensity, &powerDensityUnits, powerDensityMinMetricPrefix);

                keyString = getString(STRING_POWER_DENSITY);

                break;
            }

            default:
                break;
            }

            drawMeasurementInfo(keyString, valueString, unitString, MEASUREMENTSTYLE_NORMAL);
        }

        break;
    }

    default:
        break;
    }
}

View magneticFieldView = {
    onMagneticFieldViewEvent,
    NULL,
};

// Magnetic field units menu

static cstring magneticFieldUnitsMenuOptions[] = {
    STRING_TESLA,
    STRING_GAUSS,
    NULL,
};

static const char *onMagneticFieldUnitsMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.magneticFieldUnits);

    return getString(magneticFieldUnitsMenuOptions[index]);
}

static void onMagneticFieldUnitsMenuSelect(uint32_t index)
{
    settings.magneticFieldUnits = index;
}

static MenuState magneticFieldUnitsMenuState;

static Menu magneticFieldUnitsMenu = {
    STRING_MAGNETIC_FIELD_UNITS,
    &magneticFieldUnitsMenuState,
    onMagneticFieldUnitsMenuGetOption,
    onMagneticFieldUnitsMenuSelect,
    setMeasurementsMenu,
};

View magneticFieldUnitsMenuView = {
    onMenuEvent,
    &magneticFieldUnitsMenu,
};

// Magnetic field alarm view

const float magneticFieldAlerts[] = {
    0,
    0.2E-6F,
    0.5E-6F,
    1E-6F,
    2E-6F,
    5E-6F,
    10E-6F,
    20E-6F,
};

static const char *onMagneticFieldAlarmMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.magneticFieldAlarm);

    if (index == 0)
        return getString(STRING_OFF);
    else if (index < MAGNETIC_FIELD_NUM)
    {
        char unitString[16];

        strclr(menuOption);
        strcpy(unitString, " ");

        buildValueString(menuOption, unitString, magneticFieldAlerts[index], &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);
        strcat(menuOption, unitString);

        return menuOption;
    }
    else
        return NULL;
}

static void onMagneticFieldAlarmMenuSelect(uint32_t index)
{
    settings.magneticFieldAlarm = index;
}

static MenuState magneticFieldAlarmMenuState;

static Menu magneticFieldAlarmMenu = {
    STRING_MAGNETIC_FIELD_ALARM,
    &magneticFieldAlarmMenuState,
    onMagneticFieldAlarmMenuGetOption,
    onMagneticFieldAlarmMenuSelect,
    setAlertsMenu,
};

View magneticFieldAlarmMenuView = {
    onMenuEvent,
    &magneticFieldAlarmMenu,
};

#endif
