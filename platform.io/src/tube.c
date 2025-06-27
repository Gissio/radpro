/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include "cmath.h"
#include "cstring.h"
#include "display.h"
#include "led.h"
#include "measurements.h"
#include "settings.h"
#include "tube.h"
#include "vibration.h"

static const Menu tubeMenu;
static const Menu tubeSensitivityMenu;
static const Menu tubeDeadTimeCompensationMenu;

#if defined(TUBE_HV_PWM)
static const Menu tubeHVProfileMenu;
static const Menu tubeHVCustomProfileMenu;
static const Menu tubeHVFrequencyMenu;
static const Menu tubeHVDutyCycleMenu;
#endif

void onTubeSubMenuBack(const Menu *menu);

#if defined(TUBE_HV_PWM)

static float getTubeHVCustomProfileFrequency(uint32_t index);
static float getTubeHVCustomProfileDutyCycle(uint32_t index);

static void onTubeHVCustomProfileSubMenuBack(const Menu *menu);

static void onTubeHVGeneratorSubMenuBack(const Menu *menu);

static struct
{
    float pwmFrequency;
    float pwmDutyCycle;
} tube;

#endif

void initTube(void)
{
#if defined(TUBE_HV_PWM)
    tube.pwmFrequency = getTubeHVCustomProfileFrequency(
        (settings.tubeHVFrequency >= TUBE_HVFREQUENCY_NUM)
            ? TUBE_HVFREQUENCY_NUM - 1
            : settings.tubeHVFrequency);
    tube.pwmDutyCycle = getTubeHVCustomProfileDutyCycle(
        (settings.tubeHVDutyCycle >= TUBE_HVDUTYCYCLE_NUM)
            ? TUBE_HVDUTYCYCLE_NUM - 1
            : settings.tubeHVDutyCycle);
#endif

    initTubeHardware();
}

void resetTube(void)
{
    selectMenuItem(&tubeMenu,
                   0,
                   0);
    selectMenuItem(&tubeSensitivityMenu,
                   settings.tubeSensitivity,
                   TUBE_SENSITIVITY_NUM);
    selectMenuItem(&tubeDeadTimeCompensationMenu,
                   settings.tubeDeadTimeCompensation,
                   TUBE_DEADTIMECOMPENSATION_NUM);

#if defined(TUBE_HV_PWM)
    selectMenuItem(&tubeHVProfileMenu,
                   settings.tubeHVProfile,
                   TUBE_HVPROFILE_NUM);
    selectMenuItem(&tubeHVCustomProfileMenu,
                   0,
                   0);
    selectMenuItem(&tubeHVFrequencyMenu,
                   settings.tubeHVFrequency,
                   TUBE_HVFREQUENCY_NUM);
    selectMenuItem(&tubeHVDutyCycleMenu,
                   settings.tubeHVDutyCycle,
                   TUBE_HVDUTYCYCLE_NUM);
#endif
}

// Tube HV frequency menu

#if defined(TUBE_HV_PWM)

static float getTubeHVCustomProfileFrequency(uint32_t index)
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
        return tube.pwmFrequency;

    default:
        return 1250;
    }
}

bool setTubeHVFrequency(float value)
{
    if ((value > 100000.0F) || (value < 100.0F))
        return false;

    tube.pwmFrequency = value;

    int32_t tubeHVFrequency = log2f(value / 1250);
    settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;
    settings.tubeHVFrequency = tubeHVFrequency;

    updateTubeHV();

    return true;
}

static const char *onTubeHVFrequencyMenuGetOption(const Menu *menu,
                                                  uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVFrequency);

    float frequency = getTubeHVCustomProfileFrequency(index) / 1000;

    strcpy(menuOption, "");
    strcatFloat(menuOption, frequency, 2);
    strcatChar(menuOption, ' ');
    strcat(menuOption, getString(STRING_KHZ));

    return menuOption;
}

static void onTubeHVFrequencyMenuSelect(const Menu *menu)
{
    setTubeHVFrequency(
        getTubeHVCustomProfileFrequency(menu->state->selectedIndex));
}

static MenuState tubeHVFrequencyMenuState;

static const Menu tubeHVFrequencyMenu = {
    getString(STRING_PWMFREQUENCY),
    &tubeHVFrequencyMenuState,
    onTubeHVFrequencyMenuGetOption,
    onTubeHVFrequencyMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static const View tubeHVrequencyMenuView = {
    onMenuEvent,
    &tubeHVFrequencyMenu,
};

#endif

// Tube HV duty cycle menu

#if defined(TUBE_HV_PWM)

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
        return tube.pwmDutyCycle;

    default:
        return 0;
    }
}

bool setTubeHVDutyCycle(float value)
{
    if ((value > 1) || (value < 0))
        return false;

    tube.pwmDutyCycle = value;

    int32_t tubeHVDutyCycle = (value - TUBE_HVDUTYCYCLE_MIN / 2) /
                              TUBE_HVDUTYCYCLE_STEP;
    if (tubeHVDutyCycle < 0)
        tubeHVDutyCycle = 0;
    else if (tubeHVDutyCycle >= TUBE_HVDUTYCYCLE_NUM)
        tubeHVDutyCycle = TUBE_HVDUTYCYCLE_NUM - 1;

    settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;
    settings.tubeHVDutyCycle = tubeHVDutyCycle;

    updateTubeHV();

    return true;
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
    setTubeHVDutyCycle(
        getTubeHVCustomProfileDutyCycle(menu->state->selectedIndex));
}

static MenuState tubeHVDutyCycleMenuState;

static const Menu tubeHVDutyCycleMenu = {
    getString(STRING_PWMDUTY_CYCLE),
    &tubeHVDutyCycleMenuState,
    onTubeHVDutyCycleMenuGetOption,
    onTubeHVDutyCycleMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static const View tubeHVDutyCycleMenuView = {
    onMenuEvent,
    &tubeHVDutyCycleMenu,
};

#endif

// Tube HV custom profile menu

#if defined(TUBE_HV_PWM)

static const char *const tubeHVCustomProfileMenuOptions[] = {
    getString(STRING_PWMFREQUENCY),
    getString(STRING_PWMDUTY_CYCLE),
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
    getString(STRING_CUSTOM),
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

#endif

// Tube HV custom profile warning

#if defined(TUBE_HV_PWM)

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
        drawNotification(getString(STRING_NOTIFICATION_WARNING),
                         getString(STRING_NOTIFICATION_HVCUSTOM));

        break;

    default:
        break;
    }
}

static const View tubeHVCustomProfileWarningView = {
    onHVCustomProfileWarningEvent,
    NULL,
};

#endif

// Tube HV profile menu

#if defined(TUBE_HV_PWM)

static const char *const tubeHVProfileMenuOptions[] = {
    getString(STRING_FACTORY_DEFAULT),
#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    getString(STRING_ACCURACY),
#endif
#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    getString(STRING_ENERGY_SAVING),
#endif
    getString(STRING_CUSTOM),
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
    getString(STRING_HVPROFILE),
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

#endif

// Tube sensitivity menu

static const float tubeSensitivityMenuPresets[] = {
    153.8F,
    153.8F,
    60.0F,
    60.0F,
    153.8F,
    150.5F,
};

static const char *const tubeSensitivityMenuOptions[] = {
    getString(STRING_J305),
    getString(STRING_J321),
    getString(STRING_J613),
    getString(STRING_J614),
    getString(STRING_M4011),
    getString(STRING_SBM20),
};

static float getTubeSensitivityForIndex(uint32_t index)
{
    if (index < TUBE_SENSITIVITY_PRESETS_NUM)
        return tubeSensitivityMenuPresets[index];

    return TUBE_SENSITIVITY_VALUE_MIN *
           exp2f((index - TUBE_SENSITIVITY_PRESETS_NUM) *
                 (TUBE_SENSITIVITY_VALUE_LOG2_MAX_MIN /
                  (TUBE_SENSITIVITY_VALUE_NUM - 1)));
}

float getTubeSensitivity(void)
{
    return getTubeSensitivityForIndex(settings.tubeSensitivity);
}

uint32_t getLossOfCountTime(void)
{
    // 60 s/min * [number of periods] * [lowest expected radiation in uSv/h]
    return (uint32_t) ((60.0F * 10.0F / 0.05F) / getTubeSensitivity()) + 1;
}

static const char *onTubeSensitivityMenuGetOption(const Menu *menu,
                                                       uint32_t index,
                                                       MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeSensitivity);

    if (index < TUBE_SENSITIVITY_PRESETS_NUM)
    {
        strcpy(menuOption, tubeSensitivityMenuOptions[index]);

        if (index == TUBE_SENSITIVITY_DEFAULT)
            strcat(menuOption, getString(STRING_DEFAULT));

        return menuOption;
    }
    else if (index < TUBE_SENSITIVITY_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, getTubeSensitivityForIndex(index), 2);
        strcatChar(menuOption, ' ');
        strcat(menuOption, getString(STRING_CPMUSVH));
    }
    else
        return NULL;

    return menuOption;
}

static void onTubeSensitivityMenuSelect(const Menu *menu)
{
    settings.tubeSensitivity = menu->state->selectedIndex;

    updateMeasurementUnits();
}

static MenuState tubeSensitivityMenuState;

static const Menu tubeSensitivityMenu = {
    getString(STRING_SENSITIVITY),
    &tubeSensitivityMenuState,
    onTubeSensitivityMenuGetOption,
    onTubeSensitivityMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeSensitivityMenuView = {
    onMenuEvent,
    &tubeSensitivityMenu,
};

// Tube dead-time compensation menu

static float getTubeDeadTimeCompensationFromIndex(uint32_t index)
{
    if (index == 0)
        return 0;

    return TUBE_DEADTIMECOMPENSATION_MIN * exp2f((index - 1) *
                                                 (TUBE_DEADTIMECOMPENSATION_LOG2_MAX_MIN /
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
        return getString(STRING_OFF);
    else if (index < TUBE_DEADTIMECOMPENSATION_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, 1000000 * getTubeDeadTimeCompensationFromIndex(index), 2);
        strcatChar(menuOption, ' ');
        strcat(menuOption, getString(STRING_MICROSECONDS));

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeDeadTimeCompensationMenuSelect(const Menu *menu)
{
    settings.tubeDeadTimeCompensation = menu->state->selectedIndex;
}

static MenuState tubeDeadTimeCompensationMenuState;

static const Menu tubeDeadTimeCompensationMenu = {
    getString(STRING_DEAD_TIME_SHORT_COMPENSATION),
    &tubeDeadTimeCompensationMenuState,
    onTubeDeadTimeCompensationMenuGetOption,
    onTubeDeadTimeCompensationMenuSelect,
    onTubeSubMenuBack,
};

static const View tubeDeadTimeCompensationMenuView = {
    onMenuEvent,
    &tubeDeadTimeCompensationMenu,
};

// Tube menu

static const char *const tubeMenuOptions[] = {
    getString(STRING_SENSITIVITY),
    getString(STRING_DEAD_TIME_SHORT_COMPENSATION),
#if defined(TUBE_HV_PWM)
    getString(STRING_HVPROFILE),
#endif
    NULL,
};

static const View *tubeMenuOptionViews[] = {
    &tubeSensitivityMenuView,
    &tubeDeadTimeCompensationMenuView,
#if defined(TUBE_HV_PWM)
    &tubeHVProfileMenuView,
#endif
    NULL,
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
    getString(STRING_GEIGER_TUBE),
    &tubeMenuState,
    onTubeMenuGetOption,
    onTubeMenuSelect,
    onSettingsSubMenuBack,
};

const View tubeMenuView = {
    onMenuEvent,
    &tubeMenu,
};
