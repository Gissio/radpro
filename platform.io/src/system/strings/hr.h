/*
 * Rad Pro
 * UI strings: Croatian
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

// Application name
#define STRING_APP_NAME "Rad Pro"

// Styling characters
#define STRING_EMPTY ""
#define STRING_NUMBERS "0123456789.:"
#define STRING_ELLIPSIS "..."
#define STRING_PLUSMINUS "±"
#define STRING_PERCENT "%"
#define STRING_NONE "-"
#define STRING_LESSTHAN "< "

// Units
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

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "imp."
#define STRING_COUNTS "imp."

// Time strings
#define STRING_24_HOURS "24 sata"
#define STRING_12_HOURS "12 sati"
#define STRING_6_HOURS "6 sati"
#define STRING_3_HOURS "3 sata"
#define STRING_1_HOUR "1 sat"
#define STRING_30_MINUTES "30 minuta"
#define STRING_10_MINUTES "10 minuta"
#define STRING_5_MINUTES "5 minuta"
#define STRING_2_MINUTES "2 minute"
#define STRING_1_MINUTE "1 minuta"
#define STRING_30_SECONDS "30 sekundi"
#define STRING_10_SECONDS "10 sekundi"
#define STRING_5_SECONDS "5 sekundi"
#define STRING_1_SECOND "1 sekunda"

// Strength strings
#define STRING_OFF "Isključeno"
#define STRING_VERY_LOW "Vrlo nisko"
#define STRING_LOW "Nisko"
#define STRING_MEDIUM "Srednje"
#define STRING_HIGH "Visoko"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "UPOZORENJE\n\nNeuspjeh provjere kontrolnog zbroja firmvera."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "UPOZORENJE\n\nPogrešne vrijednosti mogu oštetiti uređaj."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Resetirati dnevnik podataka?\n\nSvi zapisi će biti izbrisani."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "USPJEH\n\nDnevnik podataka je resetiran."

// Measurements title strings
#define STRING_INSTANTANEOUS "Trenutno"
#define STRING_AVERAGE "Prosjek"
#define STRING_CUMULATIVE "Kumulativno"
#define STRING_HISTORY "Povijest"
#define STRING_ELECTRIC_FIELD "Električno polje"
#define STRING_MAGNETIC_FIELD "Magnetsko polje"

// Measurements secondary view strings
#define STRING_TIME "Vrijeme"
#define STRING_MAX "Maks"
#define STRING_RATE "Brzina"
#define STRING_DOSE "Doza"

// Measurements info strings
#define STRING_ALERT_FAULT "KVAR"
#define STRING_ALERT_MAX "MAKS"
#define STRING_ALERT_DONE "GOTOVO"
#define STRING_ALERT_UNAVAILABLE "NEDOSTUPNO"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 tj"
#define STRING_HISTORY_1_MONTH "1 mj"
#define STRING_HISTORY_1_YEAR "1 g"

// Settings menu items
#define STRING_SETTINGS "Postavke"
#define STRING_PULSES "Impulsi"
#define STRING_ALERTS "Upozorenja"
#define STRING_MEASUREMENTS "Mjerenja"
#define STRING_GEIGER_TUBE "Geigerova cijev"
#define STRING_DATALOG "Zapis podataka"
#define STRING_DISPLAY "Zaslon"
#define STRING_DATE_AND_TIME "Datum i vrijeme"
#define STRING_POWER "Napajanje"
#define STRING_RANDOM_GENERATOR "Slučajni generator"
#define STRING_GAME "Igra"
#define STRING_STATISTICS "Statistika"
#define STRING_DATAMODE "Način podataka"

// Pulses menu items
#define STRING_SOUND "Zvuk"
#define STRING_VOICE "Glas"
#define STRING_VIBRATION "Vibracija"
#define STRING_PULSE_LED "LED impulsa"
#define STRING_DISPLAY_FLASH "Bljeskalica zaslona"
#define STRING_THRESHOLD "Prag"

// Alerts menu items
#define STRING_INDICATION "Indikacija"
#define STRING_RATE_WARNING "Upozorenje brzine"
#define STRING_RATE_ALARM "Alarm brzine"
#define STRING_DOSE_WARNING "Upozorenje doze"
#define STRING_DOSE_ALARM "Alarm doze"
#define STRING_ELECTRIC_FIELD_ALARM "Alarm električnog polja"
#define STRING_MAGNETIC_FIELD_ALARM "Alarm magnetskog polja"

// Measurements menu items
#define STRING_SOURCE "Izvor"
#define STRING_DOSE_UNITS "Jedinice doze"
#define STRING_SECONDARY_DOSE_UNITS "Sekundarne jedinice doze"
#define STRING_MAGNETIC_FIELD_UNITS "Jedinice magnetskog polja"

// Source menu items
#define STRING_CS137 "Zadano: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Radij (γ)"
#define STRING_URANIUM_ORE "Uranova ruda (γ)"
#define STRING_URANIUM_GLASS "Uranovo staklo (γ)"
#define STRING_DEPLETED_URANIUM "Osiromašeni uranij (γ)"
#define STRING_THORIUM_ORE "Torijeva ruda (γ)"
#define STRING_XRAYS "Rendgen (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Pozadinsko zračenje"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Prilagodljivo brzo"
#define STRING_ADAPTIVE_PRECISION "Prilagodljiva preciznost"

// Average menu items
#define STRING_UNLIMITED "Neograničeno"
#define STRING_50_CONFIDENCE "±50% pouzdanost"
#define STRING_20_CONFIDENCE "±20% pouzdanost"
#define STRING_10_CONFIDENCE "±10% pouzdanost"
#define STRING_5_CONFIDENCE "±5% pouzdanost"
#define STRING_2_CONFIDENCE "±2% pouzdanost"
#define STRING_1_CONFIDENCE "±1% pouzdanost"

// Geiger tube menu items
#define STRING_TUBE_TYPE "Tip cijevi"
#define STRING_SENSITIVITY "Osjetljivost (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Mrtvo vrijeme kompenzacija"
#define STRING_HVPROFILE "HV profil"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011/J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Zadana cijev"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Tvornički zadano"
#define STRING_ACCURACY "Točnost"
#define STRING_ENERGY_SAVING "Ušteda energije"
#define STRING_CUSTOM "Prilagođeno"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "PWM frekvencija"
#define STRING_PWMDUTY_CYCLE "PWM radni ciklus"

// Data log menu items
#define STRING_LOGGING_MODE "Način zapisivanja"
#define STRING_RESET "Poništi"

// Data log options menu items
#define STRING_EVERY_HOUR "Svaki sat"
#define STRING_EVERY10_MINUTES "Svakih 10 minuta"
#define STRING_EVERY_MINUTE "Svaku minutu"
#define STRING_EVERY10_SECONDS "Svakih 10 sekundi"
#define STRING_EVERY_SECOND "Svaku sekundu"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Svjetlina"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Spavanje"

// Display theme menu items
#define STRING_THEME_DAY "Dan"
#define STRING_THEME_DUSK "Sumrak"
#define STRING_THEME_NIGHT "Noć"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Razina"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Uvijek isključeno"
#define STRING_ALWAYS_ON "Uvijek uključeno"

// Sound menu items
#define STRING_ALERTSTYLE "Stil upozorenja"
#define STRING_ALERTVOLUME "Glasnoća upozorenja"
#define STRING_VOICEVOLUME "Glasnoća glasa"

// Sound alert style menu items
#define STRING_SHORT "Kratak"
#define STRING_LONG "Dug"

// Sound pulses menu items
#define STRING_CLICKS "Klikovi"
#define STRING_CHIRPS "Cvrkuti"
#define STRING_BEEPS "Bipovi"

// Time and date menu items
#define STRING_TIME_ZONE "Vremenska zona"
#define STRING_YEAR "Godina"
#define STRING_MONTH "Mjesec"
#define STRING_DAY "Dan"
#define STRING_HOUR "Sat"
#define STRING_MINUTE "Minuta"
#define STRING_TIME_FORMAT "Format vremena"

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24-satni"
#define STRING_12_HOUR "12-satni"

// Power menu items
#define STRING_BATTERY "Baterija"
#define STRING_USB_AUTO_POWER_ON "USB automatsko uključivanje"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalna"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerički"
#define STRING_HEXADECIMAL "Heksadekadski"
#define STRING_DECIMAL "Decimalni"
#define STRING_BINARY "Binarno"
#define STRING_100_SIDED_DIE "100-strana kocka (0-99)"
#define STRING_20_SIDED_DIE "20-strana kocka"
#define STRING_12_SIDED_DIE "12-strana kocka"
#define STRING_10_SIDED_DIE "10-strana kocka (0-9)"
#define STRING_8_SIDED_DIE "8-strana kocka"
#define STRING_6_SIDED_DIE "6-strana kocka"
#define STRING_4_SIDED_DIE "4-strana kocka"
#define STRING_COIN_FLIP "Bacanje novčića (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Igraj bijelo"
#define STRING_PLAY_BLACK "Igraj crno"
#define STRING_CONTINUE_GAME "Nastavi igru"
#define STRING_NEW_GAME "Nova igra"
#define STRING_STRENGTH "Jačina"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Razina"

// Statistics view items
#define STRING_TUBE_LIFE "Vijek cijevi"
#define STRING_TUBE "Cijev"
#define STRING_LIFE "Trajanja"
#define STRING_DEAD_TIME "Mrtvo vrijeme"
#define STRING_DEVICE_ID "ID uređaja"
#define STRING_DEVICE "Uređaj"
#define STRING_ID "ID"
