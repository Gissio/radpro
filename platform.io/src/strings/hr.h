/*
 * Rad Pro
 * UI strings: Croatian
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
#define STRING_COUNT "imp."
#define STRING_COUNTS "imp."

// Common time menu items
#define STRING_24_HOURS "24 sata"
#define STRING_12_HOURS "12 sati"
#define STRING_6_HOURS "6 sati"
#define STRING_3_HOURS "3 sata"
#define STRING_1_HOUR "1 sat"
#define STRING_30_MINUTES "30 minuta"
#define STRING_10_MINUTES "10 minuta"
#define STRING_5_MINUTES "5 minuta"
#define STRING_2_MINUTES "2 minute"
#define STRING_1_MINUTE "1 minuta"
#define STRING_30_SECONDS "30 sekundi"
#define STRING_10_SECONDS "10 sekundi"
#define STRING_5_SECONDS "5 sekundi"
#define STRING_1_SECOND "1 sekunda"

// Common menu items
#define STRING_OFF "Isključeno"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "UPOZORENJE"
#define STRING_NOTIFICATION_SUCCESS "USPJEH"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Neispravna suma firmvera."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Kriva vrijednost šteti uređaju."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Poništiti log?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Svi zapisi će biti izbrisani."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Zapis podataka poništen."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Trenutno"
#define STRING_AVERAGE "Prosjek"
#define STRING_CUMULATIVE "Kumulativno"
#define STRING_HISTORY10_MIN "Povijest (10 min)"
#define STRING_HISTORY1_H "Povijest (1 h)"
#define STRING_HISTORY24_H "Povijest (24 h)"

// Measurement view secondary view strings
#define STRING_TIME "Vrijeme"
#define STRING_MAX "Maks"
#define STRING_RATE "Stopa"
#define STRING_DOSE "Doza"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ALARM"
#define STRING_INFO_FAULT "GREŠKA"
#define STRING_INFO_MAX "MAKS"
#define STRING_INFO_OVER "PREKO"
#define STRING_INFO_DONE "GOTOVO"

// Settings menu items
#define STRING_SETTINGS "Postavke"
#define STRING_PULSES "Impulsi"
#define STRING_ALARMS "Alarmi"
#define STRING_MEASUREMENTS "Mjerenja"
#define STRING_GEIGER_TUBE "Geigerova cijev"
#define STRING_DATA_LOG "Zapis podataka"
#define STRING_DISPLAY "Zaslon"
#define STRING_DATE_AND_TIME "Datum i vrijeme"
#define STRING_BATTERY_TYPE "Tip baterije"
#define STRING_RANDOM_GENERATOR "Slučajni generator"
#define STRING_GAME "Igra"
#define STRING_STATISTICS "Statistika"
#define STRING_DATA_MODE "Način podataka"

// Pulses menu items
#define STRING_SOUND "Zvuk"
#define STRING_VIBRATION "Vibracija"
#define STRING_ALERT_LED "LED upozorenja"
#define STRING_PULSE_LED "LED impulsa"
#define STRING_DISPLAY_FLASH "Bljeskalica zaslona"
#define STRING_THRESHOLD "Prag"

// Sound menu items
#define STRING_CLICKS "Klikovi"
#define STRING_CHIRPS "Cvrkuti"
#define STRING_BEEPS "Bipovi"

// Alarms menu items
#define STRING_RATE_ALARM "Alarm stope"
#define STRING_DOSE_ALARM "Alarm doze"
#define STRING_OVERRANGE_ALARM "Alarm prekoračenja"
#define STRING_INDICATION "Indikacija"

// Measurements menu items
#define STRING_UNITS "Jedinice"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Prilagodljivo brzo"
#define STRING_ADAPTIVE_PRECISION "Prilagodljiva preciznost"

// Average menu items
#define STRING_UNLIMITED "Neograničeno"
#define STRING_50_CONFIDENCE "±50% pouzdanost"
#define STRING_20_CONFIDENCE "±20% pouzdanost"
#define STRING_10_CONFIDENCE "±10% pouzdanost"
#define STRING_5_CONFIDENCE "±5% pouzdanost"
#define STRING_2_CONFIDENCE "±2% pouzdanost"
#define STRING_1_CONFIDENCE "±1% pouzdanost"

// Geiger tube menu items
#define STRING_SENSITIVITY "Osjetljivost"
#define STRING_DEAD_TIME_COMPENSATION "Mrtvo vrijeme kompenzacija"
#define STRING_HVPROFILE "HV profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (zadano)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Tvornički zadano"
#define STRING_ACCURACY "Točnost"
#define STRING_ENERGY_SAVING "Ušteda energije"
#define STRING_CUSTOM "Prilagođeno"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM frekvencija"
#define STRING_PWMDUTY_CYCLE "PWM radni ciklus"

// Data Log menu items
#define STRING_LOGGING_MODE "Način zapisivanja"
#define STRING_RESET "Poništi"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Svaki sat"
#define STRING_EVERY10_MINUTES "Svakih 10 minuta"
#define STRING_EVERY_MINUTE "Svaku minutu"
#define STRING_EVERY10_SECONDS "Svakih 10 sekundi"
#define STRING_EVERY_SECOND "Svaku sekundu"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Svjetlina"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Spavanje"

// Display - Theme menu items
#define STRING_THEME_DAY "Dan"
#define STRING_THEME_DUSK "Sumrak"
#define STRING_THEME_NIGHT "Noć"

// Display - Brightness menu items
#define STRING_VERY_LOW "Vrlo nisko"
#define STRING_LOW "Nisko"
#define STRING_MEDIUM "Srednje"
#define STRING_HIGH "Visoko"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Razina"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Uvijek isključeno"
#define STRING_ALWAYS_ON "Uvijek uključeno"

// Time and date menu items
#define STRING_TIME_ZONE "Vremenska zona"
#define STRING_YEAR "Godina"
#define STRING_MONTH "Mjesec"
#define STRING_DAY "Dan"
#define STRING_HOUR "Sat"
#define STRING_MINUTE "Minuta"
#define STRING_TIME_FORMAT "Format vremena"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-satni"
#define STRING_12_HOUR "12-satni"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalna"

// Random generator menu items
#define STRING_FULL_ASCII "Puni ASCII"
#define STRING_ALPHANUMERIC "Alfanumerički"
#define STRING_HEXADECIMAL "Heksadekadski"
#define STRING_DECIMAL "Decimalni"
#define STRING_BINARY "Binarno"
#define STRING_100_SIDED_DIE "100-strana kocka (0-99)"
#define STRING_20_SIDED_DIE "20-strana kocka"
#define STRING_12_SIDED_DIE "12-strana kocka"
#define STRING_10_SIDED_DIE "10-strana kocka (0-9)"
#define STRING_8_SIDED_DIE "8-strana kocka"
#define STRING_6_SIDED_DIE "6-strana kocka"
#define STRING_4_SIDED_DIE "4-strana kocka"
#define STRING_COIN_FLIP "Bacanje novčića"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Glava"
#define STRING_COIN_TAIL "Pismo"

// Game menu items
#define STRING_PLAY_WHITE "Igraj bijelo"
#define STRING_PLAY_BLACK "Igraj crno"
#define STRING_CONTINUE_GAME "Nastavi igru"
#define STRING_NEW_GAME "Nova igra"
#define STRING_STRENGTH "Jačina"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Razina"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Vijek cijevi"
#define STRING_TUBE_LIFE_PULSES "Impulsi"
#define STRING_TUBE_DEAD_TIME "Mrtvo vrijeme"
#define STRING_DEVICE_ID "ID uređaja"
#define STRING_DEVICE_VOLTAGE "Napon"

#define STRING_TUBE "Cijev"
#define STRING_LIFE_TIME "Trajanja"
#define STRING_LIFE_PULSES "Impulsi"
#define STRING_DEAD_TIME "Mrtvo vr."
#define STRING_DEVICE "Uređaj"
#define STRING_ID "ID"
#define STRING_VOLTAGE "Napon"
