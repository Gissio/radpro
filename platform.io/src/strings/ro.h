/*
 * Rad Pro
 * UI strings: Romanian
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

// Application name (should not be translated)
#define STRING_APP_NAME "Rad Pro"

// Styling characters (should not be translated)
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

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "puls."
#define STRING_COUNTS "puls."

// Common time menu items
#define STRING_24_HOURS "24 ore"
#define STRING_12_HOURS "12 ore"
#define STRING_6_HOURS "6 ore"
#define STRING_3_HOURS "3 ore"
#define STRING_1_HOUR "1 oră"
#define STRING_30_MINUTES "30 minute"
#define STRING_10_MINUTES "10 minute"
#define STRING_5_MINUTES "5 minute"
#define STRING_2_MINUTES "2 minute"
#define STRING_1_MINUTE "1 minut"
#define STRING_30_SECONDS "30 secunde"
#define STRING_10_SECONDS "10 secunde"
#define STRING_5_SECONDS "5 secunde"
#define STRING_1_SECOND "1 secundă"

// Common menu items
#define STRING_OFF "Oprit"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "AVERTIZARE"
#define STRING_NOTIFICATION_SUCCESS "SUCCES"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Eșec sumă control firmware."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Valori greșite dăunează."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Resetare?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Toate înregistrările șterse."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Jurnal date resetat."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Instantaneu"
#define STRING_AVERAGE "Medie"
#define STRING_CUMULATIVE "Cumulativ"
#define STRING_HISTORY10_MIN "Istoric (10 min)"
#define STRING_HISTORY1_H "Istoric (1 oră)"
#define STRING_HISTORY24_H "Istoric (24 ore)"

// Measurement view secondary view strings
#define STRING_TIME "Timp"
#define STRING_MAX "Max"
#define STRING_RATE "Rată"
#define STRING_DOSE "Doză"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ALARMĂ"
#define STRING_INFO_FAULT "EROARE"
#define STRING_INFO_MAX "MAX"
#define STRING_INFO_OVER "DEPĂȘ"
#define STRING_INFO_DONE "GATA"

// Settings menu items
#define STRING_SETTINGS "Setări"
#define STRING_PULSES "Pulsuri"
#define STRING_ALARMS "Alarme"
#define STRING_MEASUREMENTS "Măsurători"
#define STRING_GEIGER_TUBE "Tub Geiger"
#define STRING_DATA_LOG "Jurnal date"
#define STRING_DISPLAY "Afișaj"
#define STRING_DATE_AND_TIME "Dată și oră"
#define STRING_BATTERY_TYPE "Tip baterie"
#define STRING_RANDOM_GENERATOR "Generator aleator"
#define STRING_GAME "Joc"
#define STRING_STATISTICS "Statistici"
#define STRING_DATA_MODE "Mod date"

// Pulses menu items
#define STRING_SOUND "Sunet"
#define STRING_VIBRATION "Vibrație"
#define STRING_ALERT_LED "LED alertă"
#define STRING_PULSE_LED "LED puls"
#define STRING_DISPLAY_FLASH "Flash afișaj"
#define STRING_THRESHOLD "Prag"

// Sound menu items
#define STRING_CLICKS "Clicuri"
#define STRING_CHIRPS "Ciripituri"
#define STRING_BEEPS "Bipuri"

// Alarms menu items
#define STRING_RATE_ALARM "Alarmă rată"
#define STRING_DOSE_ALARM "Alarmă doză"
#define STRING_OVERRANGE_ALARM "Alarmă depășire"
#define STRING_INDICATION "Indicație"

// Measurements menu items
#define STRING_UNITS "Unități"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptiv rapid"
#define STRING_ADAPTIVE_PRECISION "Adaptiv precizie"

// Average menu items
#define STRING_UNLIMITED "Nelimitat"
#define STRING_50_CONFIDENCE "±50% încredere"
#define STRING_20_CONFIDENCE "±20% încredere"
#define STRING_10_CONFIDENCE "±10% încredere"
#define STRING_5_CONFIDENCE "±5% încredere"
#define STRING_2_CONFIDENCE "±2% încredere"
#define STRING_1_CONFIDENCE "±1% încredere"

// Geiger tube menu items
#define STRING_SENSITIVITY "Sensibilitate"
#define STRING_DEAD_TIME_COMPENSATION "Compensare timp mort"
#define STRING_HVPROFILE "Profil HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (implicit)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Setare fabrică"
#define STRING_ACCURACY "Precizie"
#define STRING_ENERGY_SAVING "Economisire energie"
#define STRING_CUSTOM "Personalizat"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Frecvență PWM"
#define STRING_PWMDUTY_CYCLE "Ciclu de lucru PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Mod înregistrare"
#define STRING_RESET "Resetare"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Fiecare oră"
#define STRING_EVERY10_MINUTES "Fiecare 10 minute"
#define STRING_EVERY_MINUTE "Fiecare minut"
#define STRING_EVERY10_SECONDS "Fiecare 10 secunde"
#define STRING_EVERY_SECOND "Fiecare secundă"

// Display menu items
#define STRING_THEME "Temă"
#define STRING_BRIGHTNESS "Luminozitate"
#define STRING_CONTRAST "Contrast"
#define STRING_SLEEP "Repaus"

// Display - Theme menu items
#define STRING_THEME_DAY "Zi"
#define STRING_THEME_DUSK "Amurg"
#define STRING_THEME_NIGHT "Noapte"

// Display - Brightness menu items
#define STRING_VERY_LOW "Foarte scăzut"
#define STRING_LOW "Scăzut"
#define STRING_MEDIUM "Mediu"
#define STRING_HIGH "Ridicat"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nivel"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Întotdeauna oprit"
#define STRING_ALWAYS_ON "Întotdeauna pornit"

// Time and date menu items
#define STRING_TIME_ZONE "Fus orar"
#define STRING_YEAR "An"
#define STRING_MONTH "Lună"
#define STRING_DAY "Zi"
#define STRING_HOUR "Oră"
#define STRING_MINUTE "Minut"
#define STRING_TIME_FORMAT "Format oră"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 ore"
#define STRING_12_HOUR "12 ore"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alcalină"

// Random generator menu items
#define STRING_FULL_ASCII "ASCII complet"
#define STRING_ALPHANUMERIC "Alfanumeric"
#define STRING_HEXADECIMAL "Hexazecimal"
#define STRING_DECIMAL "Zecimal"
#define STRING_BINARY "Binar"
#define STRING_100_SIDED_DIE "Zar 100 fețe (0-99)"
#define STRING_20_SIDED_DIE "Zar 20 fețe"
#define STRING_12_SIDED_DIE "Zar 12 fețe"
#define STRING_10_SIDED_DIE "Zar 10 fețe (0-9)"
#define STRING_8_SIDED_DIE "Zar 8 fețe"
#define STRING_6_SIDED_DIE "Zar 6 fețe"
#define STRING_4_SIDED_DIE "Zar 4 fețe"
#define STRING_COIN_FLIP "Aruncare monedă"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Cap"
#define STRING_COIN_TAIL "Pajură"

// Game menu items
#define STRING_PLAY_WHITE "Joacă alb"
#define STRING_PLAY_BLACK "Joacă negru"
#define STRING_CONTINUE_GAME "Continuă joc"
#define STRING_NEW_GAME "Joc nou"
#define STRING_STRENGTH "Dificultate"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Nivel"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Durata tub"
#define STRING_TUBE_LIFE_PULSES "Pulsuri"
#define STRING_TUBE_DEAD_TIME "Timp mort"
#define STRING_DEVICE_ID "ID dispozitiv"
#define STRING_DEVICE_VOLTAGE "Voltaj"

#define STRING_TUBE "Tub"
#define STRING_LIFE_TIME "Durată"
#define STRING_LIFE_PULSES "Pulsuri"
#define STRING_DEAD_TIME "Timp mort"
#define STRING_DEVICE "Dispozitiv"
#define STRING_ID "ID"
#define STRING_VOLTAGE "Voltaj"
