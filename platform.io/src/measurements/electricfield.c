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

const Unit electricFieldUnits = {STRING_VOLT_PER_METER_UNIT, 1};

const int32_t electricFieldMinMetricPrefix = 0;

static struct
{
    float value;

    float maxValue;

    AlertLevel alertLevel;
} electricField;

static ElectricFieldTab electricFieldTab;

void resetElectricField(void)
{
    electricFieldTab = ELECTRIC_FIELD_TAB_MAX;

    clearElectricField();
}

void clearElectricField(void)
{
    memset(&electricField, 0, sizeof(electricField));
}

void updateElectricField(void)
{
    float value = calculateFieldStrength(getElectricFieldSamples(), &electricFieldSensor);
    if (value < 1E-6F)
        value = 1E-6F;

    electricField.value = value;
    if (value > electricField.maxValue)
        electricField.maxValue = value;

    updateFieldAlertLevel(&electricField.alertLevel, value, electricFieldAlerts, settings.electricFieldAlarm);
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

void advanceElectricFieldTab(void)
{
    electricFieldTab = getNextMeasurementTab(electricFieldTab, ELECTRIC_FIELD_TAB_NUM);
}

void drawElectricFieldView(void)
{
    drawMeasurementValueWithUnit(getString(STRING_ELECTRIC_FIELD), electricField.value, &electricFieldUnits, electricFieldMinMetricPrefix, 0, getAlertMeasurementStyle(electricField.alertLevel));

    if (electricFieldTab == ELECTRIC_FIELD_TAB_MAX)
        drawMeasurementInfoValue(getString(STRING_MAX), electricField.maxValue, &electricFieldUnits, electricFieldMinMetricPrefix, MEASUREMENTSTYLE_NORMAL);
    else
        drawMeasurementInfoValue(getString(STRING_MAGNETIC_FIELD), getMagneticField(), &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits], MEASUREMENTSTYLE_NORMAL);
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

    return buildAlertMenuOption(index, electricFieldAlerts[index], &electricFieldUnits, electricFieldMinMetricPrefix);
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
