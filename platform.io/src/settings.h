/*
 * Rad Pro
 * Settings
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

#include "menu.h"
#include "view.h"

#define CONVERSION_FACTOR_MIN 25.0F
#define CONVERSION_FACTOR_MAX 400.01F
#define CONVERSION_FACTOR_LOG_MAX_MIN 4.0F
#define CONVERSION_FACTOR_STEPS 128

extern const char *const firmwareName;
extern const char *const firmwareVersion;

enum UnitsSetting
{
    UNITS_SIEVERTS,
    UNITS_REM,
    UNITS_CPM,
    UNITS_CPS,

    UNITS_NUM,
};

enum RateAlarmSetting
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

enum DoseAlarmSetting
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

enum HistorySetting
{
    HISTORY_2M,
    HISTORY_10M,
    HISTORY_1H,
    HISTORY_6H,
    HISTORY_24H,
    HISTORY_LAST = HISTORY_24H,

    HISTORY_NUM,
};

enum PulseClicksSetting
{
    PULSE_CLICKS_OFF,
    PULSE_CLICKS_QUIET,
    PULSE_CLICKS_LOUD,
};

enum BacklightSetting
{
    BACKLIGHT_OFF,
    BACKLIGHT_10S,
    BACKLIGHT_60S,
    BACKLIGHT_PULSE_FLASHES,
    BACKLIGHT_ON,
};

enum BatteryTypeSetting
{
    BATTERY_TYPE_NI_MH,
    BATTERY_TYPE_ALKALINE,
};

enum TubeTypeSetting
{
    TUBE_TYPE_M4011,
    TUBE_TYPE_HH614,
    TUBE_TYPE_J305,
    TUBE_TYPE_CUSTOM,
};

enum DataLoggingSetting
{
    DATA_LOGGING_60M,
    DATA_LOGGING_30M,
    DATA_LOGGING_10M,
    DATA_LOGGING_5M,
    DATA_LOGGING_1M,
};

enum GameSkillLevelSetting
{
    GAME_SKILL_LEVEL_1,
    GAME_SKILL_LEVEL_2,
    GAME_SKILL_LEVEL_3,
    GAME_SKILL_LEVEL_4,
    GAME_SKILL_LEVEL_5,
    GAME_SKILL_LEVEL_6,
    GAME_SKILL_LEVEL_7,
    GAME_SKILL_LEVEL_8,
};

struct Settings
{
    unsigned int units : 4;
    unsigned int history : 4;
    unsigned int rateAlarm : 4;
    unsigned int doseAlarm : 4;
    unsigned int pulseClicks : 4;
    unsigned int backlight : 4;
    unsigned int batteryType : 4;
    unsigned int tubeType : 4;
    unsigned int conversionFactor : 8;
    unsigned int dataLogging : 4;
    unsigned int gameSkillLevel : 4;

    uint8_t alignment[8];

    uint32_t lifeTime;
    uint32_t lifePulseCount;

    uint32_t doseTime;
    uint32_t dosePulseCount;
};

struct DataLogEntry
{
    uint32_t timestamp;
    uint32_t pulseCount;
};

extern struct Settings settings;

extern const struct View settingsMenuView;

void initSettings(void);

float getConversionFactor(uint32_t index);

void writeSettings(void);

void onSettingsSubMenuBack(const struct Menu *menu);

#endif
