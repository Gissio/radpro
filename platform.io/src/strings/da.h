/*
 * Rad Pro
 * UI strings: Danish
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
#define STRING_COUNT "tæl."
#define STRING_COUNTS "tæl."

// Common time menu items
#define STRING_24_HOURS "24 timer"
#define STRING_12_HOURS "12 timer"
#define STRING_6_HOURS "6 timer"
#define STRING_3_HOURS "3 timer"
#define STRING_1_HOUR "1 time"
#define STRING_30_MINUTES "30 minutter"
#define STRING_10_MINUTES "10 minutter"
#define STRING_5_MINUTES "5 minutter"
#define STRING_2_MINUTES "2 minutter"
#define STRING_1_MINUTE "1 minut"
#define STRING_30_SECONDS "30 sekunder"
#define STRING_10_SECONDS "10 sekunder"
#define STRING_5_SECONDS "5 sekunder"
#define STRING_1_SECOND "1 sekund"

// Common menu items
#define STRING_OFF "Fra"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "ADVARSEL"
#define STRING_NOTIFICATION_SUCCESS "SUCCES"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Firmware kontrolsum fejl."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Forkerte værdier skader enhed."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Nulstil datalog?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Alle poster slettes."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Datalog er nulstillet."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Øjeblikkelig"
#define STRING_AVERAGE "Gennemsnit"
#define STRING_CUMULATIVE "Kumulativ"
#define STRING_HISTORY10_MIN "Historik (10 min)"
#define STRING_HISTORY1_H "Historik (1 t)"
#define STRING_HISTORY24_H "Historik (24 t)"

// Measurement view secondary view strings
#define STRING_TIME "Tid"
#define STRING_MAX "Maks"
#define STRING_RATE "Rate"
#define STRING_DOSE "Dosis"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ALARM"
#define STRING_INFO_FAULT "FEJL"
#define STRING_INFO_MAX "MAKS"
#define STRING_INFO_OVER "OVER"
#define STRING_INFO_DONE "FÆRDIG"

// Settings menu items
#define STRING_SETTINGS "Indstillinger"
#define STRING_PULSES "Pulser"
#define STRING_ALARMS "Alarmer"
#define STRING_MEASUREMENTS "Målinger"
#define STRING_GEIGER_TUBE "Geiger-rør"
#define STRING_DATA_LOG "Datalog"
#define STRING_DISPLAY "Skærm"
#define STRING_DATE_AND_TIME "Dato og tid"
#define STRING_BATTERY_TYPE "Batteritype"
#define STRING_RANDOM_GENERATOR "Tilfældsgenerator"
#define STRING_GAME "Spil"
#define STRING_STATISTICS "Statistik"
#define STRING_DATA_MODE "Datatilstand"

// Pulses menu items
#define STRING_SOUND "Lyd"
#define STRING_VIBRATION "Vibration"
#define STRING_ALERT_LED "Alarm-LED"
#define STRING_PULSE_LED "Puls-LED"
#define STRING_DISPLAY_FLASH "Skærmblink"
#define STRING_THRESHOLD "Tærskel"

// Sound menu items
#define STRING_CLICKS "Klik"
#define STRING_CHIRPS "Pib"
#define STRING_BEEPS "Bip"

// Alarms menu items
#define STRING_RATE_ALARM "Ratealarm"
#define STRING_DOSE_ALARM "Dosisalarm"
#define STRING_OVERRANGE_ALARM "Overområde-alarm"
#define STRING_INDICATION "Indikation"

// Measurements menu items
#define STRING_UNITS "Enheder"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptiv hurtig"
#define STRING_ADAPTIVE_PRECISION "Adaptiv præcision"

// Average menu items
#define STRING_UNLIMITED "Ubegrænset"
#define STRING_50_CONFIDENCE "±50% tillid"
#define STRING_20_CONFIDENCE "±20% tillid"
#define STRING_10_CONFIDENCE "±10% tillid"
#define STRING_5_CONFIDENCE "±5% tillid"
#define STRING_2_CONFIDENCE "±2% tillid"
#define STRING_1_CONFIDENCE "±1% tillid"

// Geiger tube menu items
#define STRING_SENSITIVITY "Følsomhed"
#define STRING_DEAD_TIME_COMPENSATION "Dødtidskompensation"
#define STRING_HVPROFILE "HV-profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (standard)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Fabriksstandard"
#define STRING_ACCURACY "Nøjagtighed"
#define STRING_ENERGY_SAVING "Energibesparende"
#define STRING_CUSTOM "Brugerdefineret"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM-frekvens"
#define STRING_PWMDUTY_CYCLE "PWM-arbejdsforhold"

// Data Log menu items
#define STRING_LOGGING_MODE "Logtilstand"
#define STRING_RESET "Nulstil"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Hver time"
#define STRING_EVERY10_MINUTES "Hver 10 minutter"
#define STRING_EVERY_MINUTE "Hvert minut"
#define STRING_EVERY10_SECONDS "Hvert 10 sekunder"
#define STRING_EVERY_SECOND "Hvert sekund"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Lysstyrke"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Søvn"

// Display - Theme menu items
#define STRING_THEME_DAY "Dag"
#define STRING_THEME_DUSK "Skumring"
#define STRING_THEME_NIGHT "Nat"

// Display - Brightness menu items
#define STRING_VERY_LOW "Meget lav"
#define STRING_LOW "Lav"
#define STRING_MEDIUM "Mellem"
#define STRING_HIGH "Høj"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Niveau"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Altid fra"
#define STRING_ALWAYS_ON "Altid tændt"

// Time and date menu items
#define STRING_TIME_ZONE "Tidszone"
#define STRING_YEAR "År"
#define STRING_MONTH "Måned"
#define STRING_DAY "Dag"
#define STRING_HOUR "Time"
#define STRING_MINUTE "Minut"
#define STRING_TIME_FORMAT "Tidsformat"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-timers"
#define STRING_12_HOUR "12-timers"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkaline"

// Random generator menu items
#define STRING_FULL_ASCII "Fuld ASCII"
#define STRING_ALPHANUMERIC "Alfanumerisk"
#define STRING_HEXADECIMAL "Hexadecimal"
#define STRING_DECIMAL "Decimal"
#define STRING_BINARY "Binær"
#define STRING_100_SIDED_DIE "100-sidet terning (0-99)"
#define STRING_20_SIDED_DIE "20-sidet terning"
#define STRING_12_SIDED_DIE "12-sidet terning"
#define STRING_10_SIDED_DIE "10-sidet terning (0-9)"
#define STRING_8_SIDED_DIE "8-sidet terning"
#define STRING_6_SIDED_DIE "6-sidet terning"
#define STRING_4_SIDED_DIE "4-sidet terning"
#define STRING_COIN_FLIP "Møntkast"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Krone"
#define STRING_COIN_TAIL "Plat"

// Game menu items
#define STRING_PLAY_WHITE "Spil hvid"
#define STRING_PLAY_BLACK "Spil sort"
#define STRING_CONTINUE_GAME "Fortsæt spil"
#define STRING_NEW_GAME "Nyt spil"
#define STRING_STRENGTH "Styrke"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Niveau"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Rørlevetid"
#define STRING_TUBE_LIFE_PULSES "Impulser"
#define STRING_TUBE_DEAD_TIME "Dødtid"
#define STRING_DEVICE_ID "Enheds-ID"
#define STRING_DEVICE_VOLTAGE "Spænding"

#define STRING_TUBE "Rør"
#define STRING_LIFE_TIME "Levetid"
#define STRING_LIFE_PULSES "Pulser"
#define STRING_DEAD_TIME "Dødtid"
#define STRING_DEVICE "Enhed"
#define STRING_ID "ID"
#define STRING_VOLTAGE "Spænding"
