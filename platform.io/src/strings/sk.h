/*
 * Rad Pro
 * UI strings: Slovakian
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

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "imp."
#define STRING_COUNTS "imp."

// Common time menu items
#define STRING_24_HOURS "24 hodín"
#define STRING_12_HOURS "12 hodín"
#define STRING_6_HOURS "6 hodín"
#define STRING_3_HOURS "3 hodiny"
#define STRING_1_HOUR "1 hodina"
#define STRING_30_MINUTES "30 minút"
#define STRING_10_MINUTES "10 minút"
#define STRING_5_MINUTES "5 minút"
#define STRING_2_MINUTES "2 minúty"
#define STRING_1_MINUTE "1 minúta"
#define STRING_30_SECONDS "30 sekúnd"
#define STRING_10_SECONDS "10 sekúnd"
#define STRING_5_SECONDS "5 sekúnd"
#define STRING_1_SECOND "1 sekunda"

// Common menu items
#define STRING_OFF "Vypnuté"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Veľmi nízky"
#define STRING_LOW "Nízky"
#define STRING_MEDIUM "Stredný"
#define STRING_HIGH "Vysoký"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "UPOZORNENIE\n\nZlyhanie kontrolného súčtu firmvéru."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "UPOZORNENIE\n\nNesprávne hodnoty môžu poškodiť zariadenie."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Vynulovať dátový záznam?\n\nVšetky záznamy budú vymazané."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "ÚSPECH\n\nDátový záznam bol vynulovaný."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Okamžité"
#define STRING_AVERAGE "Priemer"
#define STRING_CUMULATIVE "Kumulatívne"
#define STRING_HISTORY "História"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 t"
#define STRING_HISTORY_1_MONTH "1 m"
#define STRING_HISTORY_1_YEAR "1 r"

// Measurement view secondary view strings
#define STRING_TIME "Čas"
#define STRING_MAX "Max"
#define STRING_RATE "Rýchlosť"
#define STRING_DOSE "Dávka"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_ALERT_ALARM "ALARM"
#define STRING_ALERT_WARNING "UPOZORNENIE"
#define STRING_ALERT_FAULT "CHYBA"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_OVERFLOW "NAD"
#define STRING_ALERT_DONE "HOTOVO"

// Settings menu items
#define STRING_SETTINGS "Nastavenia"
#define STRING_PULSES "Impulzy"
#define STRING_ALERTS "Upozornenia"
#define STRING_MEASUREMENTS "Merania"
#define STRING_GEIGER_TUBE "Geigerova trubica"
#define STRING_DATALOG "Dátový záznam"
#define STRING_DISPLAY "Displej"
#define STRING_DATE_AND_TIME "Dátum a čas"
#define STRING_BATTERY_TYPE "Typ batérie"
#define STRING_RANDOM_GENERATOR "Generátor náhodných čísel"
#define STRING_GAME "Hra"
#define STRING_STATISTICS "Štatistiky"
#define STRING_DATAMODE "Dátový režim"

// Pulses menu items
#define STRING_SOUND "Zvuk"
#define STRING_VOICE "Hlas"
#define STRING_VIBRATION "Vibrácie"
#define STRING_PULSE_LED "LED impulzu"
#define STRING_DISPLAY_FLASH "Blikanie displeja"
#define STRING_THRESHOLD "Prah"

// Alarms menu items
#define STRING_RATE_WARNING "Varovanie rýchlosti"
#define STRING_RATE_ALARM "Alarm rýchlosti"
#define STRING_DOSE_WARNING "Varovanie dávky"
#define STRING_DOSE_ALARM "Alarm dávky"
#define STRING_INDICATION "Indikácia"

// Measurements menu items
#define STRING_UNITS "Jednotky"
#define STRING_SECONDARY_UNITS "Sekundárne jednotky"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptívne rýchle"
#define STRING_ADAPTIVE_PRECISION "Adaptívne presné"

// Average menu items
#define STRING_UNLIMITED "Neobmedzené"
#define STRING_50_CONFIDENCE "±50% spoľahlivosť"
#define STRING_20_CONFIDENCE "±20% spoľahlivosť"
#define STRING_10_CONFIDENCE "±10% spoľahlivosť"
#define STRING_5_CONFIDENCE "±5% spoľahlivosť"
#define STRING_2_CONFIDENCE "±2% spoľahlivosť"
#define STRING_1_CONFIDENCE "±1% spoľahlivosť"

// Geiger tube menu items
#define STRING_SENSITIVITY "Citlivosť"
#define STRING_DEAD_TIME_COMPENSATION "Kompenzácia mŕtveho času"
#define STRING_HVPROFILE "HV profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (predvolené)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Továrenské predvolené"
#define STRING_ACCURACY "Presnosť"
#define STRING_ENERGY_SAVING "Úspora energie"
#define STRING_CUSTOM "Vlastné"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Frekvencia PWM"
#define STRING_PWMDUTY_CYCLE "Pracovný cyklus PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Režim záznamu"
#define STRING_RESET "Resetovať"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Každú hodinu"
#define STRING_EVERY10_MINUTES "Každých 10 minút"
#define STRING_EVERY_MINUTE "Každú minútu"
#define STRING_EVERY10_SECONDS "Každých 10 sekúnd"
#define STRING_EVERY_SECOND "Každú sekundu"

// Display menu items
#define STRING_THEME "Téma"
#define STRING_BRIGHTNESS "Jas"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Spánok"

// Display - Theme menu items
#define STRING_THEME_DAY "Deň"
#define STRING_THEME_DUSK "Súmrak"
#define STRING_THEME_NIGHT "Noc"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Úroveň"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Vždy vypnuté"
#define STRING_ALWAYS_ON "Vždy zapnuté"

// Sound menu items
#define STRING_ALERTSTYLE "Štýl upozornenia"
#define STRING_ALERTVOLUME "Hlasitosť upozornenia"
#define STRING_VOICEVOLUME "Hlasitosť hlasu"

// Sound Alarms length
#define STRING_SHORT "Krátky"
#define STRING_LONG "Dlhý"

// Sound pulses menu items
#define STRING_CLICKS "Kliknutia"
#define STRING_CHIRPS "Cvrčanie"
#define STRING_BEEPS "Pípanie"

// Time and date menu items
#define STRING_TIME_ZONE "Časové pásmo"
#define STRING_YEAR "Rok"
#define STRING_MONTH "Mesiac"
#define STRING_DAY "Deň"
#define STRING_HOUR "Hodina"
#define STRING_MINUTE "Minúta"
#define STRING_TIME_FORMAT "Formát času"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-hodinový"
#define STRING_12_HOUR "12-hodinový"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalická"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerické"
#define STRING_HEXADECIMAL "Hexadecimálne"
#define STRING_DECIMAL "Desiatkové"
#define STRING_BINARY "Binárne"
#define STRING_100_SIDED_DIE "100-stenná kocka (0-99)"
#define STRING_20_SIDED_DIE "20-stenná kocka"
#define STRING_12_SIDED_DIE "12-stenná kocka"
#define STRING_10_SIDED_DIE "10-stenná kocka (0-9)"
#define STRING_8_SIDED_DIE "8-stenná kocka"
#define STRING_6_SIDED_DIE "6-stenná kocka"
#define STRING_4_SIDED_DIE "4-stenná kocka"
#define STRING_COIN_FLIP "Hod mincou (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Hrať bielymi"
#define STRING_PLAY_BLACK "Hrať čiernymi"
#define STRING_CONTINUE_GAME "Pokračovať v hre"
#define STRING_NEW_GAME "Nová hra"
#define STRING_STRENGTH "Sila"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Úroveň"

// Statistics view items
#define STRING_TUBE_LIFE "Životnosť trubice"
#define STRING_TUBE "Trubica"
#define STRING_LIFE "Životnosť"
#define STRING_DEAD_TIME "Mŕtvy čas"
#define STRING_DEVICE_ID "ID zariadenia"
#define STRING_DEVICE "Zariadenie"
#define STRING_ID "ID"
#define STRING_BATTERY "Batéria"
