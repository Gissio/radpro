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

void resetMagneticField(void)
{
    magneticFieldTab = MAGNETIC_FIELD_TAB_MAX;

    clearMagneticField();

    selectMenuItem(&magneticFieldUnitsMenu, settings.magneticFieldUnits);
}

void clearMagneticField(void)
{
    memset(&magneticField, 0, sizeof(magneticField));
}

void updateMagneticField(void)
{
    // Update magnetic field strength
    magneticField.value = readMagneticFieldStrength();

    if (magneticField.value > magneticField.maxValue)
        magneticField.maxValue = magneticField.value;

    updateMeasurementAlertLevel(&magneticField.alertLevel,
                                getMeasurementAlertLevel(magneticField.value, magneticFieldAlerts, 0, settings.magneticFieldAlarm));
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

void advanceMagneticFieldTab(void)
{
    magneticFieldTab = getNextMeasurementTab(magneticFieldTab, MAGNETIC_FIELD_TAB_NUM);
}

void drawMagneticFieldView(void)
{
    const Unit *unit = &magneticFieldUnits[settings.magneticFieldUnits];
    int32_t minMetricPrefix = magneticFieldMinMetricPrefix[settings.magneticFieldUnits];

    drawMeasurementValueWithUnit(getString(STRING_MAGNETIC_FIELD), magneticField.value, unit, minMetricPrefix, 0, getAlertMeasurementStyle(magneticField.alertLevel));

    if (magneticFieldTab == MAGNETIC_FIELD_TAB_MAX)
        drawMeasurementInfoValue(getString(STRING_MAX), magneticField.maxValue, unit, minMetricPrefix, MEASUREMENTSTYLE_NORMAL);
    else
        drawMeasurementInfoValue(getString(STRING_ELECTRIC_FIELD), getElectricField(), &electricFieldUnits, electricFieldMinMetricPrefix, MEASUREMENTSTYLE_NORMAL);
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

    return buildAlertMenuOption(index, magneticFieldAlerts[index], &magneticFieldUnits[settings.magneticFieldUnits], magneticFieldMinMetricPrefix[settings.magneticFieldUnits]);
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
