/*
 * Rad Pro
 * UI strings: German
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
#define STRING_COUNT "Imp."
#define STRING_COUNTS "Imp."

// Common time menu items
#define STRING_24_HOURS "24 Stunden"
#define STRING_12_HOURS "12 Stunden"
#define STRING_6_HOURS "6 Stunden"
#define STRING_3_HOURS "3 Stunden"
#define STRING_1_HOUR "1 Stunde"
#define STRING_30_MINUTES "30 Minuten"
#define STRING_10_MINUTES "10 Minuten"
#define STRING_5_MINUTES "5 Minuten"
#define STRING_2_MINUTES "2 Minuten"
#define STRING_1_MINUTE "1 Minute"
#define STRING_30_SECONDS "30 Sekunden"
#define STRING_10_SECONDS "10 Sekunden"
#define STRING_5_SECONDS "5 Sekunden"
#define STRING_1_SECOND "1 Sekunde"

// Common menu items
#define STRING_OFF "Aus"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "WARNUNG"
#define STRING_NOTIFICATION_SUCCESS "ERFOLG"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Firmware-Fehler."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Falsche Werte schaden Gerät."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Zurücksetzen?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Alle Daten werden gelöscht."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Datenlog zurückgesetzt."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Momentan"
#define STRING_AVERAGE "Durchschnitt"
#define STRING_CUMULATIVE "Kumulativ"
#define STRING_HISTORY10_MIN "Verlauf (10 Min)"
#define STRING_HISTORY1_H "Verlauf (1 Std)"
#define STRING_HISTORY24_H "Verlauf (24 Std)"

// Measurement view secondary view strings
#define STRING_TIME "Zeit"
#define STRING_MAX "Max"
#define STRING_RATE "Rate"
#define STRING_DOSE "Dosis"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ALARM"
#define STRING_INFO_FAULT "FEHLER"
#define STRING_INFO_MAX "MAX"
#define STRING_INFO_OVER "ÜBER"
#define STRING_INFO_DONE "FERTIG"

// Settings menu items
#define STRING_SETTINGS "Einstellungen"
#define STRING_PULSES "Impulse"
#define STRING_ALARMS "Alarme"
#define STRING_MEASUREMENTS "Messungen"
#define STRING_GEIGER_TUBE "Zählröhre"
#define STRING_DATA_LOG "Datenlog"
#define STRING_DISPLAY "Anzeige"
#define STRING_DATE_AND_TIME "Datum und Uhrzeit"
#define STRING_BATTERY_TYPE "Batterietyp"
#define STRING_RANDOM_GENERATOR "Zufallsgenerator"
#define STRING_GAME "Spiel"
#define STRING_STATISTICS "Statistiken"
#define STRING_DATA_MODE "Datenmodus"

// Pulses menu items
#define STRING_SOUND "Ton"
#define STRING_VIBRATION "Vibration"
#define STRING_ALERT_LED "Warn-LED"
#define STRING_PULSE_LED "Puls-LED"
#define STRING_DISPLAY_FLASH "Anzeigeblitz"
#define STRING_THRESHOLD "Schwelle"

// Sound menu items
#define STRING_CLICKS "Klicks"
#define STRING_CHIRPS "Chirps"
#define STRING_BEEPS "Pieptöne"

// Alarms menu items
#define STRING_RATE_ALARM "Dosisratenalarm"
#define STRING_DOSE_ALARM "Dosisalarm"
#define STRING_OVERRANGE_ALARM "Überbereichsalarm"
#define STRING_INDICATION "Anzeige"

// Measurements menu items
#define STRING_UNITS "Einheiten"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptiv schnell"
#define STRING_ADAPTIVE_PRECISION "Adaptiv präzise"

// Average menu items
#define STRING_UNLIMITED "Unbegrenzt"
#define STRING_50_CONFIDENCE "±50% Konfidenz"
#define STRING_20_CONFIDENCE "±20% Konfidenz"
#define STRING_10_CONFIDENCE "±10% Konfidenz"
#define STRING_5_CONFIDENCE "±5% Konfidenz"
#define STRING_2_CONFIDENCE "±2% Konfidenz"
#define STRING_1_CONFIDENCE "±1% Konfidenz"

// Geiger tube menu items
#define STRING_SENSITIVITY "Empfindlichkeit"
#define STRING_DEAD_TIME_COMPENSATION "Totzeitkompensation"
#define STRING_HVPROFILE "HV-Profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (Standard)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Werkseinstellung"
#define STRING_ACCURACY "Genauigkeit"
#define STRING_ENERGY_SAVING "Energiesparend"
#define STRING_CUSTOM "Benutzerdefiniert"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM-Frequenz"
#define STRING_PWMDUTY_CYCLE "PWM-Tastverhältnis"

// Data Log menu items
#define STRING_LOGGING_MODE "Logmodus"
#define STRING_RESET "Zurücksetzen"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Jede Stunde"
#define STRING_EVERY10_MINUTES "Alle 10 Minuten"
#define STRING_EVERY_MINUTE "Jede Minute"
#define STRING_EVERY10_SECONDS "Alle 10 Sekunden"
#define STRING_EVERY_SECOND "Jede Sekunde"

// Display menu items
#define STRING_THEME "Thema"
#define STRING_BRIGHTNESS "Helligkeit"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Ruhemodus"

// Display - Theme menu items
#define STRING_THEME_DAY "Tag"
#define STRING_THEME_DUSK "Dämmerung"
#define STRING_THEME_NIGHT "Nacht"

// Display - Brightness menu items
#define STRING_VERY_LOW "Sehr niedrig"
#define STRING_LOW "Niedrig"
#define STRING_MEDIUM "Mittel"
#define STRING_HIGH "Hoch"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Stufe"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Immer ausgeschaltet"
#define STRING_ALWAYS_ON "Immer eingeschaltet"

// Time and date menu items
#define STRING_TIME_ZONE "Zeitzone"
#define STRING_YEAR "Jahr"
#define STRING_MONTH "Monat"
#define STRING_DAY "Tag"
#define STRING_HOUR "Stunde"
#define STRING_MINUTE "Minute"
#define STRING_TIME_FORMAT "Zeitformat"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-Stunden"
#define STRING_12_HOUR "12-Stunden"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkali-Mangan"

// Random generator menu items
#define STRING_FULL_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alphanumerisch"
#define STRING_HEXADECIMAL "Hexadezimal"
#define STRING_DECIMAL "Dezimal"
#define STRING_BINARY "Binär"
#define STRING_100_SIDED_DIE "100-seitiger Würfel (0-99)"
#define STRING_20_SIDED_DIE "20-seitiger Würfel"
#define STRING_12_SIDED_DIE "12-seitiger Würfel"
#define STRING_10_SIDED_DIE "10-seitiger Würfel (0-9)"
#define STRING_8_SIDED_DIE "8-seitiger Würfel"
#define STRING_6_SIDED_DIE "6-seitiger Würfel"
#define STRING_4_SIDED_DIE "4-seitiger Würfel"
#define STRING_COIN_FLIP "Münzwurf"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Kopf"
#define STRING_COIN_TAIL "Zahl"

// Game menu items
#define STRING_PLAY_WHITE "Weiß spielen"
#define STRING_PLAY_BLACK "Schwarz spielen"
#define STRING_CONTINUE_GAME "Spiel fortsetzen"
#define STRING_NEW_GAME "Neues Spiel"
#define STRING_STRENGTH "Stärke"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Stufe"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Röhrenzeit"
#define STRING_TUBE_LIFE_PULSES "Impulse"
#define STRING_TUBE_DEAD_TIME "Totzeit"
#define STRING_DEVICE_ID "Geräte-ID"
#define STRING_DEVICE_VOLTAGE "Spannung"

#define STRING_TUBE "Zählröhre"
#define STRING_LIFE_TIME "Lebenszeit"
#define STRING_LIFE_PULSES "Impulse"
#define STRING_DEAD_TIME "Totzeit"
#define STRING_DEVICE "Gerät"
#define STRING_ID "ID"
#define STRING_VOLTAGE "Spannung"
