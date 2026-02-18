/*
 * Rad Pro
 * Measurements
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <float.h>

#include "../measurements/average.h"
#include "../measurements/cumulative.h"
#include "../measurements/electricfield.h"
#include "../measurements/history.h"
#include "../measurements/instantaneous.h"
#include "../measurements/magneticfield.h"
#include "../peripherals/display.h"
#include "../peripherals/emf.h"
#include "../peripherals/keyboard.h"
#include "../peripherals/led.h"
#include "../peripherals/pulsesoundenable.h"
#include "../peripherals/tube.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/menu.h"

typedef enum
{
    MEASUREMENTS_VIEW_INSTANTANEOUS_RATE,
    MEASUREMENTS_VIEW_AVERAGE_RATE,
    MEASUREMENTS_VIEW_CUMULATIVE_DOSE,
#if defined(EMFMETER)
    MEASUREMENTS_VIEW_ELECTRIC_FIELD,
    MEASUREMENTS_VIEW_MAGNETIC_FIELD,
#endif
    MEASUREMENTS_VIEW_HISTORY,

    MEASUREMENTS_VIEW_NUM,
} MeasurementView;

static struct
{
    bool enabled;

    int32_t viewIndex;

    AlertLevel alertLevel;
    bool alertPending;
    bool alertFlashing;
    bool soundIconActive;
} measurements;

static const Menu alertsMenu;
static const Menu alertIndicationMenu;

static const Menu measurementsMenu;

// Measurements

static OnViewEvent *const measurementViews[] = {
    onInstantaneousRateViewEvent,
    onAverageRateViewEvent,
    onCumulativeDoseViewEvent,
#if defined(EMFMETER)
    onElectricFieldViewEvent,
    onMagneticFieldViewEvent,
#endif
    onHistoryViewEvent,
};

static void setMeasurementViewSelect(MeasurementView index);

void setupMeasurements(void)
{
    measurements.viewIndex = MEASUREMENTS_VIEW_INSTANTANEOUS_RATE;

    setupPulses();

#if defined(EMFMETER)
    setupElectricField();
    setupMagneticField();
#endif

    selectMenuItem(&alertsMenu, 0);
    selectMenuItem(&alertIndicationMenu, 0);

    selectMenuItem(&measurementsMenu, 0);
}

void setMeasurementsEnabled(bool value)
{
    measurements.enabled = value;

    setTubeHVEnabled(value);
#if defined(EMFMETER)
    setEMFMeterEnabled(value);
#endif
}

bool isMeasurementsEnabled(void)
{
    return measurements.enabled;
}

static AlertLevel getMaxAlertLevel(AlertLevel a, AlertLevel b)
{
    return (b > a) ? b : a;
}

void updateMeasurements(void)
{
    if (!measurements.enabled)
        return;

    // Measurements
    updatePulses();
#if defined(EMFMETER)
    updateEMFMeter();
#endif

    // Alerts
    AlertLevel alertLevel = getTubeFaultAlertLevel();
    alertLevel = getMaxAlertLevel(alertLevel, getInstantaneousRateAlertLevel());
    alertLevel = getMaxAlertLevel(alertLevel, getCumulativeDoseAlertLevel());
#if defined(EMFMETER)
    alertLevel = getMaxAlertLevel(alertLevel, getElectricFieldAlertLevel());
    alertLevel = getMaxAlertLevel(alertLevel, getMagneticFieldAlertLevel());
#endif

    if (alertLevel)
    {
        if (measurements.alertPending)
            triggerAlert(alertLevel == ALERTLEVEL_ALARM);

        bool alertTriggered = (alertLevel && !measurements.alertLevel);
        measurements.alertFlashing = alertTriggered ? true : !measurements.alertFlashing;
    }
    else
    {
        measurements.alertPending = false;

        measurements.alertFlashing = false;
    }

    measurements.alertLevel = alertLevel;

    // Sound control
#if defined(PULSESOUND_ENABLE)
    updatePulseSoundEnable();
#endif

    // LED
#if defined(PULSE_LED) || defined(PULSE_LED_EN)
    updateLED();
#endif

    // Set view
    if (!isDisplayEnabled() && measurements.alertPending)
    {
        if (getInstantaneousRateAlertLevel())
            setMeasurementViewSelect(MEASUREMENTS_VIEW_INSTANTANEOUS_RATE);
        else if (getCumulativeDoseAlertLevel())
            setMeasurementViewSelect(MEASUREMENTS_VIEW_CUMULATIVE_DOSE);
#if defined(EMFMETER)
        else if (getElectricFieldAlertLevel())
            setMeasurementViewSelect(MEASUREMENTS_VIEW_ELECTRIC_FIELD);
        else if (getMagneticFieldAlertLevel())
            setMeasurementViewSelect(MEASUREMENTS_VIEW_MAGNETIC_FIELD);
#endif
    }
}

bool isAlertEnabled(void)
{
    return (settings.rateAlarm ||
            settings.doseAlarm ||
            settings.rateWarning ||
            settings.doseWarning
#if defined(EMFMETER)
            ||
            settings.electricFieldAlarm ||
            settings.magneticFieldAlarm
#endif
    );
}

AlertLevel getAlertLevel(void)
{
    return measurements.alertLevel;
}

void setAlertPending(bool value)
{
    measurements.alertPending = value;
}

bool isAlertPending(void)
{
    return measurements.alertPending;
}

bool isAlertFlashing(void)
{
    return measurements.alertFlashing;
}

bool isSoundIconActive(void)
{
    return measurements.soundIconActive;
}

// Measurement views common

void buildValueString(char *valueString, char *unitString, float value, const Unit *unit, int32_t minMetricPrefixIndex)
{
    if (!unit->name[0] && (value < 10000.0F))
    {
        uint32_t intValue = (uint32_t)value;

        strcatUInt32(valueString, intValue, 0);
        strcpy(unitString, " ");
        if (intValue == 1)
            strcat(unitString, getString(STRING_COUNT));
        else
            strcat(unitString, getString(STRING_COUNTS));
    }
    else
    {
        strcatMetricValue(valueString, unitString, value * unit->scale, minMetricPrefixIndex);

        strcat(unitString, unit->name[0] ? unit->name : getString(STRING_COUNTS));
    }
}

static void setMeasurementViewSelect(MeasurementView index)
{
    measurements.viewIndex = index;

    setMeasurementView();
}

void setMeasurementView(void)
{
    setKeyboardMode(KEYBOARD_MODE_MEASUREMENT);

    showView(measurementViews[measurements.viewIndex]);
}

bool onMeasurementViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_SELECT:
        if (!isInLockMode())
        {
            measurements.soundIconActive = false;

            setKeyboardMode(KEYBOARD_MODE_MENU);

            showSettingsMenu();
        }

        break;

    case EVENT_KEY_UP:
        measurements.viewIndex--;
        if (measurements.viewIndex < 0)
            measurements.viewIndex = MEASUREMENTS_VIEW_NUM - 1;

        setMeasurementView();

        break;

    case EVENT_KEY_DOWN:
        measurements.viewIndex++;
        if (measurements.viewIndex >= MEASUREMENTS_VIEW_NUM)
            measurements.viewIndex = 0;

        setMeasurementView();

        break;

    case EVENT_KEY_RESET:
        if (measurements.alertPending)
            measurements.alertPending = false;
        else
            return isInLockMode();

        break;

    case EVENT_KEY_TOGGLEPULSESOUND:
        measurements.soundIconActive = true;

        settings.pulseSound = !settings.pulseSound;

#if defined(PULSESOUND_ENABLE)
        updatePulseSoundEnable();
#endif

        triggerVibration();

        break;

    default:
        return false;
    }

    return true;
}

// Alert indication menu

enum
{
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    ALERTINDICATION_MENU_SOUND,
#endif
#if defined(VOICE)
    ALERTINDICATION_MENU_VOICE,
#endif
#if defined(VIBRATOR)
    ALERTINDICATION_MENU_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    ALERTINDICATION_MENU_PULSELED,
#endif
    ALERTINDICATION_MENU_DISPLAYFLASH,
};

static cstring alertIndicationMenuOptions[] = {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    STRING_SOUND,
#endif
#if defined(VOICE)
    STRING_VOICE,
#endif
#if defined(VIBRATOR)
    STRING_VIBRATION,
#endif
#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    STRING_PULSE_LED,
#endif
    STRING_DISPLAY_FLASH,
};

static const char *onAlertIndicationMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    switch (index)
    {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    case ALERTINDICATION_MENU_SOUND:
        *menuStyle = settings.alertSound;

        break;
#endif

#if defined(VOICE)
    case ALERTINDICATION_MENU_VOICE:
        *menuStyle = settings.alertVoice;

        break;
#endif

#if defined(VIBRATOR)
    case ALERTINDICATION_MENU_VIBRATION:
        *menuStyle = settings.alertVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    case ALERTINDICATION_MENU_PULSELED:
        *menuStyle = settings.alertPulseLED;

        break;
#endif

    case ALERTINDICATION_MENU_DISPLAYFLASH:
        *menuStyle = settings.alertDisplayFlash;

        break;
    }

    return getString(alertIndicationMenuOptions[index]);
}

static void onAlertIndicationMenuSelect(menu_size_t index)
{
    switch (index)
    {
#if defined(BUZZER) || defined(PULSESOUND_ENABLE)
    case ALERTINDICATION_MENU_SOUND:
        settings.alertSound = !settings.alertSound;

        break;
#endif

#if defined(VOICE)
    case ALERTINDICATION_MENU_VOICE:
        settings.alertVoice = !settings.alertVoice;

        break;
#endif

#if defined(VIBRATOR)
    case ALERTINDICATION_MENU_VIBRATION:
        settings.alertVibration = !settings.alertVibration;

        break;
#endif

#if defined(PULSE_LED) || defined(ALERT_LED) || defined(PULSE_LED_EN) || defined(ALERT_LED_EN)
    case ALERTINDICATION_MENU_PULSELED:
        settings.alertPulseLED = !settings.alertPulseLED;

        updateLED();

        break;
#endif

    case ALERTINDICATION_MENU_DISPLAYFLASH:
        settings.alertDisplayFlash = !settings.alertDisplayFlash;

        break;
    }
}

static MenuState alertIndicationMenuState;

static const Menu alertIndicationMenu = {
    STRING_INDICATION,
    &alertIndicationMenuState,
    ARRAY_SIZE(alertIndicationMenuOptions),
    onAlertIndicationMenuGetOption,
    onAlertIndicationMenuSelect,
    showAlertsMenu,
};

// Alerts menu

static const MenuOption alertsMenuOptions[] = {
    {STRING_INDICATION, &alertIndicationMenu},
    {STRING_RATE_WARNING, &rateWarningMenu},
    {STRING_RATE_ALARM, &rateAlarmMenu},
    {STRING_DOSE_WARNING, &doseWarningMenu},
    {STRING_DOSE_ALARM, &doseAlarmMenu},
#if defined(EMFMETER)
    {STRING_ELECTRIC_FIELD_ALARM, &electricFieldAlarmMenu},
    {STRING_MAGNETIC_FIELD_ALARM, &magneticFieldAlarmMenu},
#endif
};

static const char *onAlertsMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(alertsMenuOptions[index].title);
}

static void onAlertsMenuSelect(menu_size_t index)
{
    showMenu(alertsMenuOptions[index].menu);
}

static MenuState alertsMenuState;

static const Menu alertsMenu = {
    STRING_ALERTS,
    &alertsMenuState,
    ARRAY_SIZE(alertsMenuOptions),
    onAlertsMenuGetOption,
    onAlertsMenuSelect,
    showSettingsMenu,
};

void showAlertsMenu(void)
{
    showMenu(&alertsMenu);
}

// Measurements menu

static const MenuOption measurementsMenuOptions[] = {
    {STRING_SOURCE, &sourceMenu},
    {STRING_DOSE_UNITS, &doseUnitsMenu},
    {STRING_SECONDARY_DOSE_UNITS, &secondaryDoseUnitsMenu},
#if defined(EMFMETER)
    {STRING_MAGNETIC_FIELD_UNITS, &magneticFieldUnitsMenu},
#endif
    {STRING_INSTANTANEOUS, &instantaneousMenu},
    {STRING_AVERAGE, &averageMenu},
};

static const char *onMeasurementsMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(measurementsMenuOptions[index].title);
}

static void onMeasurementsMenuSelect(menu_size_t index)
{
    showMenu(measurementsMenuOptions[index].menu);
}

static MenuState measurementsMenuState;

static const Menu measurementsMenu = {
    STRING_MEASUREMENTS,
    &measurementsMenuState,
    ARRAY_SIZE(measurementsMenuOptions),
    onMeasurementsMenuGetOption,
    onMeasurementsMenuSelect,
    showSettingsMenu,
};

void showMeasurementsMenu(void)
{
    showMenu(&measurementsMenu);
}
