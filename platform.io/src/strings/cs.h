/*
 * Rad Pro
 * UI strings: Czech
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
#define STRING_24_HOURS "24 hodin"
#define STRING_12_HOURS "12 hodin"
#define STRING_6_HOURS "6 hodin"
#define STRING_3_HOURS "3 hodiny"
#define STRING_1_HOUR "1 hodina"
#define STRING_30_MINUTES "30 minut"
#define STRING_10_MINUTES "10 minut"
#define STRING_5_MINUTES "5 minut"
#define STRING_2_MINUTES "2 minuty"
#define STRING_1_MINUTE "1 minuta"
#define STRING_30_SECONDS "30 sekund"
#define STRING_10_SECONDS "10 sekund"
#define STRING_5_SECONDS "5 sekund"
#define STRING_1_SECOND "1 sekunda"

// Common menu items
#define STRING_OFF "Vypnuto"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "VAROVÁNÍ"
#define STRING_NOTIFICATION_SUCCESS "ÚSPĚCH"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Selhání firmwaru."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Chyby ničí zařízení."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Reset záznamu?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Všechny záznamy smazány."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Záznam resetován."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Okamžitý"
#define STRING_AVERAGE "Průměrný"
#define STRING_CUMULATIVE "Kumulativní"
#define STRING_HISTORY10_MIN "Historie (10 min)"
#define STRING_HISTORY1_H "Historie (1 h)"
#define STRING_HISTORY24_H "Historie (24 h)"

// Measurement view secondary view strings
#define STRING_TIME "Čas"
#define STRING_MAX "Max"
#define STRING_RATE "Tok"
#define STRING_DOSE "Dávka"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ALARM"
#define STRING_INFO_FAULT "CHYBA"
#define STRING_INFO_MAX "MAX"
#define STRING_INFO_OVER "PŘES"
#define STRING_INFO_DONE "HOTOVO"

// Settings menu items
#define STRING_SETTINGS "Nastavení"
#define STRING_PULSES "Impulzy"
#define STRING_ALARMS "Alarmy"
#define STRING_MEASUREMENTS "Měření"
#define STRING_GEIGER_TUBE "Geigerova trubice"
#define STRING_DATA_LOG "Datový záznam"
#define STRING_DISPLAY "Displej"
#define STRING_DATE_AND_TIME "Datum a čas"
#define STRING_BATTERY_TYPE "Typ baterie"
#define STRING_RANDOM_GENERATOR "Náhodný generátor"
#define STRING_GAME "Hra"
#define STRING_STATISTICS "Statistiky"
#define STRING_DATA_MODE "Datový režim"

// Pulses menu items
#define STRING_SOUND "Zvuk"
#define STRING_VIBRATION "Vibrace"
#define STRING_ALERT_LED "Výstražná LED"
#define STRING_PULSE_LED "Pulsní LED"
#define STRING_DISPLAY_FLASH "Blikání displeje"
#define STRING_THRESHOLD "Práh"

// Sound menu items
#define STRING_CLICKS "Kliknutí"
#define STRING_CHIRPS "Cvakání"
#define STRING_BEEPS "Pípání"

// Alarms menu items
#define STRING_RATE_ALARM "Alarm toku"
#define STRING_DOSE_ALARM "Alarm dávky"
#define STRING_OVERRANGE_ALARM "Alarm rozsahu"
#define STRING_INDICATION "Indikace"

// Measurements menu items
#define STRING_UNITS "Jednotky"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptivní rychlý"
#define STRING_ADAPTIVE_PRECISION "Adaptivní přesný"

// Average menu items
#define STRING_UNLIMITED "Neomezený"
#define STRING_50_CONFIDENCE "±50% spolehlivost"
#define STRING_20_CONFIDENCE "±20% spolehlivost"
#define STRING_10_CONFIDENCE "±10% spolehlivost"
#define STRING_5_CONFIDENCE "±5% spolehlivost"
#define STRING_2_CONFIDENCE "±2% spolehlivost"
#define STRING_1_CONFIDENCE "±1% spolehlivost"

// Geiger tube menu items
#define STRING_SENSITIVITY "Citlivost"
#define STRING_DEAD_TIME_SHORT_COMPENSATION "Komp. mrtvého času"
#define STRING_HVPROFILE "HV profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (výchozí)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Tovární výchozí"
#define STRING_ACCURACY "Přesnost"
#define STRING_ENERGY_SAVING "Úspora energie"
#define STRING_CUSTOM "Vlastní"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Frekvence PWM"
#define STRING_PWMDUTY_CYCLE "Pracovní cyklus PWM"

// Tube/battery voltage
#define STRING_VOLTAGE "Napětí"

// Data Log menu items
#define STRING_LOGGING_MODE "Režim záznamu"
#define STRING_RESET "Reset"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Každou hodinu"
#define STRING_EVERY10_MINUTES "Každých 10 minut"
#define STRING_EVERY_MINUTE "Každou minutu"
#define STRING_EVERY10_SECONDS "Každých 10 sekund"
#define STRING_EVERY_SECOND "Každou sekundu"

// Display menu items
#define STRING_THEME "Téma"
#define STRING_BRIGHTNESS "Jas"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Spánek"

// Display - Theme menu items
#define STRING_THEME_DAY "Den"
#define STRING_THEME_DUSK "Soumrak"
#define STRING_THEME_NIGHT "Noc"

// Display - Brightness menu items
#define STRING_VERY_LOW "Velmi nízký"
#define STRING_LOW "Nízký"
#define STRING_MEDIUM "Střední"
#define STRING_HIGH "Vysoký"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Úroveň"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Vždy vypnuto"
#define STRING_ALWAYS_ON "Vždy zapnuto"

// Time and date menu items
#define STRING_TIME_ZONE "Časové pásmo"
#define STRING_YEAR "Rok"
#define STRING_MONTH "Měsíc"
#define STRING_DAY "Den"
#define STRING_HOUR "Hodina"
#define STRING_MINUTE "Minuta"
#define STRING_TIME_FORMAT "Formát času"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24hodinový"
#define STRING_12_HOUR "12hodinový"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalická"

// Random generator menu items
#define STRING_FULL_ASCII "Plný ASCII"
#define STRING_ALPHANUMERIC "Alfanumerický"
#define STRING_HEXADECIMAL "Hexadecimální"
#define STRING_DECIMAL "Desítkový"
#define STRING_BINARY "Binární"
#define STRING_100_SIDED_DIE "100stěnná kostka (0-99)"
#define STRING_20_SIDED_DIE "20stěnná kostka"
#define STRING_12_SIDED_DIE "12stěnná kostka"
#define STRING_10_SIDED_DIE "10stěnná kostka (0-9)"
#define STRING_8_SIDED_DIE "8stěnná kostka"
#define STRING_6_SIDED_DIE "6stěnná kostka"
#define STRING_4_SIDED_DIE "4stěnná kostka"
#define STRING_COIN_FLIP "Hod mincí"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Hlava"
#define STRING_COIN_TAIL "Orel"

// Game menu items
#define STRING_PLAY_WHITE "Hrát bílé"
#define STRING_PLAY_BLACK "Hrát černé"
#define STRING_CONTINUE_GAME "Pokračovat ve hře"
#define STRING_NEW_GAME "Nová hra"
#define STRING_STRENGTH "Síla"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Úroveň"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Životnost trubice"
#define STRING_DEAD_TIME "Mrtvý čas"
#define STRING_DEVICE_ID "ID zařízení"

#define STRING_TUBE "Trubice"
#define STRING_LIFE_TIME "Životnost"
#define STRING_DEAD_TIME_SHORT "Mrtvý čas"
#define STRING_DEVICE "Zařízení"
#define STRING_ID "ID"
#define STRING_VOLTAGE_SHORT "Napětí"
