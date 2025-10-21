/*
 * Rad Pro
 * UI strings: Norwegian
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
#define STRING_COUNT "tell"
#define STRING_COUNTS "tell."

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
#define STRING_1_MINUTE "1 minutt"
#define STRING_30_SECONDS "30 sekunder"
#define STRING_10_SECONDS "10 sekunder"
#define STRING_5_SECONDS "5 sekunder"
#define STRING_1_SECOND "1 sekund"

// Common menu items
#define STRING_OFF "Av"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Veldig lav"
#define STRING_LOW "Lav"
#define STRING_MEDIUM "Middels"
#define STRING_HIGH "Høy"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING ""
#define STRING_NOTIFICATION_SUCCESS "SUKSESS"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ADVARSEL\n\nFirmware kontrollsumfeil."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "ADVARSEL\n\nFeil verdier kan skade enheten."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Tilbakestill datalogg?\n\nAlle opptak vil bli slettet."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUKSESS\n\nDataloggen er tilbakestilt."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Øyeblikkelig"
#define STRING_AVERAGE "Gjennomsnitt"
#define STRING_CUMULATIVE "Kumulativ"
#define STRING_HISTORY "Historikk"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 t"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 u"
#define STRING_HISTORY_1_MONTH "1 m"
#define STRING_HISTORY_1_YEAR "1 å"

// Measurement view secondary view strings
#define STRING_TIME "Tid"
#define STRING_MAX "Maks"
#define STRING_RATE "Hastighet"
#define STRING_DOSE "Dose"

// Measurement view info strings
#define STRING_ALERT_ALARM "ALARM"
#define STRING_ALERT_WARNING "ADVARSEL"
#define STRING_ALERT_FAULT "FEIL"
#define STRING_ALERT_MAX "MAKS"
#define STRING_ALERT_DONE "FERDIG"

// Settings menu items
#define STRING_SETTINGS "Innstillinger"
#define STRING_PULSES "Pulser"
#define STRING_ALERTS "Varsler"
#define STRING_MEASUREMENTS "Målinger"
#define STRING_GEIGER_TUBE "Geiger-rør"
#define STRING_DATALOG "Datalogg"
#define STRING_DISPLAY "Skjerm"
#define STRING_DATE_AND_TIME "Dato og tid"
#define STRING_BATTERY_TYPE "Batteritype"
#define STRING_RANDOM_GENERATOR "Tilfeldig generator"
#define STRING_GAME "Spill"
#define STRING_STATISTICS "Statistikk"
#define STRING_DATAMODE "Datamodus"

// Pulses menu items
#define STRING_SOUND "Lyd"
#define STRING_VOICE "Stemme"
#define STRING_VIBRATION "Vibrasjon"
#define STRING_PULSE_LED "Puls-LED"
#define STRING_DISPLAY_FLASH "Skjermblink"
#define STRING_THRESHOLD "Terskel"

// Alarms menu items
#define STRING_RATE_WARNING "Hastighetsadvarsel"
#define STRING_RATE_ALARM "Hastighetsalarm"
#define STRING_DOSE_WARNING "Doseadvarsel"
#define STRING_DOSE_ALARM "Dosealarm"
#define STRING_INDICATION "Indikasjon"

// Measurements menu items
#define STRING_UNITS "Enheter"
#define STRING_SECONDARY_UNITS "Sekundære enheter"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptiv rask"
#define STRING_ADAPTIVE_PRECISION "Adaptiv presisjon"

// Average menu items
#define STRING_UNLIMITED "Ubegrenset"
#define STRING_50_CONFIDENCE "±50% konfidens"
#define STRING_20_CONFIDENCE "±20% konfidens"
#define STRING_10_CONFIDENCE "±10% konfidens"
#define STRING_5_CONFIDENCE "±5% konfidens"
#define STRING_2_CONFIDENCE "±2% konfidens"
#define STRING_1_CONFIDENCE "±1% konfidens"

// Geiger tube menu items
#define STRING_SENSITIVITY "Følsomhet"
#define STRING_DEAD_TIME_COMPENSATION "Dødtidskompensasjon"
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
#define STRING_FACTORY_DEFAULT "Fabrikkstandard"
#define STRING_ACCURACY "Nøyaktighet"
#define STRING_ENERGY_SAVING "Energisparing"
#define STRING_CUSTOM "Tilpasset"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM-frekvens"
#define STRING_PWMDUTY_CYCLE "PWM-arbeidssyklus"

// Data Log menu items
#define STRING_LOGGING_MODE "Loggingsmodus"
#define STRING_RESET "Tilbakestill"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Hver time"
#define STRING_EVERY10_MINUTES "Hvert 10. minutt"
#define STRING_EVERY_MINUTE "Hvert minutt"
#define STRING_EVERY10_SECONDS "Hvert 10. sekund"
#define STRING_EVERY_SECOND "Hvert sekund"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Lysstyrke"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Sovemodus"

// Display - Theme menu items
#define STRING_THEME_DAY "Dag"
#define STRING_THEME_DUSK "Skumring"
#define STRING_THEME_NIGHT "Natt"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nivå"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Alltid av"
#define STRING_ALWAYS_ON "Alltid på"

// Sound menu items
#define STRING_ALERTSTYLE "Varselstil"
#define STRING_ALERTVOLUME "Varselvolum"
#define STRING_VOICEVOLUME "Stemmevolum"

// Sound Alarms length
#define STRING_SHORT "Kort"
#define STRING_LONG "Lang"

// Sound pulses menu items
#define STRING_CLICKS "Klikk"
#define STRING_CHIRPS "Kvirr"
#define STRING_BEEPS "Pip"

// Time and date menu items
#define STRING_TIME_ZONE "Tidssone"
#define STRING_YEAR "År"
#define STRING_MONTH "Måned"
#define STRING_DAY "Dag"
#define STRING_HOUR "Time"
#define STRING_MINUTE "Minutt"
#define STRING_TIME_FORMAT "Tidsformat"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-timers"
#define STRING_12_HOUR "12-timers"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalisk"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerisk"
#define STRING_HEXADECIMAL "Heksadesimal"
#define STRING_DECIMAL "Desimal"
#define STRING_BINARY "Binær"
#define STRING_100_SIDED_DIE "100-sidig terning (0-99)"
#define STRING_20_SIDED_DIE "20-sidig terning"
#define STRING_12_SIDED_DIE "12-sidig terning"
#define STRING_10_SIDED_DIE "10-sidig terning (0-9)"
#define STRING_8_SIDED_DIE "8-sidig terning"
#define STRING_6_SIDED_DIE "6-sidig terning"
#define STRING_4_SIDED_DIE "4-sidig terning"
#define STRING_COIN_FLIP "Myntkast (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Spill hvit"
#define STRING_PLAY_BLACK "Spill svart"
#define STRING_CONTINUE_GAME "Fortsett spill"
#define STRING_NEW_GAME "Nytt spill"
#define STRING_STRENGTH "Styrke"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Nivå"

// Statistics view items
#define STRING_TUBE_LIFE "Rørets levetid"
#define STRING_TUBE "Rør"
#define STRING_LIFE "Levetid"
#define STRING_DEAD_TIME "Dødtid"
#define STRING_DEVICE_ID "Enhets-ID"
#define STRING_DEVICE "Enhet"
#define STRING_ID "ID"
#define STRING_BATTERY "Batteri"
