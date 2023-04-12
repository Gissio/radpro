/*
 * FS2011 Pro
 * Settings
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

enum UnitsSetting
{
    UNITS_SIEVERTS,
    UNITS_REM,
    UNITS_CPM,
    UNITS_CPS,
    UNITS_NUM,
};

typedef struct
{
    char *const name;
    float scale;
    int minExponent;
} Unit;

typedef struct
{
    Unit rate;
    Unit dose;
} UnitType;

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

enum PulseSoundSetting
{
    PULSE_SOUND_OFF,
    PULSE_SOUND_QUIET,
    PULSE_SOUND_LOUD,
};

enum BacklightSetting
{
    BACKLIGHT_OFF,
    BACKLIGHT_10S,
    BACKLIGHT_60S,
    BACKLIGHT_ON,
};

enum BatteryTypeSetting
{
    BATTERY_NI_MH,
    BATTERY_ALKALINE,
};

enum TubeTypeSetting
{
    TUBE_HH614,
    TUBE_M4011,
    TUBE_SBM20,
    TUBE_SI3BG,
};

enum GameSkillLevelSetting
{
    GAME_SKILLLEVEL_1,
    GAME_SKILLLEVEL_2,
    GAME_SKILLLEVEL_3,
    GAME_SKILLLEVEL_4,
    GAME_SKILLLEVEL_5,
    GAME_SKILLLEVEL_6,
    GAME_SKILLLEVEL_7,
    GAME_SKILLLEVEL_8,
    GAME_SKILLLEVEL_NUM,
};

enum ValidStateSettings
{
    SETTING_VALID,
    SETTING_INVALID,
};

extern UnitType units[UNITS_NUM];

typedef struct
{
    unsigned int units : 2;
    unsigned int history : 3;
    unsigned int rateAlarm : 5;
    unsigned int doseAlarm : 5;
    unsigned int pulseSound : 2;
    unsigned int backlight : 2;
    unsigned int batteryType : 1;
    unsigned int tubeType : 4;
    unsigned int gameSkillLevel : 3;
    unsigned int validState : 1;

    uint32_t lifeTimer;
    uint64_t lifeCounts;
} Settings;

extern Settings settings;

void updateTubeType();

void readSettings();
void writeSettings();

float getRateAlarmSvH(uint32_t index);
float getDoseAlarmSv(uint32_t index);
int getBacklightTime(uint32_t index);

#endif
