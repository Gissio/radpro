/*
 * Rad Pro
 * Magnetic field
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(EMFMETER)

#include "../measurements/electricfield.h"
#include "../measurements/magneticfield.h"
#include "../measurements/pulses.h"
#include "../peripherals/adc.h"
#include "../peripherals/emf.h"
#include "../system/cmath.h"
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

    MAGNETIC_FIELD_TAB_NUM,
} MagneticFieldTab;

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
    magneticFieldTab = MAGNETIC_FIELD_TAB_MAX;

    resetMagneticField();

    selectMenuItem(&magneticFieldUnitsMenu, settings.magneticFieldUnits);
}

static void resetMagneticField(void)
{
    memset(&magneticField, 0, sizeof(magneticField));
}

static void updateMagneticFieldAlerts(void)
{
    AlertLevel alertLevel = ALERTLEVEL_NONE;
    if (settings.magneticFieldAlarm && (magneticField.value >= magneticFieldAlerts[settings.magneticFieldAlarm]))
        alertLevel = ALERTLEVEL_ALARM;

    AlertLevel previousAlertLevel = magneticField.alertLevel;
    magneticField.alertLevel = alertLevel;

    bool alertTriggered = (alertLevel > previousAlertLevel);
    if (alertTriggered)
        setAlertPending(true);
}

void updateMagneticField(void)
{
    // Update magnetic field strength
    magneticField.value = readMagneticFieldStrength();

    if (magneticField.value > magneticField.maxValue)
        magneticField.maxValue = magneticField.value;

    updateMagneticFieldAlerts();
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

static void advanceMagneticFieldTab(void)
{
    magneticFieldTab++;
    if (magneticFieldTab >= MAGNETIC_FIELD_TAB_NUM)
        magneticFieldTab = MAGNETIC_FIELD_TAB_MAX;
}

static MeasurementStyle getMagneticFieldMeasurementStyle(void)
{
    if (magneticField.alertLevel == ALERTLEVEL_ALARM)
        return MEASUREMENTSTYLE_ALARM;

    return MEASUREMENTSTYLE_NORMAL;
}
static void drawMagneticFieldValue(void)
{
    char valueString[32] = "";
    char unitString[32] = "";
    buildValueString(valueString, unitString, magneticField.value, &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);

    drawTitleBar(getString(STRING_MAGNETIC_FIELD));
    drawMeasurementValue(valueString, unitString, 0, getMagneticFieldMeasurementStyle());
}

static void drawMagneticFieldTab(void)
{
    char valueString[32] = "";
    char unitString[32] = " ";
    const char *keyString = NULL;

    switch (magneticFieldTab)
    {
    case MAGNETIC_FIELD_TAB_MAX:
        buildValueString(valueString, unitString, magneticField.maxValue, &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);

        keyString = getString(STRING_MAX);

        break;

    case MAGNETIC_FIELD_TAB_ELECTRIC:
    {
        buildValueString(valueString, unitString, getElectricField(), &electricFieldUnits, electricFieldMinMetricPrefix);

        keyString = getString(STRING_ELECTRIC_FIELD);

        break;
    }

    default:
        break;
    }

    drawMeasurementInfo(keyString, valueString, unitString, MEASUREMENTSTYLE_NORMAL);
}

static void drawMagneticFieldView(void)
{
    drawMagneticFieldValue();
    drawMagneticFieldTab();
}

void onMagneticFieldViewEvent(ViewEvent event)
{
    if (onMeasurementViewEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        advanceMagneticFieldTab();

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetMagneticField();

        requestViewUpdate();

        break;

    case EVENT_DRAW:
        drawMagneticFieldView();

        break;

    default:
        break;
    }
}

// Magnetic field units menu

static cstring magneticFieldUnitsMenuOptions[] = {
    STRING_TESLA,
    STRING_GAUSS,
};

static const char *onMagneticFieldUnitsMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.magneticFieldUnits);

    return getString(magneticFieldUnitsMenuOptions[index]);
}

static void onMagneticFieldUnitsMenuSelect(menu_size_t index)
{
    settings.magneticFieldUnits = index;
}

static MenuState magneticFieldUnitsMenuState;

const Menu magneticFieldUnitsMenu = {
    STRING_MAGNETIC_FIELD_UNITS,
    &magneticFieldUnitsMenuState,
    ARRAY_SIZE(magneticFieldUnitsMenuOptions),
    onMagneticFieldUnitsMenuGetOption,
    onMagneticFieldUnitsMenuSelect,
    showMeasurementsMenu,
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

static const char *onMagneticFieldAlarmMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.magneticFieldAlarm);

    if (index == 0)
        return getString(STRING_OFF);

    char unitString[32] = " ";
    strclr(menuOption);
    buildValueString(menuOption, unitString, magneticFieldAlerts[index], &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);
    strcat(menuOption, unitString);

    return menuOption;
}

static void onMagneticFieldAlarmMenuSelect(menu_size_t index)
{
    settings.magneticFieldAlarm = index;
}

static MenuState magneticFieldAlarmMenuState;

const Menu magneticFieldAlarmMenu = {
    STRING_MAGNETIC_FIELD_ALARM,
    &magneticFieldAlarmMenuState,
    ARRAY_SIZE(magneticFieldAlerts),
    onMagneticFieldAlarmMenuGetOption,
    onMagneticFieldAlarmMenuSelect,
    showAlertsMenu,
};

#endif
