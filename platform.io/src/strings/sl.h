/*
 * Rad Pro
 * UI strings: Slovenian
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

// Application name
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
#define STRING_24_HOURS "24 ur"
#define STRING_12_HOURS "12 ur"
#define STRING_6_HOURS "6 ur"
#define STRING_3_HOURS "3 ure"
#define STRING_1_HOUR "1 ura"
#define STRING_30_MINUTES "30 minut"
#define STRING_10_MINUTES "10 minut"
#define STRING_5_MINUTES "5 minut"
#define STRING_2_MINUTES "2 minuti"
#define STRING_1_MINUTE "1 minuta"
#define STRING_30_SECONDS "30 sekund"
#define STRING_10_SECONDS "10 sekund"
#define STRING_5_SECONDS "5 sekund"
#define STRING_1_SECOND "1 sekunda"

// Common menu items
#define STRING_OFF "Izklopljeno"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "OPOZORILO"
#define STRING_NOTIFICATION_SUCCESS "USPEH"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Napaka vsote firmware."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Napačne vrednosti škodijo."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Ponastavi dnevnik?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Vsi zapisi bodo izbrisani."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Dnevnik ponastavljen."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Trenutno"
#define STRING_AVERAGE "Povprečno"
#define STRING_CUMULATIVE "Kumulativno"
#define STRING_HISTORY10_MIN "Zgodovina (10 min)"
#define STRING_HISTORY1_H "Zgodovina (1 h)"
#define STRING_HISTORY24_H "Zgodovina (24 h)"

// Measurement view secondary view strings
#define STRING_TIME "Čas"
#define STRING_MAX "Max"
#define STRING_RATE "Hitrost"
#define STRING_DOSE "Doza"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ALARM"
#define STRING_INFO_FAULT "NAPAKA"
#define STRING_INFO_MAX "MAX"
#define STRING_INFO_OVER "PREK"
#define STRING_INFO_DONE "KONČ"

// Settings menu items
#define STRING_SETTINGS "Nastavitve"
#define STRING_PULSES "Impulzi"
#define STRING_ALARMS "Alarmi"
#define STRING_MEASUREMENTS "Meritve"
#define STRING_GEIGER_TUBE "Geigerjeva cev"
#define STRING_DATA_LOG "Dnevnik podatkov"
#define STRING_DISPLAY "Zaslon"
#define STRING_DATE_AND_TIME "Datum in čas"
#define STRING_BATTERY_TYPE "Tip baterije"
#define STRING_RANDOM_GENERATOR "Naključni generator"
#define STRING_GAME "Igra"
#define STRING_STATISTICS "Statistika"
#define STRING_DATA_MODE "Način podatkov"

// Pulses menu items
#define STRING_SOUND "Zvok"
#define STRING_VIBRATION "Vibracija"
#define STRING_ALERT_LED "Opozorilna LED"
#define STRING_PULSE_LED "Impulzna LED"
#define STRING_DISPLAY_FLASH "Utrip zaslona"
#define STRING_THRESHOLD "Prag"

// Sound menu items
#define STRING_CLICKS "Kliki"
#define STRING_CHIRPS "Cvrčanje"
#define STRING_BEEPS "Piski"

// Alarms menu items
#define STRING_RATE_ALARM "Alarm hitrosti"
#define STRING_DOSE_ALARM "Alarm doze"
#define STRING_OVERRANGE_ALARM "Alarm prekoračitve"
#define STRING_INDICATION "Indikacija"

// Measurements menu items
#define STRING_UNITS "Enote"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Prilagodljivo hitro"
#define STRING_ADAPTIVE_PRECISION "Prilagodljivo natančno"

// Average menu items
#define STRING_UNLIMITED "Neomejeno"
#define STRING_50_CONFIDENCE "±50% zaupanje"
#define STRING_20_CONFIDENCE "±20% zaupanje"
#define STRING_10_CONFIDENCE "±10% zaupanje"
#define STRING_5_CONFIDENCE "±5% zaupanje"
#define STRING_2_CONFIDENCE "±2% zaupanje"
#define STRING_1_CONFIDENCE "±1% zaupanje"

// Geiger tube menu items
#define STRING_SENSITIVITY "Občutljivost"
#define STRING_DEAD_TIME_SHORT_COMPENSATION "Kompenzacija mrtvega časa"
#define STRING_HVPROFILE "HV profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (privzeto)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Tovarniško privzeto"
#define STRING_ACCURACY "Natančnost"
#define STRING_ENERGY_SAVING "Varčevanje z energijo"
#define STRING_CUSTOM "Po meri"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Frekvenca PWM"
#define STRING_PWMDUTY_CYCLE "Delovni cikel PWM"

// Tube/battery voltage
#define STRING_VOLTAGE "Napetost"

// Data Log menu items
#define STRING_LOGGING_MODE "Način beleženja"
#define STRING_RESET "Ponastavi"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Vsako uro"
#define STRING_EVERY10_MINUTES "Vsakih 10 minut"
#define STRING_EVERY_MINUTE "Vsako minuto"
#define STRING_EVERY10_SECONDS "Vsakih 10 sekund"
#define STRING_EVERY_SECOND "Vsako sekundo"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Svetlost"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Spanje"

// Display - Theme menu items
#define STRING_THEME_DAY "Dan"
#define STRING_THEME_DUSK "Mrak"
#define STRING_THEME_NIGHT "Noč"

// Display - Brightness menu items
#define STRING_VERY_LOW "Zelo nizka"
#define STRING_LOW "Nizka"
#define STRING_MEDIUM "Srednja"
#define STRING_HIGH "Visoka"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Raven"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Vedno izklopljeno"
#define STRING_ALWAYS_ON "Vedno vklopljeno"

// Time and date menu items
#define STRING_TIME_ZONE "Časovni pas"
#define STRING_YEAR "Leto"
#define STRING_MONTH "Mesec"
#define STRING_DAY "Dan"
#define STRING_HOUR "Ura"
#define STRING_MINUTE "Minuta"
#define STRING_TIME_FORMAT "Format časa"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-urni"
#define STRING_12_HOUR "12-urni"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalna"

// Random generator menu items
#define STRING_FULL_ASCII "Polni ASCII"
#define STRING_ALPHANUMERIC "Alfanumerično"
#define STRING_HEXADECIMAL "Heksadecimalno"
#define STRING_DECIMAL "Decimalno"
#define STRING_BINARY "Binarno"
#define STRING_100_SIDED_DIE "100-stranska kocka (0-99)"
#define STRING_20_SIDED_DIE "20-stranska kocka"
#define STRING_12_SIDED_DIE "12-stranska kocka"
#define STRING_10_SIDED_DIE "10-stranska kocka (0-9)"
#define STRING_8_SIDED_DIE "8-stranska kocka"
#define STRING_6_SIDED_DIE "6-stranska kocka"
#define STRING_4_SIDED_DIE "4-stranska kocka"
#define STRING_COIN_FLIP "Met kovanca"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Glava"
#define STRING_COIN_TAIL "Rep"

// Game menu items
#define STRING_PLAY_WHITE "Igraj belo"
#define STRING_PLAY_BLACK "Igraj črno"
#define STRING_CONTINUE_GAME "Nadaljuj igro"
#define STRING_NEW_GAME "Nova igra"
#define STRING_STRENGTH "Moč"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Raven"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Življenjska doba"
#define STRING_DEAD_TIME "Mrtev čas"
#define STRING_DEVICE_ID "ID naprave"

#define STRING_TUBE "Cev"
#define STRING_LIFE_TIME "Doba"
#define STRING_DEAD_TIME_SHORT "Mrtev čas"
#define STRING_DEVICE "Naprava"
#define STRING_ID "ID"
#define STRING_VOLTAGE_SHORT "Napetost"
