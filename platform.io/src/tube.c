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

static Menu tubeMenu;
static Menu tubeSensitivityMenu;
static Menu tubeDeadTimeCompensationMenu;

#if defined(TUBE_HV_PWM)
static Menu tubeHVProfileMenu;
static Menu tubeHVCustomProfileMenu;
static Menu tubeHVFrequencyMenu;
static Menu tubeHVDutyCycleMenu;
#endif

void onTubeSubMenuBack(void);

#if defined(TUBE_HV_PWM)

static float getTubeHVCustomProfileFrequency(uint32_t index);
static float getTubeHVCustomProfileDutyCycle(uint32_t index);

static void onTubeHVCustomProfileSubMenuBack(void);

static void onTubeHVGeneratorSubMenuBack(void);

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

    initTubeHardware();
#endif
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

#if defined(TUBE_HV_PWM)

// Tube HV frequency menu

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

static const char *onTubeHVFrequencyMenuGetOption(uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVFrequency);

    if (index < TUBE_HVFREQUENCY_NUM)
    {
        float frequency = getTubeHVCustomProfileFrequency(index) / 1000;

        strclr(menuOption);
        strcatFloat(menuOption, frequency, 2);
        strcatChar(menuOption, ' ');
        strcat(menuOption, getString(STRING_KHZ));

        return menuOption;
    }
    else
        return NULL;
}

static void onTubeHVFrequencyMenuSelect(uint32_t index)
{
    setTubeHVFrequency(getTubeHVCustomProfileFrequency(index));
}

static MenuState tubeHVFrequencyMenuState;

static Menu tubeHVFrequencyMenu = {
    STRING_PWMFREQUENCY,
    &tubeHVFrequencyMenuState,
    onTubeHVFrequencyMenuGetOption,
    onTubeHVFrequencyMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static View tubeHVrequencyMenuView = {
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

static const char *onTubeHVDutyCycleMenuGetOption(uint32_t index,
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

static void onTubeHVDutyCycleMenuSelect(uint32_t index)
{
    setTubeHVDutyCycle(
        getTubeHVCustomProfileDutyCycle(index));
}

static MenuState tubeHVDutyCycleMenuState;

static Menu tubeHVDutyCycleMenu = {
    STRING_PWMDUTY_CYCLE,
    &tubeHVDutyCycleMenuState,
    onTubeHVDutyCycleMenuGetOption,
    onTubeHVDutyCycleMenuSelect,
    onTubeHVCustomProfileSubMenuBack,
};

static View tubeHVDutyCycleMenuView = {
    onMenuEvent,
    &tubeHVDutyCycleMenu,
};

// Tube HV custom profile menu

static cstring tubeHVCustomProfileMenuOptions[] = {
    STRING_PWMFREQUENCY,
    STRING_PWMDUTY_CYCLE,
    NULL,
};

static const char *onTubeHVCustomProfileMenuGetOption(uint32_t index,
                                                      MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(tubeHVCustomProfileMenuOptions[index]);
}

static void onTubeHVCustomProfileMenuSelect(uint32_t index)
{
    switch (index)
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

static Menu tubeHVCustomProfileMenu = {
    STRING_CUSTOM,
    &tubeHVCustomProfileMenuState,
    onTubeHVCustomProfileMenuGetOption,
    onTubeHVCustomProfileMenuSelect,
    onTubeHVGeneratorSubMenuBack,
};

static View tubeHVCustomProfileMenuView = {
    onMenuEvent,
    &tubeHVCustomProfileMenu,
};

static void onTubeHVCustomProfileSubMenuBack(void)
{
    setView(&tubeHVCustomProfileMenuView);
}

// Tube HV custom profile warning

static void onHVCustomProfileWarningEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        onTubeHVGeneratorSubMenuBack();

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

static View tubeHVCustomProfileWarningView = {
    onHVCustomProfileWarningEvent,
    NULL,
};

// Tube HV profile menu

static cstring tubeHVProfileMenuOptions[] = {
    STRING_FACTORY_DEFAULT,
#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    STRING_ACCURACY,
#endif
#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    STRING_ENERGY_SAVING,
#endif
    STRING_CUSTOM,
    NULL,
};

static const char *onTubeHVProfileMenuGetOption(uint32_t index,
                                                MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVProfile);

    if (index == TUBE_HVPROFILE_CUSTOM)
        *menuStyle |= MENUSTYLE_SUBMENU;

    return getString(tubeHVProfileMenuOptions[index]);
}

static void onTubeHVProfileMenuSelect(uint32_t index)
{
    if (index < TUBE_HVPROFILE_CUSTOM)
    {
        settings.tubeHVProfile = index;

        updateTubeHV();
    }
    else
        setView(&tubeHVCustomProfileWarningView);
}

static MenuState tubeHVProfileMenuState;

static Menu tubeHVProfileMenu = {
    STRING_HVPROFILE,
    &tubeHVProfileMenuState,
    onTubeHVProfileMenuGetOption,
    onTubeHVProfileMenuSelect,
    onTubeSubMenuBack,
};

static View tubeHVProfileMenuView = {
    onMenuEvent,
    &tubeHVProfileMenu,
};

static void onTubeHVGeneratorSubMenuBack(void)
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

static cstring tubeSensitivityMenuOptions[] = {
    STRING_J305,
    STRING_J321,
    STRING_J613,
    STRING_J614,
    STRING_M4011,
    STRING_SBM20,
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
    return (uint32_t)((60.0F * 10.0F / 0.05F) / getTubeSensitivity()) + 1;
}

static const char *onTubeSensitivityMenuGetOption(uint32_t index,
                                                  MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeSensitivity);

    if (index < TUBE_SENSITIVITY_PRESETS_NUM)
    {
        strcpy(menuOption, getString(tubeSensitivityMenuOptions[index]));

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

static void onTubeSensitivityMenuSelect(uint32_t index)
{
    settings.tubeSensitivity = index;

    updateMeasurementUnits();
}

static MenuState tubeSensitivityMenuState;

static Menu tubeSensitivityMenu = {
    STRING_SENSITIVITY,
    &tubeSensitivityMenuState,
    onTubeSensitivityMenuGetOption,
    onTubeSensitivityMenuSelect,
    onTubeSubMenuBack,
};

static View tubeSensitivityMenuView = {
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

static const char *onTubeDeadTimeCompensationMenuGetOption(uint32_t index,
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

static void onTubeDeadTimeCompensationMenuSelect(uint32_t index)
{
    settings.tubeDeadTimeCompensation = index;
}

static MenuState tubeDeadTimeCompensationMenuState;

static Menu tubeDeadTimeCompensationMenu = {
    STRING_DEAD_TIME_SHORT_COMPENSATION,
    &tubeDeadTimeCompensationMenuState,
    onTubeDeadTimeCompensationMenuGetOption,
    onTubeDeadTimeCompensationMenuSelect,
    onTubeSubMenuBack,
};

static View tubeDeadTimeCompensationMenuView = {
    onMenuEvent,
    &tubeDeadTimeCompensationMenu,
};

// Tube menu

static SubMenuOption tubeMenuOptions[] = {
    {STRING_SENSITIVITY, &tubeSensitivityMenuView},
    {STRING_DEAD_TIME_SHORT_COMPENSATION, &tubeDeadTimeCompensationMenuView},
#if defined(TUBE_HV_PWM)
    {STRING_HVPROFILE, &tubeHVProfileMenuView},
#endif
    {NULL},
};

static const char *onTubeMenuGetOption(uint32_t index,
                                       MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(tubeMenuOptions[index].title);
}

static void onTubeMenuSelect(uint32_t index)
{
    setView(tubeMenuOptions[index].view);
}

void onTubeSubMenuBack(void)
{
    setView(&tubeMenuView);
}

static MenuState tubeMenuState;

static Menu tubeMenu = {
    STRING_GEIGER_TUBE,
    &tubeMenuState,
    onTubeMenuGetOption,
    onTubeMenuSelect,
    onSettingsSubMenuBack,
};

View tubeMenuView = {
    onMenuEvent,
    &tubeMenu,
};
