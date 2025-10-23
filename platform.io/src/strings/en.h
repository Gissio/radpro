/*
 * Rad Pro
 * UI strings: English
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

// Application name (should not be translated)
#define STRING_APP_NAME "Rad Pro"

// Styling characters (should not be translated)
#define STRING_EMPTY ""
#define STRING_NUMBERS "0123456789.:"
#define STRING_ELLIPSIS "..."
#define STRING_PLUSMINUS "±"
#define STRING_PERCENT "%"
#define STRING_NONE "-"
#define STRING_LESSTHAN "< "

// Units (should not be translated)
#define STRING_NANO "n"
#define STRING_MICRO "µ"
#define STRING_MILLI "m"
#define STRING_KILO "k"
#define STRING_MEGA "M"
#define STRING_GIGA "G"
#define STRING_SIEVERT "Sievert"
#define STRING_SV "Sv"
#define STRING_SVH "Sv/h"
#define STRING_REM "rem"
#define STRING_REMH "rem/h"
#define STRING_CPM "cpm"
#define STRING_CPS "cps"
#define STRING_MICROSECONDS "µs"
#define STRING_VOLTS "V"
#define STRING_KHZ "kHz"
#define STRING_CPMUSVH "cpm/µSv/h"
#define STRING_TEMPERATURE "C"

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "count"
#define STRING_COUNTS "counts"

// Common time menu items
#define STRING_24_HOURS "24 hours"
#define STRING_12_HOURS "12 hours"
#define STRING_6_HOURS "6 hours"
#define STRING_3_HOURS "3 hours"
#define STRING_1_HOUR "1 hour"
#define STRING_30_MINUTES "30 minutes"
#define STRING_10_MINUTES "10 minutes"
#define STRING_5_MINUTES "5 minutes"
#define STRING_2_MINUTES "2 minutes"
#define STRING_1_MINUTE "1 minute"
#define STRING_30_SECONDS "30 seconds"
#define STRING_10_SECONDS "10 seconds"
#define STRING_5_SECONDS "5 seconds"
#define STRING_1_SECOND "1 second"

// Common menu items
#define STRING_OFF "Off"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Very low"
#define STRING_LOW "Low"
#define STRING_MEDIUM "Medium"
#define STRING_HIGH "High"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "WARNING\n\nFirmware checksum failure."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "WARNING\n\nWrong values may harm device."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Reset data log?\n\nAll records will be deleted."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCCESS\n\nData log has been reset."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Instantaneous"
#define STRING_AVERAGE "Average"
#define STRING_CUMULATIVE "Cumulative"
#define STRING_HISTORY "History"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 w"
#define STRING_HISTORY_1_MONTH "1 mo"
#define STRING_HISTORY_1_YEAR "1 y"

// Measurement view secondary view strings
#define STRING_TIME "Time"
#define STRING_MAX "Max"
#define STRING_RATE "Rate"
#define STRING_DOSE "Dose"

// Measurement view info strings
#define STRING_ALERT_ALARM "ALARM"
#define STRING_ALERT_WARNING "WARNING"
#define STRING_ALERT_FAULT "FAULT"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_DONE "DONE"

// Settings menu items
#define STRING_SETTINGS "Settings"
#define STRING_PULSES "Pulses"
#define STRING_ALERTS "Alerts"
#define STRING_MEASUREMENTS "Measurements"
#define STRING_GEIGER_TUBE "Geiger tube"
#define STRING_DATALOG "Data log"
#define STRING_DISPLAY "Display"
#define STRING_DATE_AND_TIME "Date and time"
#define STRING_BATTERY_TYPE "Battery type"
#define STRING_RANDOM_GENERATOR "Random generator"
#define STRING_GAME "Game"
#define STRING_STATISTICS "Statistics"
#define STRING_DATAMODE "Data mode"

// Pulses menu items
#define STRING_SOUND "Sound"
#define STRING_VOICE "Voice"
#define STRING_VIBRATION "Vibration"
#define STRING_PULSE_LED "Pulse LED"
#define STRING_DISPLAY_FLASH "Display flash"
#define STRING_THRESHOLD "Threshold"

// Alarms menu items
#define STRING_RATE_WARNING "Rate warning"
#define STRING_RATE_ALARM "Rate alarm"
#define STRING_DOSE_WARNING "Dose warning"
#define STRING_DOSE_ALARM "Dose alarm"
#define STRING_INDICATION "Indication"

// Measurements menu items
#define STRING_UNITS "Units"
#define STRING_SECONDARY_UNITS "Secondary units"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptive fast"
#define STRING_ADAPTIVE_PRECISION "Adaptive precision"

// Average menu items
#define STRING_UNLIMITED "Unlimited"
#define STRING_50_CONFIDENCE "±50% confidence"
#define STRING_20_CONFIDENCE "±20% confidence"
#define STRING_10_CONFIDENCE "±10% confidence"
#define STRING_5_CONFIDENCE "±5% confidence"
#define STRING_2_CONFIDENCE "±2% confidence"
#define STRING_1_CONFIDENCE "±1% confidence"

// Geiger tube menu items
#define STRING_SENSITIVITY "Sensitivity"
#define STRING_DEAD_TIME_COMPENSATION "Dead-time compensation"
#define STRING_HVPROFILE "HV profile"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (default)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Factory default"
#define STRING_ACCURACY "Accuracy"
#define STRING_ENERGY_SAVING "Energy-saving"
#define STRING_CUSTOM "Custom"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM frequency"
#define STRING_PWMDUTY_CYCLE "PWM duty cycle"

// Data Log menu items
#define STRING_LOGGING_MODE "Logging mode"
#define STRING_RESET "Reset"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Every hour"
#define STRING_EVERY10_MINUTES "Every 10 minutes"
#define STRING_EVERY_MINUTE "Every minute"
#define STRING_EVERY10_SECONDS "Every 10 seconds"
#define STRING_EVERY_SECOND "Every second"

// Display menu items
#define STRING_THEME "Theme"
#define STRING_BRIGHTNESS "Brightness"
#define STRING_CONTRAST "Contrast"
#define STRING_SLEEP "Sleep"

// Display - Theme menu items
#define STRING_THEME_DAY "Day"
#define STRING_THEME_DUSK "Dusk"
#define STRING_THEME_NIGHT "Night"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Level"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Always off"
#define STRING_ALWAYS_ON "Always on"

// Sound menu items
#define STRING_ALERTSTYLE "Alert style"
#define STRING_ALERTVOLUME "Alert volume"
#define STRING_VOICEVOLUME "Voice volume"

// Sound Alarms length
#define STRING_SHORT "Short"
#define STRING_LONG "Long"

// Sound pulses menu items
#define STRING_CLICKS "Clicks"
#define STRING_CHIRPS "Chirps"
#define STRING_BEEPS "Beeps"

// Time and date menu items
#define STRING_TIME_ZONE "Time zone"
#define STRING_YEAR "Year"
#define STRING_MONTH "Month"
#define STRING_DAY "Day"
#define STRING_HOUR "Hour"
#define STRING_MINUTE "Minute"
#define STRING_TIME_FORMAT "Time format"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-hour"
#define STRING_12_HOUR "12-hour"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkaline"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alphanumeric"
#define STRING_HEXADECIMAL "Hexadecimal"
#define STRING_DECIMAL "Decimal"
#define STRING_BINARY "Binary"
#define STRING_100_SIDED_DIE "100-sided die (0-99)"
#define STRING_20_SIDED_DIE "20-sided die"
#define STRING_12_SIDED_DIE "12-sided die"
#define STRING_10_SIDED_DIE "10-sided die (0-9)"
#define STRING_8_SIDED_DIE "8-sided die"
#define STRING_6_SIDED_DIE "6-sided die"
#define STRING_4_SIDED_DIE "4-sided die"
#define STRING_COIN_FLIP "Coin flip (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Play white"
#define STRING_PLAY_BLACK "Play black"
#define STRING_CONTINUE_GAME "Continue game"
#define STRING_NEW_GAME "New game"
#define STRING_STRENGTH "Strength"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Level"

// Statistics view items
#define STRING_TUBE_LIFE "Tube life"
#define STRING_TUBE "Tube"
#define STRING_LIFE "Life"
#define STRING_DEAD_TIME "Dead time"
#define STRING_DEVICE_ID "Device ID"
#define STRING_DEVICE "Device"
#define STRING_ID "ID"
#define STRING_BATTERY "Battery"
