/*
 * Rad Pro
 * UI strings: Lithuanian
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
#define STRING_24_HOURS "24 valandos"
#define STRING_12_HOURS "12 valandų"
#define STRING_6_HOURS "6 valandos"
#define STRING_3_HOURS "3 valandos"
#define STRING_1_HOUR "1 valanda"
#define STRING_30_MINUTES "30 minučių"
#define STRING_10_MINUTES "10 minučių"
#define STRING_5_MINUTES "5 minutės"
#define STRING_2_MINUTES "2 minutės"
#define STRING_1_MINUTE "1 minutė"
#define STRING_30_SECONDS "30 sekundžių"
#define STRING_10_SECONDS "10 sekundžių"
#define STRING_5_SECONDS "5 sekundės"
#define STRING_1_SECOND "1 sekundė"

// Common menu items
#define STRING_OFF "Išjungta"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Labai žemas"
#define STRING_LOW "Žemas"
#define STRING_MEDIUM "Vidutinis"
#define STRING_HIGH "Aukštas"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ĮSPĖJIMAS\n\nPrograminės įrangos kontrolinės sumos klaida."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "ĮSPĖJIMAS\n\nNeteisingos reikšmės gali pakenkti įrenginiui."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Atstatyti duomenų žurnalą?\n\nVisi įrašai bus ištrinti."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SĖKMĖ\n\nDuomenų žurnalas atstatytas."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Akimirkinis"
#define STRING_AVERAGE "Vidutinis"
#define STRING_CUMULATIVE "Kaupiamasis"
#define STRING_HISTORY "Istorija"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 val"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 sav"
#define STRING_HISTORY_1_MONTH "1 mėn"
#define STRING_HISTORY_1_YEAR "1 m"

// Measurement view secondary view strings
#define STRING_TIME "Laikas"
#define STRING_MAX "Maks"
#define STRING_RATE "Greitis"
#define STRING_DOSE "Dozė"

// Measurement view info strings
#define STRING_ALERT_ALARM "ALARMAS"
#define STRING_ALERT_WARNING "ĮSPĖJIMAS"
#define STRING_ALERT_FAULT "GEDIMAS"
#define STRING_ALERT_MAX "MAKS"
#define STRING_ALERT_DONE "BAIGTA"

// Settings menu items
#define STRING_SETTINGS "Nustatymai"
#define STRING_PULSES "Impulsai"
#define STRING_ALERTS "Įspėjimai"
#define STRING_MEASUREMENTS "Matavimai"
#define STRING_GEIGER_TUBE "Geigerio vamzdis"
#define STRING_DATALOG "Duomenų žurnalas"
#define STRING_DISPLAY "Ekranas"
#define STRING_DATE_AND_TIME "Data ir laikas"
#define STRING_BATTERY_TYPE "Baterijos tipas"
#define STRING_RANDOM_GENERATOR "Atsitiktinis generatorius"
#define STRING_GAME "Žaidimas"
#define STRING_STATISTICS "Statistika"
#define STRING_DATAMODE "Duomenų režimas"

// Pulses menu items
#define STRING_SOUND "Garsas"
#define STRING_VOICE "Balsas"
#define STRING_VIBRATION "Vibracija"
#define STRING_PULSE_LED "Impulso LED"
#define STRING_DISPLAY_FLASH "Ekrano blykstė"
#define STRING_THRESHOLD "Slenkstis"

// Alarms menu items
#define STRING_RATE_WARNING "Greičio įspėjimas"
#define STRING_RATE_ALARM "Greičio alarmas"
#define STRING_DOSE_WARNING "Dozės įspėjimas"
#define STRING_DOSE_ALARM "Dozės alarmas"
#define STRING_INDICATION "Indikacija"

// Measurements menu items
#define STRING_UNITS "Vienetai"
#define STRING_SECONDARY_UNITS "Antriniai vienetai"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptyvus greitas"
#define STRING_ADAPTIVE_PRECISION "Adaptyvus tikslus"

// Average menu items
#define STRING_UNLIMITED "Neribotas"
#define STRING_50_CONFIDENCE "±50% pasitikėjimas"
#define STRING_20_CONFIDENCE "±20% pasitikėjimas"
#define STRING_10_CONFIDENCE "±10% pasitikėjimas"
#define STRING_5_CONFIDENCE "±5% pasitikėjimas"
#define STRING_2_CONFIDENCE "±2% pasitikėjimas"
#define STRING_1_CONFIDENCE "±1% pasitikėjimas"

// Geiger tube menu items
#define STRING_SENSITIVITY "Jautrumas"
#define STRING_DEAD_TIME_COMPENSATION "Mirusio laiko kompensacija"
#define STRING_HVPROFILE "HV profilis"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (numatytasis)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Gamyklos numatytasis"
#define STRING_ACCURACY "Tikslumas"
#define STRING_ENERGY_SAVING "Energijos taupymas"
#define STRING_CUSTOM "Pasirinktinis"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM dažnis"
#define STRING_PWMDUTY_CYCLE "PWM darbo ciklas"

// Data Log menu items
#define STRING_LOGGING_MODE "Žurnalavimo režimas"
#define STRING_RESET "Atstatyti"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Kas valandą"
#define STRING_EVERY10_MINUTES "Kas 10 minučių"
#define STRING_EVERY_MINUTE "Kas minutę"
#define STRING_EVERY10_SECONDS "Kas 10 sekundžių"
#define STRING_EVERY_SECOND "Kas sekundę"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Ryškumas"
#define STRING_CONTRAST "Kontrastas"
#define STRING_SLEEP "Miegas"

// Display - Theme menu items
#define STRING_THEME_DAY "Diena"
#define STRING_THEME_DUSK "Sutemos"
#define STRING_THEME_NIGHT "Naktis"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Lygis"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Visada išjungtas"
#define STRING_ALWAYS_ON "Visada įjungtas"

// Sound menu items
#define STRING_ALERTSTYLE "Įspėjimo stilius"
#define STRING_ALERTVOLUME "Įspėjimo garsumas"
#define STRING_VOICEVOLUME "Balso garsumas"

// Sound Alarms length
#define STRING_SHORT "Trumpas"
#define STRING_LONG "Ilgas"

// Sound pulses menu items
#define STRING_CLICKS "Spragtelėjimai"
#define STRING_CHIRPS "Čirpimai"
#define STRING_BEEPS "Pypsėjimai"

// Time and date menu items
#define STRING_TIME_ZONE "Laiko juosta"
#define STRING_YEAR "Metai"
#define STRING_MONTH "Mėnuo"
#define STRING_DAY "Diena"
#define STRING_HOUR "Valanda"
#define STRING_MINUTE "Minutė"
#define STRING_TIME_FORMAT "Laiko formatas"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 valandų"
#define STRING_12_HOUR "12 valandų"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Šarminė"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Raidiniai-skaitmeniniai"
#define STRING_HEXADECIMAL "Šešioliktainis"
#define STRING_DECIMAL "Dešimtainis"
#define STRING_BINARY "Dvejetainis"
#define STRING_100_SIDED_DIE "100 pusių kauliukas (0-99)"
#define STRING_20_SIDED_DIE "20 pusių kauliukas"
#define STRING_12_SIDED_DIE "12 pusių kauliukas"
#define STRING_10_SIDED_DIE "10 pusių kauliukas (0-9)"
#define STRING_8_SIDED_DIE "8 pusių kauliukas"
#define STRING_6_SIDED_DIE "6 pusių kauliukas"
#define STRING_4_SIDED_DIE "4 pusių kauliukas"
#define STRING_COIN_FLIP "Monetos metimas (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Žaisti baltu"
#define STRING_PLAY_BLACK "Žaisti juodu"
#define STRING_CONTINUE_GAME "Tęsti žaidimą"
#define STRING_NEW_GAME "Naujas žaidimas"
#define STRING_STRENGTH "Stiprumas"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Lygis"

// Statistics view items
#define STRING_TUBE_LIFE "Vamzdžio gyvavimo laikas"
#define STRING_TUBE "Vamzdis"
#define STRING_LIFE "Gyvavimas"
#define STRING_DEAD_TIME "Mirusis laikas"
#define STRING_DEVICE_ID "Įrenginio ID"
#define STRING_DEVICE "Įrenginys"
#define STRING_ID "ID"
#define STRING_BATTERY "Baterija"
