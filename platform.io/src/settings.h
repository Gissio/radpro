/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#if !defined(SETTINGS_H)

#define SETTINGS_H

#include <stdint.h>

#include "menu.h"
#include "view.h"

struct Dose
{
    uint32_t time;
    uint32_t pulseCount;
};

enum
{
    UNITS_SETTING_SIEVERTS,
    UNITS_SETTING_REM,
    UNITS_SETTING_CPM,
    UNITS_SETTING_CPS,

    UNITS_SETTING_NUM,
};

enum
{
    AVERAGE_TIMER_UNLIMITED,
    AVERAGE_TIMER_5M,
    AVERAGE_TIMER_10M,
    AVERAGE_TIMER_30M,
    AVERAGE_TIMER_60M,

    AVERAGE_TIMER_NUM,
};

enum
{
    RATE_ALARM_OFF,
    RATE_ALARM_0_5,
    RATE_ALARM_1,
    RATE_ALARM_2,
    RATE_ALARM_5,
    RATE_ALARM_10,
    RATE_ALARM_20,
    RATE_ALARM_50,
    RATE_ALARM_100,

    RATE_ALARM_NUM,
};

enum
{
    DOSE_ALARM_OFF,
    DOSE_ALARM_5,
    DOSE_ALARM_10,
    DOSE_ALARM_20,
    DOSE_ALARM_50,
    DOSE_ALARM_100,
    DOSE_ALARM_200,
    DOSE_ALARM_500,
    DOSE_ALARM_1000,

    DOSE_ALARM_NUM,
};

enum
{
    PULSE_CLICKS_OFF,
    PULSE_CLICKS_QUIET,
    PULSE_CLICKS_LOUD,

    PULSE_CLICKS_NUM,
};

#if defined(PULSE_LED)

enum
{
    PULSE_LED_OFF,
    PULSE_LED_ON,

    PULSE_LED_NUM,
};

#endif

#if defined(DISPLAY_MONO)

enum
{
    DISPLAY_BACKLIGHT_ALWAYS_OFF,
    DISPLAY_BACKLIGHT_30S,
    DISPLAY_BACKLIGHT_1M,
    DISPLAY_BACKLIGHT_2M,
    DISPLAY_BACKLIGHT_5M,
    DISPLAY_BACKLIGHT_PULSE_FLASHES,
    DISPLAY_BACKLIGHT_ALWAYS_ON,

    DISPLAY_BACKLIGHT_NUM,
};

#elif defined(DISPLAY_COLOR)

enum
{
    DISPLAY_THEME_DARK,
    DISPLAY_THEME_LIGHT,

    DISPLAY_THEME_NUM,
};

enum
{
    DISPLAY_BRIGHTNESS_LOW,
    DISPLAY_BRIGHTNESS_MEDIUM,
    DISPLAY_BRIGHTNESS_HIGH,

    DISPLAY_BRIGHTNESS_NUM,
};

enum
{
    DISPLAY_SLEEP_30S,
    DISPLAY_SLEEP_1M,
    DISPLAY_SLEEP_2M,
    DISPLAY_SLEEP_5M,
    DISPLAY_SLEEP_ALWAYS_ON,

    DISPLAY_SLEEP_NUM,
};

#endif

enum
{
    DATA_LOGGING_OFF,
    DATA_LOGGING_60M,
    DATA_LOGGING_30M,
    DATA_LOGGING_10M,
    DATA_LOGGING_5M,
    DATA_LOGGING_1M,

    DATA_LOGGING_NUM,
};

enum
{
    BATTERY_TYPE_NI_MH,
    BATTERY_TYPE_ALKALINE,

    BATTERY_TYPE_NUM,
};

enum
{
#if defined(SDLSIM) || defined(FS2011)

    TUBE_TYPE_M4011,
    TUBE_TYPE_J305,

#endif

    TUBE_TYPE_HH614,

    TUBE_TYPE_CUSTOM,

    TUBE_TYPE_NUM,
};

#define TUBE_HV_DUTY_CYCLE_NUM 100

#define TUBE_HV_FREQUENCY_NUM 6

enum
{
    GAME_STRENGTH_1,
    GAME_STRENGTH_2,
    GAME_STRENGTH_3,
    GAME_STRENGTH_4,
    GAME_STRENGTH_5,
    GAME_STRENGTH_6,
    GAME_STRENGTH_7,
    GAME_STRENGTH_8,

    GAME_STRENGTH_NUM,
};

struct Settings
{
    unsigned int empty : 1;

    unsigned int units : 2;
    unsigned int averageTimer : 3;
    unsigned int rateAlarm : 4;
    unsigned int doseAlarm : 4;

    unsigned int pulseClicks : 2;

#if defined(PULSE_LED)

    unsigned int pulseLED : 1;

#endif

#if defined(DISPLAY_MONO)

    unsigned int displayBacklight : 3;

#elif defined(DISPLAY_COLOR)

    unsigned int displayTheme : 1;
    unsigned int displayBrightnessLevel : 2;
    unsigned int displaySleep : 3;

#endif

    unsigned int datalogInterval : 3;

    unsigned int batteryType : 1;

    unsigned int tubeType : 2;
    unsigned int tubeConversionFactor : 6;
    unsigned int tubeDeadTimeCompensation : 6;
    unsigned int tubeHVDutyCycle : 7;
    unsigned int tubeHVFrequency : 3;

    unsigned int gameStrength : 3;
};

extern struct Settings settings;

extern const struct View settingsMenuView;

void initSettings(void);

void writeSettings(void);

void onSettingsSubMenuBack(const struct Menu *menu);

#endif
