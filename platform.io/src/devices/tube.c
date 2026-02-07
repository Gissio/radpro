/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "../devices/tube.h"
#include "../measurements/pulses.h"
#include "../system/cmath.h"
#include "../system/settings.h"
#include "../ui/system.h"
#include "../ui/menu.h"

#define SOURCE_FACTOR_DATA_MIN 0.125F
#define SOURCE_FACTOR_DATA_SCALE 36

static Menu sourceMenu;

static Menu tubeMenu;
static Menu tubeTypeMenu;
static Menu tubeSensitivityMenu;
static Menu tubeDeadTimeCompensationMenu;

static cstring tubeTypeLabels[] = {
    STRING_J305,
    STRING_M4011,
    STRING_HH614,
    STRING_SBM20,
    STRING_SI3BG,
    STRING_LND7317,
    NULL,
};

static const float tubeTypeSensitivities[] = {
    135.115F,
    108.683F,
    38.261F,
    106.709F,
    4.856F,
    350.0F,
};

static cstring sourceLabels[] = {
    STRING_CS137,
    STRING_CO60,
    STRING_TC99M,
    STRING_I131,
    STRING_LU177,
    STRING_AM241,
    STRING_RADIUM,
    STRING_URANIUM_ORE,
    STRING_URANIUM_GLASS,
    STRING_DEPLETED_URANIUM,
    STRING_THORIUM_ORE,
    STRING_XRAYS,
    STRING_K40,
    STRING_BACKGROUND_RADIATION,
    NULL,
};

static const uint8_t tubeTypeSourceFactorCodes[][14] = {
    {108, 113, 204, 107, 173, 236, 111, 117, 124, 123, 113, 196, 110, 161},
    {108, 113, 204, 107, 173, 236, 111, 117, 124, 123, 113, 196, 110, 161},
    {108, 124, 156, 93, 113, 185, 106, 108, 110, 113, 60, 153, 125, 124},
    {108, 95, 169, 103, 126, 207, 100, 105, 110, 110, 100, 198, 91, 137},
    {108, 125, 154, 94, 113, 77, 107, 107, 109, 113, 88, 0, 128, 88},
    {108, 108, 162, 102, 127, 192, 106, 110, 114, 114, 107, 188, 107, 137},
};

static void setTubeMenu(void);

#if defined(TUBE_HV_PWM)
static Menu hvProfilesMenu;
static Menu hvCustomProfileMenu;
static Menu hvFrequencyMenu;
static Menu hvDutyCycleMenu;

static struct
{
    float pwmFrequency;
    float pwmDutyCycle;
} tube;

static float getTubeHVFrequencyForIndex(uint32_t index);
static float getTubeHVDutyCycleForIndex(uint32_t index);

static void setHVCustomProfileMenu(void);
static void setHVProfilesMenu(void);
#endif

volatile uint32_t tubePulseCount;
volatile uint32_t tubeRandomBits;
volatile uint32_t tubeDeadTime;

void initTube(void)
{
#if defined(TUBE_HV_PWM)
    tube.pwmFrequency = getTubeHVFrequencyForIndex(settings.tubeHVFrequency);
    tube.pwmDutyCycle = getTubeHVDutyCycleForIndex(settings.tubeHVDutyCycle);
#endif

    initTubeHardware();

    tubeDeadTime = USHRT_MAX;
}

void setupTube(void)
{
    selectMenuItem(&sourceMenu, settings.source, SOURCE_NUM);

    selectMenuItem(&tubeMenu, 0, 0);
    selectMenuItem(&tubeTypeMenu, settings.tubeType, TUBE_SENSITIVITY_NUM);
    selectMenuItem(&tubeSensitivityMenu, settings.tubeSensitivity, TUBE_SENSITIVITY_NUM);
    selectMenuItem(&tubeDeadTimeCompensationMenu, settings.tubeDeadTimeCompensation, TUBE_DEADTIMECOMPENSATION_NUM);

#if defined(TUBE_HV_PWM)
    selectMenuItem(&hvProfilesMenu, settings.tubeHVProfile, TUBE_HVPROFILE_NUM);
    selectMenuItem(&hvCustomProfileMenu, 0, 0);
    selectMenuItem(&hvFrequencyMenu, settings.tubeHVFrequency, TUBE_HVFREQUENCY_NUM);
    selectMenuItem(&hvDutyCycleMenu, settings.tubeHVDutyCycle, TUBE_HVDUTYCYCLE_NUM);
#endif
}

// Tube sensitivity

static float getTubeSensitivityForIndex(uint32_t index)
{
    if (index == 0)
        return tubeTypeSensitivities[settings.tubeType];
    else
        return TUBE_SENSITIVITY_VALUE_MIN *
               exp2f((float)(index - 1) * (TUBE_SENSITIVITY_VALUE_LOG2_MAX_MIN / (TUBE_SENSITIVITY_VALUE_NUM - 1)));
}

float getTubeSensitivity(void)
{
    uint8_t code = tubeTypeSourceFactorCodes[settings.tubeType][settings.source];
    float sourceFactor = SOURCE_FACTOR_DATA_MIN * exp2f(code * (1.0F / SOURCE_FACTOR_DATA_SCALE));

    return sourceFactor * getTubeSensitivityForIndex(settings.tubeSensitivity);
}

uint32_t getLossOfCountTime(void)
{
    // (60 s/min) * (number of pulses) * (lowest expected radiation in uSv/h)
    return (uint32_t)(((60.0F) * (10.0F) / (0.05F)) / getTubeSensitivity()) + 1;
}

// Dead-time compensation

static float getTubeDeadTimeCompensationForIndex(uint32_t index)
{
    if (index == 0)
        return 0.0F;
    else
        return TUBE_DEADTIMECOMPENSATION_VALUE_MIN * exp2f((float)(index - 1) *
                                                           (TUBE_DEADTIMECOMPENSATION_VALUE_LOG2_MAX_MIN /
                                                            (TUBE_DEADTIMECOMPENSATION_VALUE_NUM - 1)));
}

float getTubeDeadTimeCompensation(void)
{
    return getTubeDeadTimeCompensationForIndex(settings.tubeDeadTimeCompensation);
}

// HV PWM parameters

#if defined(TUBE_HV_PWM)

static float getTubeHVFrequencyForIndex(uint32_t index)
{
    uint32_t value = (1250 << index);

    return value;
}

static float getTubeHVDutyCycleForIndex(uint32_t index)
{
    float value = (index + 1);

    return TUBE_HVDUTYCYCLE_MIN * value;
}

float getTubeHVFrequency(void)
{
    switch (settings.tubeHVProfile)
    {
    case TUBE_HVPROFILE_FACTORYDEFAULT:
    default:
    {
#if defined(TUBE_DEFAULT_HVPROFILE_IN_FLASH)
        float value = getTubeDefaultHVFrequency();
        if (value >= 0)
            return value;
#endif
        return TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY;
    }

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
    }
}

bool setTubeHVFrequency(float value)
{
    if ((value > 100000.0F) || (value < 100.0F))
        return false;

    int32_t tubeHVFrequency = log2f(value / 1250.0F);

    settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;
    settings.tubeHVFrequency = tubeHVFrequency;

    tube.pwmFrequency = value;

    updateTubeHV();

    return true;
}

float getTubeHVDutyCycle(void)
{
    switch (settings.tubeHVProfile)
    {
    case TUBE_HVPROFILE_FACTORYDEFAULT:
    default:
    {
#if defined(TUBE_DEFAULT_HVPROFILE_IN_FLASH)
        float value = getTubeDefaultHVDutyCycle();
        if (value >= 0)
            return value;
#endif
        return TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE;
    }

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
    }

    return 0.0F;
}

bool setTubeHVDutyCycle(float value)
{
    if ((value < 0.0F) || (value > 1.0F))
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

#endif

// Measurements source menu

static const char *onSourceMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.source);

    return getString(sourceLabels[index]);
}

static void onSourceMenuSelect(uint32_t index)
{
    settings.source = index;

    updateDoseUnits();
}

static MenuState sourceMenuState;

static Menu sourceMenu = {
    STRING_SOURCE,
    &sourceMenuState,
    onSourceMenuGetOption,
    onSourceMenuSelect,
    setMeasurementsMenu,
};

View sourceMenuView = {
    onMenuEvent,
    &sourceMenu,
};

// Tube type menu

static const char *onTubeTypeMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeType);

    return tubeTypeLabels[index];
}

static void onTubeTypeMenuSelect(uint32_t index)
{
    settings.tubeType = index;

    updateDoseUnits();
}

static MenuState tubeTypeMenuState;

static Menu tubeTypeMenu = {
    STRING_TUBE_TYPE,
    &tubeTypeMenuState,
    onTubeTypeMenuGetOption,
    onTubeTypeMenuSelect,
    setTubeMenu,
};

View tubeTypeMenuView = {
    onMenuEvent,
    &tubeTypeMenu,
};

// Sensitivity menu

static const char *onTubeSensitivityMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeSensitivity);

    if (index == 0)
        strcpy(menuOption, getString(STRING_TUBE_DEFAULT));
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

    updateDoseUnits();
}

static MenuState tubeSensitivityMenuState;

static Menu tubeSensitivityMenu = {
    STRING_SENSITIVITY,
    &tubeSensitivityMenuState,
    onTubeSensitivityMenuGetOption,
    onTubeSensitivityMenuSelect,
    setTubeMenu,
};

static View tubeSensitivityMenuView = {
    onMenuEvent,
    &tubeSensitivityMenu,
};

// Dead-time compensation menu

static const char *onDeadTimeCompensationMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeDeadTimeCompensation);

    if (index == 0)
        return getString(STRING_OFF);
    else if (index < TUBE_DEADTIMECOMPENSATION_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, 1000000.0F * getTubeDeadTimeCompensationForIndex(index), 2);
        strcatChar(menuOption, ' ');
        strcat(menuOption, getString(STRING_MICROSECONDS));

        return menuOption;
    }
    else
        return NULL;
}

static void onDeadTimeCompensationMenuSelect(uint32_t index)
{
    settings.tubeDeadTimeCompensation = index;
}

static MenuState deadTimeCompensationMenuState;

static Menu tubeDeadTimeCompensationMenu = {
    STRING_DEAD_TIME_COMPENSATION,
    &deadTimeCompensationMenuState,
    onDeadTimeCompensationMenuGetOption,
    onDeadTimeCompensationMenuSelect,
    setTubeMenu,
};

static View deadTimeCompensationMenuView = {
    onMenuEvent,
    &tubeDeadTimeCompensationMenu,
};

#if defined(TUBE_HV_PWM)

// HV frequency menu

static const char *onHVFrequencyMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVFrequency);

    if (index < TUBE_HVFREQUENCY_NUM)
    {
        float frequency = getTubeHVFrequencyForIndex(index) / 1000.0F;

        strclr(menuOption);
        strcatFloat(menuOption, frequency, 2);
        strcatChar(menuOption, ' ');
        strcat(menuOption, getString(STRING_KHZ));

        return menuOption;
    }
    else
        return NULL;
}

static void onHVFrequencyMenuSelect(uint32_t index)
{
    setTubeHVFrequency(getTubeHVFrequencyForIndex(index));
}

static MenuState hvFrequencyMenuState;

static Menu hvFrequencyMenu = {
    STRING_PWMFREQUENCY,
    &hvFrequencyMenuState,
    onHVFrequencyMenuGetOption,
    onHVFrequencyMenuSelect,
    setHVCustomProfileMenu,
};

static View hvFrequencyMenuView = {
    onMenuEvent,
    &hvFrequencyMenu,
};

// HV duty cycle menu

static const char *onHVDutyCycleMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVDutyCycle);

    if (index < TUBE_HVDUTYCYCLE_NUM)
    {
        strclr(menuOption);
        strcatFloat(menuOption, 100.0F * getTubeHVDutyCycleForIndex(index), 2);
        strcat(menuOption, "%");

        return menuOption;
    }
    else
        return NULL;
}

static void onHVDutyCycleMenuSelect(uint32_t index)
{
    setTubeHVDutyCycle(getTubeHVDutyCycleForIndex(index));
}

static MenuState hvDutyCycleMenuState;

static Menu hvDutyCycleMenu = {
    STRING_PWMDUTY_CYCLE,
    &hvDutyCycleMenuState,
    onHVDutyCycleMenuGetOption,
    onHVDutyCycleMenuSelect,
    setHVCustomProfileMenu,
};

static View hvDutyCycleMenuView = {
    onMenuEvent,
    &hvDutyCycleMenu,
};

// HV custom profile menu

static MenuOption hvCustomProfileMenuOptions[] = {
    {STRING_PWMFREQUENCY, &hvFrequencyMenuView},
    {STRING_PWMDUTY_CYCLE, &hvDutyCycleMenuView},
    {NULL},
};

static const char *onHVCustomProfileMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(hvCustomProfileMenuOptions[index].title);
}

static void onHVCustomProfileMenuSelect(uint32_t index)
{
    setView(hvCustomProfileMenuOptions[index].view);
}

static MenuState hvCustomProfileMenuState;

static Menu hvCustomProfileMenu = {
    STRING_CUSTOM,
    &hvCustomProfileMenuState,
    onHVCustomProfileMenuGetOption,
    onHVCustomProfileMenuSelect,
    setHVProfilesMenu,
};

static View hvCustomProfileMenuView = {
    onMenuEvent,
    &hvCustomProfileMenu,
};

static void setHVCustomProfileMenu(void)
{
    setView(&hvCustomProfileMenuView);
}

// HV custom profile warning

static void onHVCustomProfileWarningEvent(Event event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        setHVProfilesMenu();

        break;

    case EVENT_KEY_SELECT:
        settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;

        updateTubeHV();

        setView(&hvCustomProfileMenuView);

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_CUSTOM), getString(STRING_NOTIFICATION_HVCUSTOM));

        break;

    default:
        break;
    }
}

static View hvCustomProfileWarningView = {
    onHVCustomProfileWarningEvent,
    NULL,
};

// HV profiles menu

static cstring hvProfilesMenuOptions[] = {
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

static const char *onHVProfilesMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVProfile);

    if (index == TUBE_HVPROFILE_CUSTOM)
        *menuStyle |= MENUSTYLE_SUBMENU;

    return getString(hvProfilesMenuOptions[index]);
}

static void onHVProfilesMenuSelect(uint32_t index)
{
    if (index != TUBE_HVPROFILE_CUSTOM)
    {
        settings.tubeHVProfile = index;

        updateTubeHV();
    }
    else
    {
        if (index != settings.tubeHVProfile)
            setView(&hvCustomProfileWarningView);
        else
            setView(&hvCustomProfileMenuView);
    }
}

static MenuState hvProfilesMenuState;

static Menu hvProfilesMenu = {
    STRING_HVPROFILE,
    &hvProfilesMenuState,
    onHVProfilesMenuGetOption,
    onHVProfilesMenuSelect,
    setTubeMenu,
};

static View hvProfilesMenuView = {
    onMenuEvent,
    &hvProfilesMenu,
};

static void setHVProfilesMenu(void)
{
    setView(&hvProfilesMenuView);
}

#endif

// Tube menu

static MenuOption tubeMenuOptions[] = {
    {STRING_TUBE_TYPE, &tubeTypeMenuView},
    {STRING_SENSITIVITY, &tubeSensitivityMenuView},
    {STRING_DEAD_TIME_COMPENSATION, &deadTimeCompensationMenuView},
#if defined(TUBE_HV_PWM)
    {STRING_HVPROFILE, &hvProfilesMenuView},
#endif
    {NULL},
};

static const char *onTubeMenuGetOption(uint32_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(tubeMenuOptions[index].title);
}

static void onTubeMenuSelect(uint32_t index)
{
    setView(tubeMenuOptions[index].view);
}

static MenuState tubeMenuState;

static Menu tubeMenu = {
    STRING_GEIGER_TUBE,
    &tubeMenuState,
    onTubeMenuGetOption,
    onTubeMenuSelect,
    setSettingsMenu,
};

View tubeMenuView = {
    onMenuEvent,
    &tubeMenu,
};

static void setTubeMenu(void)
{
    setView(&tubeMenuView);
}
