/*
 * Rad Pro
 * Electric field
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(EMFMETER)

#include "../measurements/electricfield.h"
#include "../measurements/magneticfield.h"
#include "../measurements/measurements.h"
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
    ELECTRIC_FIELD_TAB_MAX,
    ELECTRIC_FIELD_TAB_MAGNETIC,

    ELECTRIC_FIELD_TAB_NUM,
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
    electricFieldTab = ELECTRIC_FIELD_TAB_MAX;

    resetElectricField();
}

static void resetElectricField(void)
{
    memset(&electricField, 0, sizeof(electricField));
}

static void updateElectricFieldAlerts(void)
{
    AlertLevel alertLevel = ALERTLEVEL_NONE;
    if (settings.electricFieldAlarm && (electricField.value >= electricFieldAlerts[settings.electricFieldAlarm]))
        alertLevel = ALERTLEVEL_ALARM;

    AlertLevel previousAlertLevel = electricField.alertLevel;
    electricField.alertLevel = alertLevel;

    bool alertTriggered = (alertLevel > previousAlertLevel);
    if (alertTriggered)
        setAlertPending(true);
}

void updateElectricField(void)
{
    // Update electric field strength
    electricField.value = readElectricFieldStrength();

    if (electricField.value > electricField.maxValue)
        electricField.maxValue = electricField.value;

    updateElectricFieldAlerts();
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

static void advanceElectricFieldTab(void)
{
    electricFieldTab++;
    if (electricFieldTab >= ELECTRIC_FIELD_TAB_NUM)
        electricFieldTab = ELECTRIC_FIELD_TAB_MAX;
}

static MeasurementStyle getElectricFieldMeasurementStyle(void)
{
    if (electricField.alertLevel == ALERTLEVEL_ALARM)
        return MEASUREMENTSTYLE_ALARM;

    return MEASUREMENTSTYLE_NORMAL;
}

static void drawElectricFieldValue(void)
{
    char valueString[32] = "";
    char unitString[32] = "";
    buildValueString(valueString, unitString, electricField.value, &electricFieldUnits, electricFieldMinMetricPrefix);

    drawTitleBar(getString(STRING_ELECTRIC_FIELD));
    drawMeasurementValue(valueString, unitString, 0, getElectricFieldMeasurementStyle());
}

static void drawElectricFieldTab(void)
{
    char valueString[32] = "";
    char unitString[32] = " ";
    const char *keyString = NULL;

    switch (electricFieldTab)
    {
    case ELECTRIC_FIELD_TAB_MAX:
        buildValueString(valueString, unitString, electricField.maxValue, &electricFieldUnits, electricFieldMinMetricPrefix);

        keyString = getString(STRING_MAX);

        break;

    case ELECTRIC_FIELD_TAB_MAGNETIC:
    {
        buildValueString(valueString, unitString, getMagneticField(), &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);

        keyString = getString(STRING_MAGNETIC_FIELD);

        break;
    }

    default:
        break;
    }

    drawMeasurementInfo(keyString, valueString, unitString, MEASUREMENTSTYLE_NORMAL);
}

static void drawElectricFieldView(void)
{
    drawElectricFieldValue();
    drawElectricFieldTab();
}

void onElectricFieldViewEvent(ViewEvent event)
{
    if (onMeasurementViewEvent(event))
        return;

    switch (event)
    {
    case EVENT_KEY_BACK:
        advanceElectricFieldTab();

        requestViewUpdate();

        break;

    case EVENT_KEY_RESET:
        triggerVibration();

        resetElectricField();

        requestViewUpdate();

        break;

    case EVENT_DRAW:
        drawElectricFieldView();

        break;

    default:
        break;
    }
}

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

static const char *onElectricFieldAlarmMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.electricFieldAlarm);

    if (index == 0)
        return getString(STRING_OFF);

    char unitString[32] = " ";
    strclr(menuOption);
    buildValueString(menuOption, unitString, electricFieldAlerts[index], &electricFieldUnits, electricFieldMinMetricPrefix);
    strcat(menuOption, unitString);

    return menuOption;
}

static void onElectricFieldAlarmMenuSelect(menu_size_t index)
{
    settings.electricFieldAlarm = index;
}

static MenuState electricFieldAlarmMenuState;

const Menu electricFieldAlarmMenu = {
    STRING_ELECTRIC_FIELD_ALARM,
    &electricFieldAlarmMenuState,
    ARRAY_SIZE(electricFieldAlerts),
    onElectricFieldAlarmMenuGetOption,
    onElectricFieldAlarmMenuSelect,
    showAlertsMenu,
};

#endif
