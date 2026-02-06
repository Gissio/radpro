/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if !defined(SETTINGS_H)
#define SETTINGS_H

#include <stdint.h>

// Settings

enum
{
    RATE_OFF,
    RATE_0_5_USVH,
    RATE_1_USVH,
    RATE_2_USVH,
    RATE_5_USVH,
    RATE_10_USVH,
    RATE_20_USVH,
    RATE_50_USVH,
    RATE_100_USVH,
    RATE_200_USVH,
    RATE_500_USVH,
    RATE_1000_USVH,
    RATE_2000_USVH,
    RATE_5000_USVH,
    RATE_10000_USVH,
    RATE_20000_USVH,

    RATE_NUM,
};

enum
{
    DOSE_OFF,
    DOSE_2_USV,
    DOSE_5_USV,
    DOSE_10_USV,
    DOSE_20_USV,
    DOSE_50_USV,
    DOSE_100_USV,
    DOSE_200_USV,
    DOSE_500_USV,
    DOSE_1000_USV,
    DOSE_2000_USV,
    DOSE_5000_USV,
    DOSE_10000_USV,
    DOSE_20000_USV,
    DOSE_50000_USV,
    DOSE_100000_USV,

    DOSE_NUM,
};

enum
{
    ELECTRIC_FIELD_OFF,
    ELECTRIC_FIELD_50_V_M,
    ELECTRIC_FIELD_100_V_M,
    ELECTRIC_FIELD_200_V_M,
    ELECTRIC_FIELD_500_V_M,
    ELECTRIC_FIELD_1000_V_M,
    ELECTRIC_FIELD_2000_V_M,
    ELECTRIC_FIELD_5000_V_M,

    ELECTRIC_FIELD_NUM,
};

enum
{
    MAGNETIC_FIELD_OFF,
    MAGNETIC_FIELD_1_UT,
    MAGNETIC_FIELD_2_UT,
    MAGNETIC_FIELD_5_UT,
    MAGNETIC_FIELD_10_UT,
    MAGNETIC_FIELD_20_UT,
    MAGNETIC_FIELD_50_UT,
    MAGNETIC_FIELD_100_UT,

    MAGNETIC_FIELD_NUM,
};

enum
{
    DOSE_UNITS_SIEVERTS,
    DOSE_UNITS_REM,
    DOSE_UNITS_CPM,
    DOSE_UNITS_CPS,

    DOSE_UNITS_NUM,
};

enum {
    SOURCE_CS137,
    SOURCE_CO60,
    SOURCE_TC99M,
    SOURCE_I131,
    SOURCE_LU177,
    SOURCE_AM241,
    SOURCE_RADIUM,
    SOURCE_URANIUM_ORE,
    SOURCE_URANIUM_GLAZE,
    SOURCE_DEPLETED_URANIUM,
    SOURCE_THORIUM_ORE,
    SOURCE_XRAY_TUBE,
    SOURCE_K40,
    SOURCE_NATURAL_BACKGROUND,

    SOURCE_NUM,
};

enum
{
    INSTANTANEOUSAVERAGING_ADAPTIVEFAST,
    INSTANTANEOUSAVERAGING_ADAPTIVEPRECISION,
    INSTANTANEOUSAVERAGING_60_SECONDS,
    INSTANTANEOUSAVERAGING_30_SECONDS,
    INSTANTANEOUSAVERAGING_10_SECONDS,

    INSTANTANEOUSAVERAGING_NUM,
};

enum
{
    AVERAGING_UNLIMITED,
    AVERAGING_24_HOURS,
    AVERAGING_12_HOURS,
    AVERAGING_6_HOURS,
    AVERAGING_3_HOURS,
    AVERAGING_1_HOUR,
    AVERAGING_30_MINUTES,
    AVERAGING_10_MINUTES,
    AVERAGING_5_MINUTES,
    AVERAGING_1_MINUTE,
    AVERAGING_30_SECONDS,
    AVERAGING_10_SECONDS,
    AVERAGING_5_SECONDS,
    AVERAGING_1_SECOND,
    AVERAGING_TIME_NUM,

    AVERAGING_CONFIDENCE_50 = AVERAGING_TIME_NUM,
    AVERAGING_CONFIDENCE_20,
    AVERAGING_CONFIDENCE_10,
    AVERAGING_CONFIDENCE_5,
    AVERAGING_CONFIDENCE_2,
    AVERAGING_CONFIDENCE_1,

    AVERAGING_NUM,
};

enum
{
    MAGNETIC_FIELD_UNITS_TESLA,
    MAGNETIC_FIELD_UNITS_GAUSS,

    MAGNETIC_FIELD_UNITS_NUM,
};

enum
{
    TUBE_TYPE_J305,
    TUBE_TYPE_M4011,
    TUBE_TYPE_HH614,
    TUBE_TYPE_SBM20,
    TUBE_TYPE_SI3BG,
    TUBE_TYPE_LND7317,

    TUBE_TYPE_NUM,
};

#define TUBE_SENSITIVITY_VALUE_MIN 1.0F
#define TUBE_SENSITIVITY_VALUE_MAX 10000.01F
#define TUBE_SENSITIVITY_VALUE_LOG2_MAX_MIN 13.28771237F
#define TUBE_SENSITIVITY_VALUE_NUM 241
#define TUBE_SENSITIVITY_NUM (TUBE_SENSITIVITY_VALUE_NUM + 1)

#define TUBE_DEADTIMECOMPENSATION_VALUE_MIN 0.000005F
#define TUBE_DEADTIMECOMPENSATION_VALUE_MAX 0.000500F
#define TUBE_DEADTIMECOMPENSATION_VALUE_LOG2_MAX_MIN 6.643856189F
#define TUBE_DEADTIMECOMPENSATION_VALUE_NUM 121
#define TUBE_DEADTIMECOMPENSATION_NUM (TUBE_DEADTIMECOMPENSATION_VALUE_NUM + 1)

#if defined(SIMULATOR)
#define TUBE_TYPE_DEFAULT TUBE_TYPE_M4011
#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 40000
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.5F
#define TUBE_HVPROFILE_ACCURACY_FREQUENCY 10000
#define TUBE_HVPROFILE_ACCURACY_DUTYCYCLE 0.1F
#define TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY 2500
#define TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE 0.03F
#elif defined(FS2011) || defined(FS600) || defined(FS1000)
#define TUBE_TYPE_DEFAULT TUBE_TYPE_HH614
#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 40000
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.5F
#define TUBE_HVPROFILE_ACCURACY_FREQUENCY 2500
#define TUBE_HVPROFILE_ACCURACY_DUTYCYCLE 0.065F
#define TUBE_HVPROFILE_ENERGYSAVING_FREQUENCY 2500
#define TUBE_HVPROFILE_ENERGYSAVING_DUTYCYCLE 0.03F
#elif defined(FS5000)
#define TUBE_TYPE_DEFAULT TUBE_TYPE_M4011
#elif defined(GC01)
#define TUBE_TYPE_DEFAULT TUBE_TYPE_M4011
#if defined(CH32)
#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 9207.16F
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.75F
#elif defined(APM32)
#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 50000.0F
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.5F
#endif
#elif defined(GC03)
#define TUBE_TYPE_DEFAULT TUBE_TYPE_M4011
#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 33000.0F
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.4125F
#elif defined(GMC800)
#define TUBE_TYPE_DEFAULT TUBE_TYPE_M4011
#define TUBE_HVPROFILE_FACTORYDEFAULT_FREQUENCY 2500
#define TUBE_HVPROFILE_FACTORYDEFAULT_DUTYCYCLE 0.163F
#endif

enum
{
    TUBE_HVPROFILE_FACTORYDEFAULT,
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
    TUBE_HVFREQUENCY_1_25_KHZ,
    TUBE_HVFREQUENCY_2_5_KHZ,
    TUBE_HVFREQUENCY_5_KHZ,
    TUBE_HVFREQUENCY_10_KHZ,
    TUBE_HVFREQUENCY_20_KHZ,
    TUBE_HVFREQUENCY_40_KHZ,

    TUBE_HVFREQUENCY_NUM,
};

#define TUBE_HVDUTYCYCLE_MIN 0.0025F
#define TUBE_HVDUTYCYCLE_MAX 0.9F
#define TUBE_HVDUTYCYCLE_STEP 0.0025F
#define TUBE_HVDUTYCYCLE_NUM ((uint32_t)((TUBE_HVDUTYCYCLE_MAX -   \
                                          TUBE_HVDUTYCYCLE_MIN +   \
                                          TUBE_HVDUTYCYCLE_STEP) / \
                                         TUBE_HVDUTYCYCLE_STEP))

enum
{
    DATALOG_LOGGINGMODE_OFF,
    DATALOG_LOGGINGMODE_60_MINUTES,
    DATALOG_LOGGINGMODE_10_MINUTES,
    DATALOG_LOGGINGMODE_1_MINUTE,
    DATALOG_LOGGINGMODE_10_SECONDS,
    DATALOG_LOGGINGMODE_1_SECOND,

    DATALOG_LOGGINGMODE_NUM,
};

#define DISPLAY_CONTRAST_DEFAULT 4
#define DISPLAY_CONTRAST_NUM 8

enum
{
    DISPLAY_THEME_DAY,
    DISPLAY_THEME_DUSK,
    DISPLAY_THEME_NIGHT,

    DISPLAY_THEME_NUM,
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
#if defined(DISPLAY_MONOCHROME)
    DISPLAY_SLEEP_ALWAYS_OFF,
#endif
    DISPLAY_SLEEP_10_SECONDS,
    DISPLAY_SLEEP_30_SECONDS,
    DISPLAY_SLEEP_1_MINUTE,
    DISPLAY_SLEEP_2_MINUTES,
    DISPLAY_SLEEP_5_MINUTES,
    DISPLAY_SLEEP_ALWAYS_ON,

    DISPLAY_SLEEP_NUM,
};

enum
{
    SOUND_PULSETYPE_CLICKS,
    SOUND_PULSETYPE_CHIRPS,
    SOUND_PULSETYPE_BEEPS,

    SOUND_PULSETYPE_NUM,
};

enum
{
    SOUND_ALERTSTYLE_SHORT,
    SOUND_ALERTSTYLE_LONG,

    SOUND_ALERTSTYLE_NUM,
};

enum
{
    SOUND_ALERTVOLUME_LOW,
    SOUND_ALERTVOLUME_MEDIUM,
    SOUND_ALERTVOLUME_HIGH,
    SOUND_ALERTVOLUME_VERYHIGH,

    SOUND_ALERTVOLUME_NUM,
};

enum
{
    SOUND_VOICEVOLUME_LOW,
    SOUND_VOICEVOLUME_MEDIUM,
    SOUND_VOICEVOLUME_HIGH,
    SOUND_VOICEVOLUME_VERYHIGH,

    SOUND_VOICEVOLUME_NUM,
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

enum
{
    RTC_TIMEFORMAT_24_HOUR,
    RTC_TIMEFORMAT_12_HOUR,

    RTC_TIMEFORMAT_NUM,
};

enum
{
#if defined BATTERY_REMOVABLE
    BATTERYTYPE_NI_MH,
    BATTERYTYPE_ALKALINE,
#else
    BATTERYTYPE_LI_ION,
#endif

    BATTERYTYPE_NUM,
};

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

typedef struct
{
    unsigned int empty : 1;

    unsigned int pulseSound : 1;
    unsigned int pulseVibration : 1;
    unsigned int pulseLED : 1;
    unsigned int pulseDisplayFlash : 1;
    unsigned int pulseThreshold : 4;

    unsigned int source : 4;
    unsigned int doseUnits : 2;
    unsigned int secondaryDoseUnits : 2;
    unsigned int instantaneousAveraging : 3;
    unsigned int averaging : 5;
    unsigned int magneticFieldUnits : 1;

    unsigned int rateWarning : 4;
    unsigned int doseWarning : 4;
    unsigned int rateAlarm : 4;
    unsigned int doseAlarm : 4;
    unsigned int electricFieldAlarm : 3;
    unsigned int magneticFieldAlarm : 3;
    unsigned int alertSound : 1;
    unsigned int alertVoice : 1;
    unsigned int alertVibration : 1;
    unsigned int alertPulseLED : 1;
    unsigned int alertDisplayFlash : 1;

    unsigned int tubeType : 4;
    unsigned int tubeSensitivity : 8;
    unsigned int tubeDeadTimeCompensation : 7;
    unsigned int tubeHVProfile : 2;
    unsigned int tubeHVFrequency : 3;
    unsigned int tubeHVDutyCycle : 9;

    unsigned int loggingMode : 3;

    unsigned int displayContrast : 3;
    unsigned int displayTheme : 2;
    unsigned int displayBrightness : 2;
    unsigned int displaySleep : 3;

    unsigned int soundPulseType : 2;
    unsigned int soundAlertStyle : 1;
    unsigned int soundAlertVolume : 2;
    unsigned int soundVoiceVolume : 2;

    unsigned int rtcTimeZone : 5;
    unsigned int rtcTimeFormat : 1;

    unsigned int powerBatteryType : 1;
    unsigned int powerUSBAutoPowerOn : 1;

    unsigned int dataMode : 1;

    unsigned int gameStrength : 3;
} Settings
#if defined(__GNUC__)
    __attribute__((aligned(8)))
#endif
    ;

extern Settings settings;

void initSettings(void);
void setupSettings(void);

void saveSettings(void);

void setSettingsMenu(void);

#endif
