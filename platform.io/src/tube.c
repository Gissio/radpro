/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#include "buzzer.h"
#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "measurements.h"
#include "pulseled.h"
#include "settings.h"
#include "tube.h"
#include "vibrator.h"

static const Menu tubeConversionFactorMenu;
static const Menu tubeDeadTimeCompensationMenu;
static const Menu tubeBackgroundCompensationMenu;
static const Menu tubeHVProfileMenu;
static const Menu tubeHVCustomProfileMenu;
static const Menu tubeHVDutyCycleMenu;
static const Menu tubeHVFrequencyMenu;
static const Menu tubeInstantaneousAveragingMenu;

static const View tubeConversionFactorMenuView;
static const View tubeDeadTimeCompensationMenuView;
static const View tubeBackgroundCompensationMenuView;
static const View tubeHVProfileMenuView;
static const View tubeHVCustomProfileWarningView;
static const View tubeHVCustomProfileMenuView;
static const View tubeHVDutyCycleMenuView;
static const View tubeHVrequencyMenuView;
static const View tubeInstantaneousAveragingMenuView;

void initTube(void)
{
    initTubeController();

    selectMenuItem(&tubeConversionFactorMenu,
                   settings.tubeConversionFactor,
                   TUBE_CONVERSIONFACTOR_NUM);
    selectMenuItem(&tubeInstantaneousAveragingMenu,
                   settings.tubeInstantaneousAveraging,
                   TUBE_INSTANTANEOUSAVERAGING_NUM);
    selectMenuItem(&tubeDeadTimeCompensationMenu,
                   settings.tubeDeadTimeCompensation,
                   TUBE_DEADTIMECOMPENSATION_NUM);
    selectMenuItem(&tubeBackgroundCompensationMenu,
                   settings.tubeBackgroundCompensation,
                   TUBE_BACKGROUNDCOMPENSATION_NUM);
    selectMenuItem(&tubeHVProfileMenu,
                   settings.tubeHVProfile,
                   TUBE_HVPROFILE_NUM);
    selectMenuItem(&tubeHVFrequencyMenu,
                   settings.tubeHVFrequency,
                   TUBE_HVFREQUENCY_NUM);
    selectMenuItem(&tubeHVDutyCycleMenu,
                   settings.tubeHVDutyCycle,
                   TUBE_HVDUTYCYCLE_NUM);
}

// Tube menu

static const char *const tubeMenuOptions[] = {
    "Conversion factor",
#if !defined(DISPLAY_240X320)
    "Instant. averaging",
#else
    "Inst. averaging",
#endif
#if !defined(DISPLAY_240X320)
    "Dead-time comp.",
    "Background comp.",
#else
    "Dead-time com.",
    "Background com.",
#endif
    "HV profile",
    NULL,
};

static const View *tubeMenuOptionViews[] = {
    &tubeConversionFactorMenuView,
    &tubeInstantaneousAveragingMenuView,
    &tubeDeadTimeCompensationMenuView,
    &tubeBackgroundCompensationMenuView,
    &tubeHVProfileMenuView,
};

static const char *onTubeMenuGetOption(const Menu *menu,
                                       uint32_t index,
                                       MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return tubeMenuOptions[index];
}

static void onTubeMenuSelect(const Menu *menu)
{
    setView(tubeMenuOptionViews[menu->state->selectedIndex]);
}

void onTubeSubMenuBack(const Menu *menu)
{
    setView(&tubeMenuView);
}

static MenuState tubeMenuState;

static const Menu tubeMenu = {
    "Geiger tube",
    &tubeMenuState,
    onTubeMenuGetOption,
    onTubeMenuSelect,
    onSettingsSubMenuBack,
};

const View tubeMenuView = {
    onMenuEvent,
    &tubeMenu,
};

// Custom conversion factor menu

static const float tubeConversionFactorMenuPresets[] = {
    153.8F,
    153.8F,
    68.4F,
    68.4F,
    153.8F,
    153.8F,
};

static const char *const tubeConversionFactorMenuOptions[] = {
    "J305",
    "J321",
    "J613",
    "J614",
    "M4011",
    "SBM-20",
};

static float getTubeConversionFactorForIndex(uint32_t index)
{
    if (index < TUBE_CONVERSIONFACTOR_PRESETS_NUM)
        return tubeConversionFactorMenuPresets[index];

    return TUBE_CONVERSIONFACTOR_VALUE_MIN *
           exp2f((index - TUBE_CONVERSIONFACTOR_PRESETS_NUM) *
                 (TUBE_CONVERSIONFACTOR_VALUE_LOG_MAX_MIN /
                  (TUBE_CONVERSIONFACTOR_VALUE_NUM - 1)));
}

float getTubeConversionFactor(void)
{
    return getTubeConversionFactorForIndex(settings.tubeConversionFactor);
}

static const char *onTubeConversionFactorMenuGetOption(const Menu *menu,
                                                       uint32_t index,
                                                       MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeConversionFactor);

    if (index < TUBE_CONVERSIONFACTOR_PRESETS_NUM)
    {
        strcpy(menuOption, tubeConversionFactorMenuOptions[index]);

        if (index == TUBE_CONVERSIONFACTOR_DEFAULT)
            strcat(menuOption, " (default)");

        return menuOption;
    }
    else if (index < TUBE_CONVERSIONFACTOR_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, getTubeConversionFactorForIndex(index), 1);
        strcat(menuOption, " cpm/\xb5Sv/h");
    }
    else
        return NULL;

    return menuOption;
}

static void onTubeConversionFactorMenuSelect(const Menu *menu)
{
    settings.tubeConversionFactor = menu->state->selectedIndex;

    updateMeasurementUnits();
}

static MenuState tubeConversionFactorMenuState;

static const Menu tubeConversionFactorMenu = {
    "Conversion factor",
    &tubeConversionFactorMenuState,
    onTubeConversionFactorMenuGetOption,
    onTubeConversionFactorMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeConversionFactorMenuView = {
    onMenuEvent,
    &tubeConversionFactorMenu,
};

// Instantaneous averaging men

static const char *const tubeInstantaneousAveragingMenuOptions[] = {
    "Adaptive fast",
    "Adaptive precision",
    "60 seconds",
    "30 seconds",
    "10 seconds",
    NULL,
};

static const char *onTubeInstantaneousAveragingMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeInstantaneousAveraging);

    return tubeInstantaneousAveragingMenuOptions[index];
}

static void onTubeInstantaneousAveragingMenuSelect(const Menu *menu)
{
    settings.tubeInstantaneousAveraging = menu->state->selectedIndex;
}

static MenuState tubeInstantaneousAveragingMenuState;

static const Menu tubeInstantaneousAveragingMenu = {
#if !defined(DISPLAY_240X320)
    "Instantaneous averaging",
#else
    "Inst. averaging",
#endif
    &tubeInstantaneousAveragingMenuState,
    onTubeInstantaneousAveragingMenuGetOption,
    onTubeInstantaneousAveragingMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeInstantaneousAveragingMenuView = {
    onMenuEvent,
    &tubeInstantaneousAveragingMenu,
};

// Tube dead-time compensation menu

static float getTubeDeadTimeCompensationFromIndex(uint32_t index)
{
    if (index == 0)
        return 0;

    return TUBE_DEADTIMECOMPENSATION_MIN * exp2f((index - 1) *
                                                 (TUBE_DEADTIMECOMPENSATION_LOG_MAX_MIN /
                                                  (TUBE_DEADTIMECOMPENSATION_NUM - 2)));
}

float getTubeDeadTimeCompensation(void)
{
    return getTubeDeadTimeCompensationFromIndex(settings.tubeDeadTimeCompensation);
}

static const char *onTubeDeadTimeCompensationMenuGetOption(const Menu *menu,
                                                           uint32_t index,
                                                           MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeDeadTimeCompensation);

    if (index == 0)
        return "Off";
    else if (index < TUBE_DEADTIMECOMPENSATION_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, 1000000 * getTubeDeadTimeCompensationFromIndex(index), 1);
        strcat(menuOption, " \xb5s");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeDeadTimeCompensationMenuSelect(const Menu *menu)
{
    settings.tubeDeadTimeCompensation = menu->state->selectedIndex;

    updateCompensations();
}

static MenuState tubeDeadTimeCompensationMenuState;

static const Menu tubeDeadTimeCompensationMenu = {
#if !defined(DISPLAY_240X320)
    "Dead-time compensation",
#else
    "Dead-time com.",
#endif
    &tubeDeadTimeCompensationMenuState,
    onTubeDeadTimeCompensationMenuGetOption,
    onTubeDeadTimeCompensationMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeDeadTimeCompensationMenuView = {
    onMenuEvent,
    &tubeDeadTimeCompensationMenu,
};

// Tube background compensation menu

static float getTubeBackgroundCompensationFromIndex(uint32_t index)
{
    if (index == 0)
        return 0;

    return 1E-9F * index / units[UNITS_SIEVERTS].rate.scale;
}

float getTubeBackgroundCompensation(void)
{
    return getTubeBackgroundCompensationFromIndex(settings.tubeBackgroundCompensation);
}

static const char *onTubeBackgroundCompensationMenuGetOption(const Menu *menu,
                                                             uint32_t index,
                                                             MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeBackgroundCompensation);

    if (index == 0)
        return "Off";
    else if (index < TUBE_BACKGROUNDCOMPENSATION_NUM)
    {
        Unit *rateUnit = &units[settings.units].rate;

        strclr(menuOption);
        strcatFloatAsMetricValueWithPrefix(menuOption,
                                           rateUnit->scale *
                                               getTubeBackgroundCompensationFromIndex(index),
                                           unitsMinMetricPrefixIndex[settings.units]);
        strcat(menuOption, rateUnit->name);

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeBackgroundCompensationMenuSelect(const Menu *menu)
{
    settings.tubeBackgroundCompensation = menu->state->selectedIndex;

    updateCompensations();
}

static MenuState tubeBackgroundCompensationMenuState;

static const Menu tubeBackgroundCompensationMenu = {
#if !defined(DISPLAY_240X320)
    "Background compensation",
#else
    "Background com.",
#endif
    &tubeBackgroundCompensationMenuState,
    onTubeBackgroundCompensationMenuGetOption,
    onTubeBackgroundCompensationMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeBackgroundCompensationMenuView = {
    onMenuEvent,
    &tubeBackgroundCompensationMenu,
};

// Tube HV profile menu

static const char *const tubeHVProfileMenuOptions[] = {
    "Factory default",
#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    "Accuracy",
#endif
#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    "Energy-saving",
#endif
    "Custom profile",
    NULL,
};

static const char *onTubeHVProfileMenuGetOption(const Menu *menu,
                                                uint32_t index,
                                                MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVProfile);

    if (index == TUBE_HVPROFILE_CUSTOM)
        *menuStyle |= MENUSTYLE_SUBMENU;

    return tubeHVProfileMenuOptions[index];
}

static void onTubeHVProfileMenuSelect(const Menu *menu)
{
    if (menu->state->selectedIndex < TUBE_HVPROFILE_CUSTOM)
    {
        settings.tubeHVProfile = menu->state->selectedIndex;

        updateTubeHV();
    }
    else
        setView(&tubeHVCustomProfileWarningView);
}

static MenuState tubeHVProfileMenuState;

static const Menu tubeHVProfileMenu = {
    "HV profile",
    &tubeHVProfileMenuState,
    onTubeHVProfileMenuGetOption,
    onTubeHVProfileMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeHVProfileMenuView = {
    onMenuEvent,
    &tubeHVProfileMenu,
};

static void onTubeHVGeneratorSubMenuBack(const Menu *menu)
{
    setView(&tubeHVProfileMenuView);
}

// Tube HV custom profile warning

static void onHVCustomProfileWarningEvent(const View *view, Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        onTubeHVGeneratorSubMenuBack(NULL);

        break;

    case EVENT_KEY_SELECT:
        settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;

        updateTubeHV();

        setView(&tubeHVCustomProfileMenuView);

        break;

    case EVENT_DRAW:
        drawNotification("WARNING",
                         "Wrong values can harm device.");

        break;

    default:
        break;
    }
}

static const View tubeHVCustomProfileWarningView = {
    onHVCustomProfileWarningEvent,
    NULL,
};

// Tube HV custom profile menu

static const char *const tubeHVCustomProfileMenuOptions[] = {
    "PWM frequency",
    "PWM duty cycle",
    NULL,
};

static const char *onTubeHVCustomProfileMenuGetOption(const Menu *menu,
                                                      uint32_t index,
                                                      MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return tubeHVCustomProfileMenuOptions[index];
}

static void onTubeHVCustomProfileMenuSelect(const Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
    case 0:
        setView(&tubeHVrequencyMenuView);

        break;
    case 1:
        setView(&tubeHVDutyCycleMenuView);

        break;
    }
}

static MenuState tubeHVCustomProfileMenuState;

static const Menu tubeHVCustomProfileMenu = {
    "Custom profile",
    &tubeHVCustomProfileMenuState,
    onTubeHVCustomProfileMenuGetOption,
    onTubeHVCustomProfileMenuSelect,
    onTubeHVGeneratorSubMenuBack,
};

static const View tubeHVCustomProfileMenuView = {
    onMenuEvent,
    &tubeHVCustomProfileMenu,
};

static void onTubeHVCustomProfileSubMenuBack(const Menu *menu)
{
    setView(&tubeHVCustomProfileMenuView);
}

// Tube HV frequency menu

static const char *const tubeHVFrequencyMenuOptions[] = {
    "1.25 kHz",
    "2.5 kHz",
    "5 kHz",
    "10 kHz",
    "20 kHz",
    "40 kHz",
    NULL,
};

float getTubeHVCustomProfileFrequency(uint32_t index)
{
    uint32_t value = 1250 << index;

    return value;
}

float getTubeHVFrequency(void)
{
    switch (settings.tubeHVProfile)
    {
    case TUBE_HVPROFILE_FACTORYDEFAULT:
        return TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY;

#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    case TUBE_HVPROFILE_ACCURACY:
        return TUBE_HVPROFILE_ACCURACY_FREQUENCY;
#endif

#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    case TUBE_HVPROFILE_ENERGYSAVING:
        return TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY;
#endif

    case TUBE_HVPROFILE_CUSTOM:
        return getTubeHVCustomProfileFrequency(settings.tubeHVFrequency);

    default:
        return 1250;
    }
}

static const char *onTubeHVFrequencyMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVFrequency);

    return tubeHVFrequencyMenuOptions[index];
}

static void onTubeHVFrequencyMenuSelect(const Menu *menu)
{
    settings.tubeHVFrequency = menu->state->selectedIndex;

    updateTubeHV();
}

static MenuState tubeHVFrequencyMenuState;

static const Menu tubeHVFrequencyMenu = {
    "PWM frequency",
    &tubeHVFrequencyMenuState,
    onTubeHVFrequencyMenuGetOption,
    onTubeHVFrequencyMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static const View tubeHVrequencyMenuView = {
    onMenuEvent,
    &tubeHVFrequencyMenu,
};

// Tube HV duty cycle menu

static float getTubeHVCustomProfileDutyCycle(uint32_t index)
{
    float value = index + 1;

    return TUBE_HVDUTYCYCLE_MIN * value;
}

float getTubeHVDutyCycle(void)
{
    switch (settings.tubeHVProfile)
    {
    case TUBE_HVPROFILE_FACTORYDEFAULT:
        return TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE;

#if defined(TUBE_HVPROFILE_ACCURACY_DUTYCYCLE)
    case TUBE_HVPROFILE_ACCURACY:
        return TUBE_HVPROFILE_ACCURACY_DUTYCYCLE;
#endif

#if defined(TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE)
    case TUBE_HVPROFILE_ENERGYSAVING:
        return TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE;
#endif

    case TUBE_HVPROFILE_CUSTOM:
        return getTubeHVCustomProfileDutyCycle(settings.tubeHVDutyCycle);

    default:
        return 0;
    }
}

static const char *onTubeHVDutyCycleMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVDutyCycle);

    if (index < TUBE_HVDUTYCYCLE_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, 100 * getTubeHVCustomProfileDutyCycle(index), 2);
        strcat(menuOption, "%");

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeHVDutyCycleMenuSelect(const Menu *menu)
{
    settings.tubeHVDutyCycle = menu->state->selectedIndex;

    updateTubeHV();
}

static MenuState tubeHVDutyCycleMenuState;

static const Menu tubeHVDutyCycleMenu = {
    "PWM duty cycle",
    &tubeHVDutyCycleMenuState,
    onTubeHVDutyCycleMenuGetOption,
    onTubeHVDutyCycleMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static const View tubeHVDutyCycleMenuView = {
    onMenuEvent,
    &tubeHVDutyCycleMenu,
};

// Pulses menu

enum
{
    PULSES_MENU_OPTIONS_PULSE_CLICKS,
#if defined(PULSE_LED)
    PULSES_MENU_OPTIONS_PULSE_LED,
#endif
    PULSES_MENU_OPTIONS_DISPLAY_FLASHES,
#if defined(VIBRATOR)
    PULSES_MENU_OPTIONS_HAPTIC_PULSES,
#endif
    PULSES_MENU_OPTIONS_PULSES_THRESHOLD,
};

static const char *const pulsesMenuOptions[] = {
    "Pulse clicks",
#if defined(PULSE_LED)
    "Pulse LED",
#endif
    "Display flashes",
#if defined(VIBRATOR)
    "Haptic pulses",
#endif
    "Pulses threshold",
    NULL,
};

static const char *onPulsesMenuGetOption(const Menu *menu,
                                         uint32_t index,
                                         MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return pulsesMenuOptions[index];
}

static void onPulsesMenuSelect(const Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
    case PULSES_MENU_OPTIONS_PULSE_CLICKS:
        setView(&pulseClicksMenuView);

        break;

#if defined(PULSE_LED)
    case PULSES_MENU_OPTIONS_PULSE_LED:
        setView(&pulseLEDMenuView);

        break;
#endif

    case PULSES_MENU_OPTIONS_DISPLAY_FLASHES:
        setView(&displayFlashesMenuView);

        break;

#if defined(VIBRATOR)
    case PULSES_MENU_OPTIONS_HAPTIC_PULSES:
        setView(&pulseVibrationsMenuView);

        break;
#endif

    case PULSES_MENU_OPTIONS_PULSES_THRESHOLD:
        setView(&pulsesThresholdMenuView);

        break;
    }
}

static MenuState pulsesMenuState;

static const Menu pulsesMenu = {
    "Pulses",
    &pulsesMenuState,
    onPulsesMenuGetOption,
    onPulsesMenuSelect,
    onSettingsSubMenuBack,
};

const View pulsesMenuView = {
    onMenuEvent,
    &pulsesMenu,
};

void onPulsesSubMenuBack(const Menu *menu)
{
    setView(&pulsesMenuView);
}
