/*
 * Rad Pro
 * UI strings: Hungarian
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
#define STRING_TEMPERATURE "C"

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "db"
#define STRING_COUNTS "db"

// Common time menu items
#define STRING_24_HOURS "24 óra"
#define STRING_12_HOURS "12 óra"
#define STRING_6_HOURS "6 óra"
#define STRING_3_HOURS "3 óra"
#define STRING_1_HOUR "1 óra"
#define STRING_30_MINUTES "30 perc"
#define STRING_10_MINUTES "10 perc"
#define STRING_5_MINUTES "5 perc"
#define STRING_2_MINUTES "2 perc"
#define STRING_1_MINUTE "1 perc"
#define STRING_30_SECONDS "30 másodperc"
#define STRING_10_SECONDS "10 másodperc"
#define STRING_5_SECONDS "5 másodperc"
#define STRING_1_SECOND "1 másodperc"

// Common menu items
#define STRING_OFF "Ki"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Nagyon alacsony"
#define STRING_LOW "Alacsony"
#define STRING_MEDIUM "Közepes"
#define STRING_HIGH "Magas"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "FIGYELMEZTETÉS\n\nFirmware ellenőrzőösszeg-hiba."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "FIGYELMEZTETÉS\n\nHelytelen értékek károsíthatják a készüléket."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Adatnapló visszaállítása?\n\nMinden rekord törlődik."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SIKER\n\nAz adatnapló visszaállítva."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Azonnali"
#define STRING_AVERAGE "Átlag"
#define STRING_CUMULATIVE "Összesített"
#define STRING_HISTORY "Előzmények"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 p"
#define STRING_HISTORY_1_HOUR "1 ó"
#define STRING_HISTORY_1_DAY "1 n"
#define STRING_HISTORY_1_WEEK "1 h"
#define STRING_HISTORY_1_MONTH "1 hó"
#define STRING_HISTORY_1_YEAR "1 é"

// Measurement view secondary view strings
#define STRING_TIME "Idő"
#define STRING_MAX "Max"
#define STRING_RATE "Ráta"
#define STRING_DOSE "Dózis"

// Measurement view info strings
#define STRING_ALERT_ALARM "RIASZTÁS"
#define STRING_ALERT_WARNING "FIGYELMEZTETÉS"
#define STRING_ALERT_FAULT "HIBA"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_DONE "KÉSZ"

// Settings menu items
#define STRING_SETTINGS "Beállítások"
#define STRING_PULSES "Impulzusok"
#define STRING_ALERTS "Riasztások"
#define STRING_MEASUREMENTS "Mérések"
#define STRING_GEIGER_TUBE "Geiger-cső"
#define STRING_DATALOG "Adatnapló"
#define STRING_DISPLAY "Kijelző"
#define STRING_DATE_AND_TIME "Dátum és idő"
#define STRING_BATTERY_TYPE "Akkumulátor típusa"
#define STRING_RANDOM_GENERATOR "Véletlenszám-generátor"
#define STRING_GAME "Játék"
#define STRING_STATISTICS "Statisztika"
#define STRING_DATAMODE "Adatmód"

// Pulses menu items
#define STRING_SOUND "Hang"
#define STRING_VOICE "Beszéd"
#define STRING_VIBRATION "Rezgés"
#define STRING_PULSE_LED "Impulzus LED"
#define STRING_DISPLAY_FLASH "Kijelző villogás"
#define STRING_THRESHOLD "Küszöb"

// Alarms menu items
#define STRING_RATE_WARNING "Rátafigyelmeztetés"
#define STRING_RATE_ALARM "Rátariasztás"
#define STRING_DOSE_WARNING "Dózisfigyelmeztetés"
#define STRING_DOSE_ALARM "Dózisriasztás"
#define STRING_INDICATION "Jelzés"

// Measurements menu items
#define STRING_UNITS "Mértékegységek"
#define STRING_SECONDARY_UNITS "Másodlagos egységek"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptív gyors"
#define STRING_ADAPTIVE_PRECISION "Adaptív pontos"

// Average menu items
#define STRING_UNLIMITED "Korlátlan"
#define STRING_50_CONFIDENCE "±50% megbízhatóság"
#define STRING_20_CONFIDENCE "±20% megbízhatóság"
#define STRING_10_CONFIDENCE "±10% megbízhatóság"
#define STRING_5_CONFIDENCE "±5% megbízhatóság"
#define STRING_2_CONFIDENCE "±2% megbízhatóság"
#define STRING_1_CONFIDENCE "±1% megbízhatóság"

// Geiger tube menu items
#define STRING_SENSITIVITY "Érzékenység"
#define STRING_DEAD_TIME_COMPENSATION "Holtidő-kompenzáció"
#define STRING_HVPROFILE "HV profil"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (alap)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Gyári alapértelmezett"
#define STRING_ACCURACY "Pontosság"
#define STRING_ENERGY_SAVING "Energiatakarékos"
#define STRING_CUSTOM "Egyéni"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM frekvencia"
#define STRING_PWMDUTY_CYCLE "PWM kitöltési tényező"

// Data Log menu items
#define STRING_LOGGING_MODE "Naplózási mód"
#define STRING_RESET "Törlés"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Minden órában"
#define STRING_EVERY10_MINUTES "10 percenként"
#define STRING_EVERY_MINUTE "Percenként"
#define STRING_EVERY10_SECONDS "10 másodpercenként"
#define STRING_EVERY_SECOND "Másodpercenként"

// Display menu items
#define STRING_THEME "Téma"
#define STRING_BRIGHTNESS "Fényerő"
#define STRING_CONTRAST "Kontraszt"
#define STRING_SLEEP "Alvó mód"

// Display - Theme menu items
#define STRING_THEME_DAY "Nappal"
#define STRING_THEME_DUSK "Alkony"
#define STRING_THEME_NIGHT "Éjszaka"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Szint"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Mindig ki"
#define STRING_ALWAYS_ON "Mindig be"

// Sound menu items
#define STRING_ALERTSTYLE "Riasztási stílus"
#define STRING_ALERTVOLUME "Riasztási hangerő"
#define STRING_VOICEVOLUME "Beszédhangerő"

// Sound Alarms length
#define STRING_SHORT "Rövid"
#define STRING_LONG "Hosszú"

// Sound pulses menu items
#define STRING_CLICKS "Kattogás"
#define STRING_CHIRPS "Csipogás"
#define STRING_BEEPS "Sípolás"

// Time and date menu items
#define STRING_TIME_ZONE "Időzóna"
#define STRING_YEAR "Év"
#define STRING_MONTH "Hónap"
#define STRING_DAY "Nap"
#define STRING_HOUR "Óra"
#define STRING_MINUTE "Perc"
#define STRING_TIME_FORMAT "Időformátum"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "DE"
#define STRING_PM "DU"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 órás"
#define STRING_12_HOUR "12 órás"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkáli"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerikus"
#define STRING_HEXADECIMAL "Hexadecimális"
#define STRING_DECIMAL "Decimális"
#define STRING_BINARY "Bináris"
#define STRING_100_SIDED_DIE "100 oldalú kocka (0-99)"
#define STRING_20_SIDED_DIE "20 oldalú kocka"
#define STRING_12_SIDED_DIE "12 oldalú kocka"
#define STRING_10_SIDED_DIE "10 oldalú kocka (0-9)"
#define STRING_8_SIDED_DIE "8 oldalú kocka"
#define STRING_6_SIDED_DIE "6 oldalú kocka"
#define STRING_4_SIDED_DIE "4 oldalú kocka"
#define STRING_COIN_FLIP "Érmefeldobás (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Fehérrel játszani"
#define STRING_PLAY_BLACK "Feketével játszani"
#define STRING_CONTINUE_GAME "Játék folytatása"
#define STRING_NEW_GAME "Új játék"
#define STRING_STRENGTH "Erősség"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Szint"

// Statistics view items
#define STRING_TUBE_LIFE "Cső élettartama"
#define STRING_TUBE "Cső"
#define STRING_LIFE "Élettartam"
#define STRING_DEAD_TIME "Holtidő"
#define STRING_DEVICE_ID "Eszköz ID"
#define STRING_DEVICE "Eszköz"
#define STRING_ID "ID"
#define STRING_BATTERY "Akkumulátor"
