/*
 * Rad Pro
 * UI strings: Hungarian
 *
 * (C) 2022-2026 Gissio
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
#define STRING_CPMUSVH "cpm/µSv/h"
#define STRING_MICROSECONDS "µs"
#define STRING_KHZ "kHz"
#define STRING_VOLT_UNIT "V"
#define STRING_VOLT_PER_METER_UNIT "V/m"
#define STRING_TESLA "Tesla"
#define STRING_TESLA_UNIT "T"
#define STRING_GAUSS "Gauss"
#define STRING_GAUSS_UNIT "G"
#define STRING_W_M2 "W/m²"

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "db"
#define STRING_COUNTS "db"

// Time strings
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

// Strength strings
#define STRING_OFF "Ki"
#define STRING_VERY_LOW "Nagyon alacsony"
#define STRING_LOW "Alacsony"
#define STRING_MEDIUM "Közepes"
#define STRING_HIGH "Magas"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "FIGYELMEZTETÉS\n\nFirmware ellenőrzőösszeg-hiba."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "FIGYELMEZTETÉS\n\nHelytelen értékek károsíthatják a készüléket."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Adatnapló visszaállítása?\n\nMinden rekord törlődik."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SIKER\n\nAz adatnapló visszaállítva."

// Measurements title strings
#define STRING_INSTANTANEOUS "Azonnali"
#define STRING_AVERAGE "Átlag"
#define STRING_CUMULATIVE "Összesített"
#define STRING_HISTORY "Előzmények"
#define STRING_ELECTRIC_FIELD "Elektromos tér"
#define STRING_MAGNETIC_FIELD "Mágneses tér"

// Measurements secondary view strings
#define STRING_TIME "Idő"
#define STRING_MAX "Max"
#define STRING_RATE "Ráta"
#define STRING_DOSE "Dózis"
#define STRING_POWER_DENSITY "Teljesítménysűrűség"

// Measurements info strings
#define STRING_ALERT_FAULT "HIBA"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_DONE "KÉSZ"
#define STRING_ALERT_UNAVAILABLE "NEM ELÉRHETŐ"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 p"
#define STRING_HISTORY_1_HOUR "1 ó"
#define STRING_HISTORY_1_DAY "1 n"
#define STRING_HISTORY_1_WEEK "1 h"
#define STRING_HISTORY_1_MONTH "1 hó"
#define STRING_HISTORY_1_YEAR "1 é"

// Settings menu items
#define STRING_SETTINGS "Beállítások"
#define STRING_PULSES "Impulzusok"
#define STRING_ALERTS "Riasztások"
#define STRING_MEASUREMENTS "Mérések"
#define STRING_GEIGER_TUBE "Geiger-cső"
#define STRING_DATALOG "Adatnapló"
#define STRING_DISPLAY "Kijelző"
#define STRING_DATE_AND_TIME "Dátum és idő"
#define STRING_POWER "Tápellátás"
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

// Alerts menu items
#define STRING_INDICATION "Jelzés"
#define STRING_RATE_WARNING "Rátafigyelmeztetés"
#define STRING_RATE_ALARM "Rátariasztás"
#define STRING_DOSE_WARNING "Dózisfigyelmeztetés"
#define STRING_DOSE_ALARM "Dózisriasztás"
#define STRING_ELECTRIC_FIELD_ALARM "Elektromos tér riasztás"
#define STRING_MAGNETIC_FIELD_ALARM "Mágneses tér riasztás"

// Measurements menu items
#define STRING_SOURCE "Forrás"
#define STRING_DOSE_UNITS "Dózis mértékegységek"
#define STRING_SECONDARY_DOSE_UNITS "Másodlagos dózis mértékegységek"
#define STRING_MAGNETIC_FIELD_UNITS "Mágneses tér mértékegységek"

// Source menu items
#define STRING_CS137 "Alapértelmezett: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Rádium (γ)"
#define STRING_URANIUM_ORE "Uránérc (γ)"
#define STRING_URANIUM_GLASS "Uránüveg (γ)"
#define STRING_DEPLETED_URANIUM "Szegényített urán (γ)"
#define STRING_THORIUM_ORE "Tóriumérc (γ)"
#define STRING_XRAYS "Röntgen (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Háttérsugárzás"

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
#define STRING_TUBE_TYPE "Cső típusa"
#define STRING_SENSITIVITY "Érzékenység (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Holtidő-kompenzáció"
#define STRING_HVPROFILE "HV profil"

// Geiger tube type strings
#define STRING_J305 "J305 (107 mm)"
#define STRING_M4011 "M4011 (87 mm)"
#define STRING_HH614 "HH614"
#define STRING_LND7317 "LND 7317"

// Geiger tube type strings that should be translated with cyrillic scripts
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Alapértelmezett cső"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Gyári alapértelmezett"
#define STRING_ACCURACY "Pontosság"
#define STRING_ENERGY_SAVING "Energiatakarékos"
#define STRING_CUSTOM "Egyéni"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "PWM frekvencia"
#define STRING_PWMDUTY_CYCLE "PWM kitöltési tényező"

// Data log menu items
#define STRING_LOGGING_MODE "Naplózási mód"
#define STRING_RESET "Törlés"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "Nappal"
#define STRING_THEME_DUSK "Alkony"
#define STRING_THEME_NIGHT "Éjszaka"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Szint"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Mindig ki"
#define STRING_ALWAYS_ON "Mindig be"

// Sound menu items
#define STRING_ALERTSTYLE "Riasztási stílus"
#define STRING_ALERTVOLUME "Riasztási hangerő"
#define STRING_VOICEVOLUME "Beszédhangerő"

// Sound alert style menu items
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

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "DE"
#define STRING_PM "DU"

// Time format menu items
#define STRING_24_HOUR "24 órás"
#define STRING_12_HOUR "12 órás"

// Power menu items
#define STRING_BATTERY "Akkumulátor"
#define STRING_USB_AUTO_POWER_ON "USB automatikus bekapcsolás"

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
