/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if !defined(SETTINGS_H)
#define SETTINGS_H

#include <stdint.h>

#include "menu.h"
#include "view.h"

typedef struct
{
    uint32_t time;
    uint32_t pulseCount;
} Dose;

enum
{
    UNITS_SIEVERTS,
    UNITS_REM,
    UNITS_CPM,
    UNITS_CPS,

    UNITS_NUM,
};

enum
{
    AVERAGETIMER_UNLIMITED,
    AVERAGETIMER_5M,
    AVERAGETIMER_10M,
    AVERAGETIMER_30M,
    AVERAGETIMER_60M,

    AVERAGETIMER_NUM,
};

enum
{
    RATEALARM_OFF,
    RATEALARM_0_5,
    RATEALARM_1,
    RATEALARM_2,
    RATEALARM_5,
    RATEALARM_10,
    RATEALARM_20,
    RATEALARM_50,
    RATEALARM_100,

    RATEALARM_NUM,
};

enum
{
    DOSEALARM_OFF,
    DOSEALARM_5,
    DOSEALARM_10,
    DOSEALARM_20,
    DOSEALARM_50,
    DOSEALARM_100,
    DOSEALARM_200,
    DOSEALARM_500,
    DOSEALARM_1000,

    DOSEALARM_NUM,
};

enum
{
    DATALOGGING_OFF,
    DATALOGGING_60M,
    DATALOGGING_30M,
    DATALOGGING_10M,
    DATALOGGING_5M,
    DATALOGGING_1M,

    DATALOGGING_NUM,
};

enum
{
    TUBE_CONVERSIONFACTOR_J305,
    TUBE_CONVERSIONFACTOR_J321,
    TUBE_CONVERSIONFACTOR_J613,
    TUBE_CONVERSIONFACTOR_J614,
    TUBE_CONVERSIONFACTOR_M4011,
    TUBE_CONVERSIONFACTOR_SBM20,

    TUBE_CONVERSIONFACTOR_NUM,
};

#if defined(SIMULATOR)

#define TUBE_CONVERSIONFACTOR_DEFAULT TUBE_CONVERSIONFACTOR_M4011

#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 40000
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.5F

#define TUBE_HVPROFILE_ACCURACY_FREQUENCY 10000
#define TUBE_HVPROFILE_ACCURACY_DUTYCYCLE 0.1F

#define TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY 2500
#define TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE 0.03F

#elif defined(FS2011) || defined(FS600) || defined(FS1000)

#define TUBE_CONVERSIONFACTOR_DEFAULT TUBE_CONVERSIONFACTOR_J614

#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 40000
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.5F

#define TUBE_HVPROFILE_ACCURACY_FREQUENCY 2500
#define TUBE_HVPROFILE_ACCURACY_DUTYCYCLE 0.065F

#define TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY 2500
#define TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE 0.03F

#elif defined(GC01)

#if defined(CH32)
#define TUBE_CONVERSIONFACTOR_DEFAULT TUBE_CONVERSIONFACTOR_J321
#elif defined(APM32)
#define TUBE_CONVERSIONFACTOR_DEFAULT TUBE_CONVERSIONFACTOR_J613
#endif

#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 9207.16F
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.75F

#define TUBE_HVPROFILE_FACTORYDEFAULT2_FREQUENCY 47058.82F
#define TUBE_HVPROFILE_FACTORYDEFAULT2_DUTYCYCLE 0.5F

#endif

enum
{
    TUBE_HVPROFILE_FACTORYDEFAULT,
#if defined(TUBE_HVPROFILE_FACTORYDEFAULT2_FREQUENCY)
    TUBE_HVPROFILE_FACTORYDEFAULT2,
#endif
#if defined(TUBE_HVPROFILE_ACCURACY_FREQUENCY)
    TUBE_HVPROFILE_ACCURACY,
#endif
#if defined(TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY)
    TUBE_HVPROFILE_ENERGYSAVING,
#endif
    TUBE_HVPROFILE_CUSTOM,

    TUBE_HVPROFILE_NUM,
};

enum
{
    TUBE_HVFREQUENCY_1_25,
    TUBE_HVFREQUENCY_2_5,
    TUBE_HVFREQUENCY_5,
    TUBE_HVFREQUENCY_10,
    TUBE_HVFREQUENCY_20,
    TUBE_HVFREQUENCY_40,

    TUBE_HVFREQUENCY_NUM,
};

#define TUBE_HVDUTYCYCLE_MIN 0.0025F
#define TUBE_HVDUTYCYCLE_MAX 0.9F
#define TUBE_HVDUTYCYCLE_STEP 0.0025F
#define TUBE_HVDUTYCYCLE_NUM ((uint32_t)((TUBE_HVDUTYCYCLE_MAX -   \
                                          TUBE_HVDUTYCYCLE_MIN +   \
                                          TUBE_HVDUTYCYCLE_STEP) / \
                                         TUBE_HVDUTYCYCLE_STEP))

#if defined(PULSELED)
enum
{
    PULSELED_OFF,
    PULSELED_ON,

    PULSELED_NUM,
};
#endif

enum
{
    PULSE_CLICKS_OFF,
    PULSE_CLICKS_QUIET,
    PULSE_CLICKS_LOUD,

    PULSE_CLICKS_NUM,
};

#if defined(DISPLAY_MONOCHROME)

#define DISPLAY_CONTRAST_DEFAULT 4
#define DISPLAY_CONTRAST_NUM 8

#elif defined(DISPLAY_COLOR)

enum
{
    DISPLAY_THEME_DAY,
    DISPLAY_THEME_DUSK,
    DISPLAY_THEME_NIGHT,

    DISPLAY_THEME_NUM,
};

#endif

enum
{
#if defined(DISPLAY_MONOCHROME)
    DISPLAY_SLEEP_ALWAYS_OFF,
#endif
    DISPLAY_SLEEP_30S,
    DISPLAY_SLEEP_1M,
    DISPLAY_SLEEP_2M,
    DISPLAY_SLEEP_5M,
#if defined(DISPLAY_MONOCHROME)
    DISPLAY_SLEEP_PULSE_FLASHES,
#endif
    DISPLAY_SLEEP_ALWAYS_ON,

    DISPLAY_SLEEP_NUM,
};

enum
{
    DISPLAY_BRIGHTNESS_LOW,
    DISPLAY_BRIGHTNESS_MEDIUM,
    DISPLAY_BRIGHTNESS_HIGH,
    DISPLAY_BRIGHTNESS_VERYHIGH,

    DISPLAY_BRIGHTNESS_NUM,
};

enum
{
    RTC_TIMEZONE_M1200,
    RTC_TIMEZONE_M1100,
    RTC_TIMEZONE_M1000,
    RTC_TIMEZONE_M0900,
    RTC_TIMEZONE_M0800,
    RTC_TIMEZONE_M0700,
    RTC_TIMEZONE_M0600,
    RTC_TIMEZONE_M0500,
    RTC_TIMEZONE_M0400,
    RTC_TIMEZONE_M0300,
    RTC_TIMEZONE_M0200,
    RTC_TIMEZONE_M0100,
    RTC_TIMEZONE_P0000,
    RTC_TIMEZONE_P0100,
    RTC_TIMEZONE_P0200,
    RTC_TIMEZONE_P0300,
    RTC_TIMEZONE_P0400,
    RTC_TIMEZONE_P0500,
    RTC_TIMEZONE_P0600,
    RTC_TIMEZONE_P0700,
    RTC_TIMEZONE_P0800,
    RTC_TIMEZONE_P0900,
    RTC_TIMEZONE_P1000,
    RTC_TIMEZONE_P1100,
    RTC_TIMEZONE_P1200,
    RTC_TIMEZONE_P1300,
    RTC_TIMEZONE_P1400,

    RTC_TIMEZONE_NUM,
};

#if defined(BATTERY_REMOVABLE)
enum
{
    BATTERYTYPE_NI_MH,
    BATTERYTYPE_ALKALINE,

    BATTERYTYPE_NUM,
};
#endif

enum
{
    GAMESTRENGTH_1,
    GAMESTRENGTH_2,
    GAMESTRENGTH_3,
    GAMESTRENGTH_4,
    GAMESTRENGTH_5,
    GAMESTRENGTH_6,
    GAMESTRENGTH_7,
    GAMESTRENGTH_8,

    GAMESTRENGTH_NUM,
};

typedef struct
{
    unsigned int entryEmpty : 1;

    unsigned int units : 2;
    unsigned int averageTimer : 3;
    unsigned int rateAlarm : 4;
    unsigned int doseAlarm : 4;
    unsigned int datalogInterval : 3;
    unsigned int tubeConversionFactor : 7;
    unsigned int tubeDeadTimeCompensation : 6;
    unsigned int tubeHVProfile : 3;
    unsigned int tubeHVFrequency : 3;
    unsigned int tubeHVDutyCycle : 9;

#if defined(PULSELED)
    unsigned int pulseLED : 1;
#endif
    unsigned int pulseClicks : 2;

#if defined(DISPLAY_MONOCHROME)
    unsigned int displayContrast : 3;
#elif defined(DISPLAY_COLOR)
    unsigned int displayTheme : 2;
#endif
    unsigned int displayBrightness : 2;
    unsigned int displaySleep : 3;

    unsigned int rtcTimeZone : 5;

#if defined(BATTERY_REMOVABLE)
    unsigned int batteryType : 1;
#endif

    unsigned int gameStrength : 3;
} Settings;

extern Settings settings;

extern const View settingsMenuView;

void initSettings(void);

void writeSettings(void);

void onSettingsSubMenuBack(const Menu *menu);

#endif
