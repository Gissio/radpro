/*
 * Rad Pro
 * UI strings: Swedish
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
#define STRING_COUNT "puls"
#define STRING_COUNTS "pulser"

// Common time menu items
#define STRING_24_HOURS "24 timmar"
#define STRING_12_HOURS "12 timmar"
#define STRING_6_HOURS "6 timmar"
#define STRING_3_HOURS "3 timmar"
#define STRING_1_HOUR "1 timme"
#define STRING_30_MINUTES "30 minuter"
#define STRING_10_MINUTES "10 minuter"
#define STRING_5_MINUTES "5 minuter"
#define STRING_2_MINUTES "2 minuter"
#define STRING_1_MINUTE "1 minut"
#define STRING_30_SECONDS "30 sekunder"
#define STRING_10_SECONDS "10 sekunder"
#define STRING_5_SECONDS "5 sekunder"
#define STRING_1_SECOND "1 sekund"

// Common menu items
#define STRING_OFF "Av"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Mycket låg"
#define STRING_LOW "Låg"
#define STRING_MEDIUM "Medel"
#define STRING_HIGH "Hög"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "VARNING\n\nFirmware kontrollsumma fel."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "VARNING\n\nFelaktiga värden kan skada enheten."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Återställ datalogg?\n\nAlla poster kommer att raderas."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "LYCKADES\n\nDataloggen har återställts."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Omedelbar"
#define STRING_AVERAGE "Genomsnitt"
#define STRING_CUMULATIVE "Kumulativ"
#define STRING_HISTORY "Historik"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 t"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 v"
#define STRING_HISTORY_1_MONTH "1 m"
#define STRING_HISTORY_1_YEAR "1 å"

// Measurement view secondary view strings
#define STRING_TIME "Tid"
#define STRING_MAX "Max"
#define STRING_RATE "Hastighet"
#define STRING_DOSE "Dos"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_ALERT_ALARM "LARM"
#define STRING_ALERT_WARNING "VARNING"
#define STRING_ALERT_FAULT "FEL"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_OVERFLOW "ÖVER"
#define STRING_ALERT_DONE "KLAR"

// Settings menu items
#define STRING_SETTINGS "Inställningar"
#define STRING_PULSES "Pulser"
#define STRING_ALERTS "Varningar"
#define STRING_MEASUREMENTS "Mätningar"
#define STRING_GEIGER_TUBE "Geiger-rör"
#define STRING_DATALOG "Datalogg"
#define STRING_DISPLAY "Skärm"
#define STRING_DATE_AND_TIME "Datum och tid"
#define STRING_BATTERY_TYPE "Batterityp"
#define STRING_RANDOM_GENERATOR "Slumpgenerator"
#define STRING_GAME "Spel"
#define STRING_STATISTICS "Statistik"
#define STRING_DATAMODE "Dateläge"

// Pulses menu items
#define STRING_SOUND "Ljud"
#define STRING_VOICE "Röst"
#define STRING_VIBRATION "Vibration"
#define STRING_PULSE_LED "Puls-LED"
#define STRING_DISPLAY_FLASH "Skärmblink"
#define STRING_THRESHOLD "Tröskel"

// Alarms menu items
#define STRING_RATE_WARNING "Hastighetsvarning"
#define STRING_RATE_ALARM "Hastighetslarm"
#define STRING_DOSE_WARNING "Dosvarning"
#define STRING_DOSE_ALARM "Doslarm"
#define STRING_INDICATION "Indikation"

// Measurements menu items
#define STRING_UNITS "Enheter"
#define STRING_SECONDARY_UNITS "Sekundära enheter"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptiv snabb"
#define STRING_ADAPTIVE_PRECISION "Adaptiv precision"

// Average menu items
#define STRING_UNLIMITED "Obegränsad"
#define STRING_50_CONFIDENCE "±50% konfidens"
#define STRING_20_CONFIDENCE "±20% konfidens"
#define STRING_10_CONFIDENCE "±10% konfidens"
#define STRING_5_CONFIDENCE "±5% konfidens"
#define STRING_2_CONFIDENCE "±2% konfidens"
#define STRING_1_CONFIDENCE "±1% konfidens"

// Geiger tube menu items
#define STRING_SENSITIVITY "Känslighet"
#define STRING_DEAD_TIME_COMPENSATION "Dödtidskompensation"
#define STRING_HVPROFILE "HV-profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (standard)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Fabriksstandard"
#define STRING_ACCURACY "Noggrannhet"
#define STRING_ENERGY_SAVING "Energisparande"
#define STRING_CUSTOM "Anpassad"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM-frekvens"
#define STRING_PWMDUTY_CYCLE "PWM-arbetscykel"

// Data Log menu items
#define STRING_LOGGING_MODE "Loggningsläge"
#define STRING_RESET "Återställ"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Varje timme"
#define STRING_EVERY10_MINUTES "Var 10:e minut"
#define STRING_EVERY_MINUTE "Varje minut"
#define STRING_EVERY10_SECONDS "Var 10:e sekund"
#define STRING_EVERY_SECOND "Varje sekund"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Ljusstyrka"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Vila"

// Display - Theme menu items
#define STRING_THEME_DAY "Dag"
#define STRING_THEME_DUSK "Skymning"
#define STRING_THEME_NIGHT "Natt"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nivå"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Alltid av"
#define STRING_ALWAYS_ON "Alltid på"

// Sound menu items
#define STRING_ALERTSTYLE "Varningsstil"
#define STRING_ALERTVOLUME "Varningsvolym"
#define STRING_VOICEVOLUME "Röstvolym"

// Sound Alarms length
#define STRING_SHORT "Kort"
#define STRING_LONG "Lång"

// Sound pulses menu items
#define STRING_CLICKS "Klick"
#define STRING_CHIRPS "Pip"
#define STRING_BEEPS "Signal"

// Time and date menu items
#define STRING_TIME_ZONE "Tidszon"
#define STRING_YEAR "År"
#define STRING_MONTH "Månad"
#define STRING_DAY "Dag"
#define STRING_HOUR "Timme"
#define STRING_MINUTE "Minut"
#define STRING_TIME_FORMAT "Tidsformat"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "FM"
#define STRING_PM "EM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-timmars"
#define STRING_12_HOUR "12-timmars"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalisk"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerisk"
#define STRING_HEXADECIMAL "Hexadecimal"
#define STRING_DECIMAL "Decimal"
#define STRING_BINARY "Binär"
#define STRING_100_SIDED_DIE "100-sidig tärning (0-99)"
#define STRING_20_SIDED_DIE "20-sidig tärning"
#define STRING_12_SIDED_DIE "12-sidig tärning"
#define STRING_10_SIDED_DIE "10-sidig tärning (0-9)"
#define STRING_8_SIDED_DIE "8-sidig tärning"
#define STRING_6_SIDED_DIE "6-sidig tärning"
#define STRING_4_SIDED_DIE "4-sidig tärning"
#define STRING_COIN_FLIP "Myntkast (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Spela vit"
#define STRING_PLAY_BLACK "Spela svart"
#define STRING_CONTINUE_GAME "Fortsätt spel"
#define STRING_NEW_GAME "Nytt spel"
#define STRING_STRENGTH "Styrka"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Nivå"

// Statistics view items
#define STRING_TUBE_LIFE "Rörlivslängd"
#define STRING_TUBE "Rör"
#define STRING_LIFE "Livslängd"
#define STRING_DEAD_TIME "Dödtid"
#define STRING_DEVICE_ID "Enhets-ID"
#define STRING_DEVICE "Enhet"
#define STRING_ID "ID"
#define STRING_BATTERY "Batteri"
