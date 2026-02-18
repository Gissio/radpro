/*
 * Rad Pro
 * Tube
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <limits.h>

#include "../measurements/pulses.h"
#include "../peripherals/tube.h"
#include "../system/cmath.h"
#include "../system/settings.h"
#include "../ui/system.h"

#define SOURCE_FACTOR_DATA_MIN 0.125F
#define SOURCE_FACTOR_DATA_SCALE 36

static const float tubeTypeSensitivities[] = {
    135.200F,
    108.345F,
    30.157F,
    106.105F,
    3.267F,
    252.567F,
};

static const uint8_t tubeTypeSourceFactorCodes[][14] = {
    {108, 113, 204, 107, 172, 236, 111, 117, 124, 123, 113, 196, 110, 123},
    {108, 113, 204, 107, 173, 236, 111, 118, 124, 123, 113, 196, 110, 123},
    {108, 124, 156, 93, 113, 185, 106, 106, 106, 112, 62, 153, 125, 112},
    {108, 95, 169, 103, 126, 207, 100, 105, 110, 110, 100, 198, 92, 108},
    {108, 125, 153, 93, 113, 77, 107, 106, 108, 112, 89, 0, 128, 109},
    {108, 107, 154, 104, 127, 167, 107, 111, 114, 113, 108, 161, 107, 115},
};

static const Menu tubeMenu;
static const Menu tubeTypeMenu;
static const Menu tubeSensitivityMenu;
static const Menu tubeDeadTimeCompensationMenu;

#if defined(TUBE_HV_PWM)
static const Menu hvProfilesMenu;
static const Menu hvCustomProfileMenu;
static const Menu hvFrequencyMenu;
static const Menu hvDutyCycleMenu;

static struct
{
    float pwmFrequency;
    float pwmDutyCycle;
} tube;

static float getTubeHVFrequencyForIndex(uint32_t index);
static float getTubeHVDutyCycleForIndex(uint32_t index);

static void showHVCustomProfileMenu(void);
static void showHVProfilesMenu(void);
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
    selectMenuItem(&sourceMenu, settings.source);

    selectMenuItem(&tubeMenu, 0);
    selectMenuItem(&tubeTypeMenu, settings.tubeType);
    selectMenuItem(&tubeSensitivityMenu, settings.tubeSensitivity);
    selectMenuItem(&tubeDeadTimeCompensationMenu, settings.tubeDeadTimeCompensation);

#if defined(TUBE_HV_PWM)
    selectMenuItem(&hvProfilesMenu, settings.tubeHVProfile);
    selectMenuItem(&hvCustomProfileMenu, 0);
    selectMenuItem(&hvFrequencyMenu, settings.tubeHVFrequency);
    selectMenuItem(&hvDutyCycleMenu, settings.tubeHVDutyCycle);
#endif
}

// Tube sensitivity

static uint32_t getTubeIndex(uint32_t type)
{
    if (type > TUBE_TYPE_M4011)
        type--;

    return type;
}

static float getTubeSensitivityForIndex(uint32_t index)
{
    if (index == 0)
        return tubeTypeSensitivities[getTubeIndex(index)];
    else
        return TUBE_SENSITIVITY_VALUE_MIN *
               exp2f((float)(index - 1) * (TUBE_SENSITIVITY_VALUE_LOG2_MAX_MIN / (TUBE_SENSITIVITY_VALUE_NUM - 1)));
}

float getTubeSensitivity(void)
{
    uint8_t code = tubeTypeSourceFactorCodes[getTubeIndex(settings.tubeType)][settings.source];
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
        float value;
        if (getTubeDefaultHVFrequency(&value))
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

    int32_t tubeHVFrequency = (int32_t)log2f(value / 1250.0F);

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
        float value;
        if (getTubeDefaultHVDutyCycle(&value))
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

static cstring sourceMenuOptions[] = {
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
};

static const char *onSourceMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.source);

    return getString(sourceMenuOptions[index]);
}

static void onSourceMenuSelect(menu_size_t index)
{
    settings.source = index;

    updateDoseUnits();
}

static MenuState sourceMenuState;

const Menu sourceMenu = {
    STRING_SOURCE,
    &sourceMenuState,
    ARRAY_SIZE(sourceMenuOptions),
    onSourceMenuGetOption,
    onSourceMenuSelect,
    showMeasurementsMenu,
};

void showSourceMenu(void)
{
    showMenu(&sourceMenu);
}

// Tube type menu

static cstring tubeTypeMenuOptions[] = {
    STRING_J305,
    STRING_M4011,
    STRING_J321,
    STRING_HH614,
    STRING_SBM20,
    STRING_SI3BG,
    STRING_LND7317,
};

static const char *onTubeTypeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeType);

    return tubeTypeMenuOptions[index];
}

static void onTubeTypeMenuSelect(menu_size_t index)
{
    settings.tubeType = index;

    updateDoseUnits();
}

static MenuState tubeTypeMenuState;

static const Menu tubeTypeMenu = {
    STRING_TUBE_TYPE,
    &tubeTypeMenuState,
    ARRAY_SIZE(tubeTypeMenuOptions),
    onTubeTypeMenuGetOption,
    onTubeTypeMenuSelect,
    showTubeMenu,
};

// Sensitivity menu

static const char *onTubeSensitivityMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeSensitivity);

    if (index == 0)
        return getString(STRING_TUBE_DEFAULT);

    strclr(menuOption);
    strcatFloat(menuOption, getTubeSensitivityForIndex(index), 2);
    strcatChar(menuOption, ' ');
    strcat(menuOption, getString(STRING_CPMUSVH));

    return menuOption;
}

static void onTubeSensitivityMenuSelect(menu_size_t index)
{
    settings.tubeSensitivity = index;

    updateDoseUnits();
}

static MenuState tubeSensitivityMenuState;

static const Menu tubeSensitivityMenu = {
    STRING_SENSITIVITY,
    &tubeSensitivityMenuState,
    TUBE_SENSITIVITY_NUM,
    onTubeSensitivityMenuGetOption,
    onTubeSensitivityMenuSelect,
    showTubeMenu,
};

// Dead-time compensation menu

static const char *onTubeDeadTimeCompensationMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeDeadTimeCompensation);

    if (index == 0)
        return getString(STRING_OFF);

    strclr(menuOption);
    strcatFloat(menuOption, 1000000.0F * getTubeDeadTimeCompensationForIndex(index), 2);
    strcatChar(menuOption, ' ');
    strcat(menuOption, getString(STRING_MICROSECONDS));

    return menuOption;
}

static void onTubeDeadTimeCompensationMenuSelect(menu_size_t index)
{
    settings.tubeDeadTimeCompensation = index;
}

static MenuState tubeDeadTimeCompensationMenuState;

static const Menu tubeDeadTimeCompensationMenu = {
    STRING_DEAD_TIME_COMPENSATION,
    &tubeDeadTimeCompensationMenuState,
    TUBE_DEADTIMECOMPENSATION_NUM,
    onTubeDeadTimeCompensationMenuGetOption,
    onTubeDeadTimeCompensationMenuSelect,
    showTubeMenu,
};

#if defined(TUBE_HV_PWM)

// HV frequency menu

static const char *onHVFrequencyMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVFrequency);

    strclr(menuOption);
    strcatFloat(menuOption, getTubeHVFrequencyForIndex(index) / 1000.0F, 2);
    strcatChar(menuOption, ' ');
    strcat(menuOption, getString(STRING_KHZ));

    return menuOption;
}

static void onHVFrequencyMenuSelect(menu_size_t index)
{
    setTubeHVFrequency(getTubeHVFrequencyForIndex(index));
}

static MenuState hvFrequencyMenuState;

static const Menu hvFrequencyMenu = {
    STRING_PWMFREQUENCY,
    &hvFrequencyMenuState,
    TUBE_HVFREQUENCY_NUM,
    onHVFrequencyMenuGetOption,
    onHVFrequencyMenuSelect,
    showHVCustomProfileMenu,
};

// HV duty cycle menu

static const char *onHVDutyCycleMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVDutyCycle);

    strclr(menuOption);
    strcatFloat(menuOption, 100.0F * getTubeHVDutyCycleForIndex(index), 2);
    strcat(menuOption, STRING_PERCENT);

    return menuOption;
}

static void onHVDutyCycleMenuSelect(menu_size_t index)
{
    setTubeHVDutyCycle(getTubeHVDutyCycleForIndex(index));
}

static MenuState hvDutyCycleMenuState;

static const Menu hvDutyCycleMenu = {
    STRING_PWMDUTY_CYCLE,
    &hvDutyCycleMenuState,
    TUBE_HVDUTYCYCLE_NUM,
    onHVDutyCycleMenuGetOption,
    onHVDutyCycleMenuSelect,
    showHVCustomProfileMenu,
};

// HV custom profile menu

static const MenuOption hvCustomProfileMenuOptions[] = {
    {STRING_PWMFREQUENCY, &hvFrequencyMenu},
    {STRING_PWMDUTY_CYCLE, &hvDutyCycleMenu},
};

static const char *onHVCustomProfileMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(hvCustomProfileMenuOptions[index].title);
}

static void onHVCustomProfileMenuSelect(menu_size_t index)
{
    showMenu(hvCustomProfileMenuOptions[index].menu);
}

static MenuState hvCustomProfileMenuState;

static const Menu hvCustomProfileMenu = {
    STRING_CUSTOM,
    &hvCustomProfileMenuState,
    ARRAY_SIZE(hvCustomProfileMenuOptions),
    onHVCustomProfileMenuGetOption,
    onHVCustomProfileMenuSelect,
    showHVProfilesMenu,
};

static void showHVCustomProfileMenu(void)
{
    showMenu(&hvCustomProfileMenu);
}

// HV custom profile warning

static void onHVCustomProfileWarningEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        showHVProfilesMenu();

        break;

    case EVENT_KEY_SELECT:
        settings.tubeHVProfile = TUBE_HVPROFILE_CUSTOM;

        updateTubeHV();

        showHVCustomProfileMenu();

        break;

    case EVENT_DRAW:
        drawNotification(getString(STRING_CUSTOM), getString(STRING_NOTIFICATION_HVCUSTOM));

        break;

    default:
        break;
    }
}

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
};

static const char *onHVProfilesMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.tubeHVProfile);

    if (index == TUBE_HVPROFILE_CUSTOM)
        *menuStyle |= MENUSTYLE_SUBMENU;

    return getString(hvProfilesMenuOptions[index]);
}

static void onHVProfilesMenuSelect(menu_size_t index)
{
    if (index != TUBE_HVPROFILE_CUSTOM)
    {
        settings.tubeHVProfile = index;

        updateTubeHV();
    }
    else
    {
        if (settings.tubeHVProfile != index)
            showView(onHVCustomProfileWarningEvent);
        else
            showHVCustomProfileMenu();
    }
}

static MenuState hvProfilesMenuState;

static const Menu hvProfilesMenu = {
    STRING_HVPROFILE,
    &hvProfilesMenuState,
    ARRAY_SIZE(hvProfilesMenuOptions),
    onHVProfilesMenuGetOption,
    onHVProfilesMenuSelect,
    showTubeMenu,
};

static void showHVProfilesMenu(void)
{
    showMenu(&hvProfilesMenu);
}

#endif

// Tube menu

static const MenuOption tubeMenuOptions[] = {
    {STRING_TUBE_TYPE, &tubeTypeMenu},
    {STRING_SENSITIVITY, &tubeSensitivityMenu},
    {STRING_DEAD_TIME_COMPENSATION, &tubeDeadTimeCompensationMenu},
#if defined(TUBE_HV_PWM)
    {STRING_HVPROFILE, &hvProfilesMenu},
#endif
};

static const char *onTubeMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(tubeMenuOptions[index].title);
}

static void onTubeMenuSelect(menu_size_t index)
{
    showMenu(tubeMenuOptions[index].menu);
}

static MenuState tubeMenuState;

static const Menu tubeMenu = {
    STRING_GEIGER_TUBE,
    &tubeMenuState,
    ARRAY_SIZE(tubeMenuOptions),
    onTubeMenuGetOption,
    onTubeMenuSelect,
    showSettingsMenu,
};

void showTubeMenu(void)
{
    showMenu(&tubeMenu);
}
