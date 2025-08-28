/*
 * Rad Pro
 * UI strings: Polish
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
#define STRING_24_HOURS "24 godziny"
#define STRING_12_HOURS "12 godzin"
#define STRING_6_HOURS "6 godzin"
#define STRING_3_HOURS "3 godziny"
#define STRING_1_HOUR "1 godzina"
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
#define STRING_OFF "Wył."

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Bardzo niska"
#define STRING_LOW "Niska"
#define STRING_MEDIUM "Średnia"
#define STRING_HIGH "Wysoka"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "OSTRZEŻENIE\n\nNiepowodzenie sumy kontrolnej oprogramowania."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "OSTRZEŻENIE\n\nNieprawidłowe wartości mogą uszkodzić urządzenie."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Zresetować dziennik danych?\n\nWszystkie rekordy zostaną usunięte."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUKCES\n\nDziennik danych został zresetowany."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Chwilowe"
#define STRING_AVERAGE "Średnie"
#define STRING_CUMULATIVE "Skumulowane"
#define STRING_HISTORY "Historia"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 tydz"
#define STRING_HISTORY_1_MONTH "1 mies"
#define STRING_HISTORY_1_YEAR "1 r"

// Measurement view secondary view strings
#define STRING_TIME "Czas"
#define STRING_MAX "Maks"
#define STRING_RATE "Natężenie"
#define STRING_DOSE "Dawka"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_ALERT_ALARM "ALARM"
#define STRING_ALERT_WARNING "OSTRZEŻENIE"
#define STRING_ALERT_FAULT "USTERKA"
#define STRING_ALERT_MAX "MAKS"
#define STRING_ALERT_OVERFLOW "PRZEKROCZENIE"
#define STRING_ALERT_DONE "ZAKOŃCZONO"

// Settings menu items
#define STRING_SETTINGS "Ustawienia"
#define STRING_PULSES "Impulsy"
#define STRING_ALERTS "Alerty"
#define STRING_MEASUREMENTS "Pomiary"
#define STRING_GEIGER_TUBE "Lampa Geigera"
#define STRING_DATALOG "Dziennik danych"
#define STRING_DISPLAY "Wyświetlacz"
#define STRING_DATE_AND_TIME "Data i czas"
#define STRING_BATTERY_TYPE "Typ baterii"
#define STRING_RANDOM_GENERATOR "Generator losowy"
#define STRING_GAME "Gra"
#define STRING_STATISTICS "Statystyki"
#define STRING_DATAMODE "Tryb danych"

// Pulses menu items
#define STRING_SOUND "Dźwięk"
#define STRING_VOICE "Głos"
#define STRING_VIBRATION "Wibracja"
#define STRING_PULSE_LED "LED impulsu"
#define STRING_DISPLAY_FLASH "Błysk wyświetlacza"
#define STRING_THRESHOLD "Próg"

// Alarms menu items
#define STRING_RATE_WARNING "Ostrzeżenie natężenia"
#define STRING_RATE_ALARM "Alarm natężenia"
#define STRING_DOSE_WARNING "Ostrzeżenie dawki"
#define STRING_DOSE_ALARM "Alarm dawki"
#define STRING_INDICATION "Wskazanie"

// Measurements menu items
#define STRING_UNITS "Jednostki"
#define STRING_SECONDARY_UNITS "Jednostki wtórne"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptacyjne szybkie"
#define STRING_ADAPTIVE_PRECISION "Adaptacyjne precyzyjne"

// Average menu items
#define STRING_UNLIMITED "Nieograniczone"
#define STRING_50_CONFIDENCE "±50% pewności"
#define STRING_20_CONFIDENCE "±20% pewności"
#define STRING_10_CONFIDENCE "±10% pewności"
#define STRING_5_CONFIDENCE "±5% pewności"
#define STRING_2_CONFIDENCE "±2% pewności"
#define STRING_1_CONFIDENCE "±1% pewności"

// Geiger tube menu items
#define STRING_SENSITIVITY "Czułość"
#define STRING_DEAD_TIME_COMPENSATION "Komp. czasu martwego"
#define STRING_HVPROFILE "Profil HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (domyślny)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Domyślne fabryczne"
#define STRING_ACCURACY "Dokładność"
#define STRING_ENERGY_SAVING "Oszczędzanie energii"
#define STRING_CUSTOM "Niestandardowy"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Częstotliwość PWM"
#define STRING_PWMDUTY_CYCLE "Cykl pracy PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Tryb logowania"
#define STRING_RESET "Resetuj"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Co godzinę"
#define STRING_EVERY10_MINUTES "Co 10 minut"
#define STRING_EVERY_MINUTE "Co minutę"
#define STRING_EVERY10_SECONDS "Co 10 sekund"
#define STRING_EVERY_SECOND "Co sekundę"

// Display menu items
#define STRING_THEME "Motyw"
#define STRING_BRIGHTNESS "Jasność"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Uśpienie"

// Display - Theme menu items
#define STRING_THEME_DAY "Dzień"
#define STRING_THEME_DUSK "Zmierzch"
#define STRING_THEME_NIGHT "Noc"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Poziom"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Zawsze wyłączony"
#define STRING_ALWAYS_ON "Zawsze włączony"

// Sound menu items
#define STRING_ALERTSTYLE "Styl alertu"
#define STRING_ALERTVOLUME "Głośność alertu"
#define STRING_VOICEVOLUME "Głośność głosu"

// Sound Alarms length
#define STRING_SHORT "Krótki"
#define STRING_LONG "Długi"

// Sound pulses menu items
#define STRING_CLICKS "Kliknięcia"
#define STRING_CHIRPS "Ćwierkanie"
#define STRING_BEEPS "Piski"

// Time and date menu items
#define STRING_TIME_ZONE "Strefa czasowa"
#define STRING_YEAR "Rok"
#define STRING_MONTH "Miesiąc"
#define STRING_DAY "Dzień"
#define STRING_HOUR "Godzina"
#define STRING_MINUTE "Minuta"
#define STRING_TIME_FORMAT "Format czasu"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-godzinny"
#define STRING_12_HOUR "12-godzinny"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkaliczne"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumeryczny"
#define STRING_HEXADECIMAL "Szesnastkowy"
#define STRING_DECIMAL "Dziesiętny"
#define STRING_BINARY "Binarny"
#define STRING_100_SIDED_DIE "Kostka 100-ścienna (0-99)"
#define STRING_20_SIDED_DIE "Kostka 20-ścienna"
#define STRING_12_SIDED_DIE "Kostka 12-ścienna"
#define STRING_10_SIDED_DIE "Kostka 10-ścienna (0-9)"
#define STRING_8_SIDED_DIE "Kostka 8-ścienna"
#define STRING_6_SIDED_DIE "Kostka 6-ścienna"
#define STRING_4_SIDED_DIE "Kostka 4-ścienna"
#define STRING_COIN_FLIP "Rzut monetą (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Graj białymi"
#define STRING_PLAY_BLACK "Graj czarnymi"
#define STRING_CONTINUE_GAME "Kontynuuj grę"
#define STRING_NEW_GAME "Nowa gra"
#define STRING_STRENGTH "Siła"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Poziom"

// Statistics view items
#define STRING_TUBE_LIFE "Żywotność lampy"
#define STRING_TUBE "Lampa"
#define STRING_LIFE "Żywotność"
#define STRING_DEAD_TIME "Czas martwy"
#define STRING_DEVICE_ID "ID urządzenia"
#define STRING_DEVICE "Urządzenie"
#define STRING_ID "ID"
#define STRING_BATTERY "Bateria"
