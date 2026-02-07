/*
 * Rad Pro
 * UI strings: Romanian
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
#define STRING_W_M2 "W/m²"

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "imp."
#define STRING_COUNTS "imp."

// Time strings
#define STRING_24_HOURS "24 ore"
#define STRING_12_HOURS "12 ore"
#define STRING_6_HOURS "6 ore"
#define STRING_3_HOURS "3 ore"
#define STRING_1_HOUR "1 oră"
#define STRING_30_MINUTES "30 minute"
#define STRING_10_MINUTES "10 minute"
#define STRING_5_MINUTES "5 minute"
#define STRING_2_MINUTES "2 minute"
#define STRING_1_MINUTE "1 minut"
#define STRING_30_SECONDS "30 secunde"
#define STRING_10_SECONDS "10 secunde"
#define STRING_5_SECONDS "5 secunde"
#define STRING_1_SECOND "1 secundă"

// Strength strings
#define STRING_OFF "Oprit"
#define STRING_VERY_LOW "Foarte scăzut"
#define STRING_LOW "Scăzut"
#define STRING_MEDIUM "Mediu"
#define STRING_HIGH "Ridicat"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "AVERTISMENT\n\nEșec al sumei de control a firmware-ului."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "AVERTISMENT\n\nValorile incorecte pot dăuna dispozitivului."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Resetați jurnalul de date?\n\nToate înregistrările vor fi șterse."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCCES\n\nJurnalul de date a fost resetat."

// Measurements title strings
#define STRING_INSTANTANEOUS "Instantaneu"
#define STRING_AVERAGE "Medie"
#define STRING_CUMULATIVE "Cumulativ"
#define STRING_HISTORY "Istoric"
#define STRING_ELECTRIC_FIELD "Câmp electric"
#define STRING_MAGNETIC_FIELD "Câmp magnetic"

// Measurements secondary view strings
#define STRING_TIME "Timp"
#define STRING_MAX "Max"
#define STRING_RATE "Rată"
#define STRING_DOSE "Doză"
#define STRING_POWER_DENSITY "Densitate de putere"

// Measurements info strings
#define STRING_ALERT_FAULT "DEFECȚIUNE"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_DONE "FINALIZAT"
#define STRING_ALERT_UNAVAILABLE "INDISPONIBIL"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 z"
#define STRING_HISTORY_1_WEEK "1 săpt"
#define STRING_HISTORY_1_MONTH "1 lună"
#define STRING_HISTORY_1_YEAR "1 an"

// Settings menu items
#define STRING_SETTINGS "Setări"
#define STRING_PULSES "Impulsuri"
#define STRING_ALERTS "Alerte"
#define STRING_MEASUREMENTS "Măsurători"
#define STRING_GEIGER_TUBE "Tub Geiger"
#define STRING_DATALOG "Jurnalul de date"
#define STRING_DISPLAY "Afișaj"
#define STRING_DATE_AND_TIME "Dată și oră"
#define STRING_POWER "Alimentare"
#define STRING_RANDOM_GENERATOR "Generator aleator"
#define STRING_GAME "Joc"
#define STRING_STATISTICS "Statistici"
#define STRING_DATAMODE "Mod date"

// Pulses menu items
#define STRING_SOUND "Sunet"
#define STRING_VOICE "Voce"
#define STRING_VIBRATION "Vibrație"
#define STRING_PULSE_LED "LED impuls"
#define STRING_DISPLAY_FLASH "Clipire ecran"
#define STRING_THRESHOLD "Prag"

// Alerts menu items
#define STRING_INDICATION "Indicație"
#define STRING_RATE_WARNING "Avertisment rată"
#define STRING_RATE_ALARM "Alarmă rată"
#define STRING_DOSE_WARNING "Avertisment doză"
#define STRING_DOSE_ALARM "Alarmă doză"
#define STRING_ELECTRIC_FIELD_ALARM "Alarm câmp electric"
#define STRING_MAGNETIC_FIELD_ALARM "Alarm câmp magnetic"

// Measurements menu items
#define STRING_SOURCE "Sursă"
#define STRING_DOSE_UNITS "Unități de doză"
#define STRING_SECONDARY_DOSE_UNITS "Unități de doză secundare"
#define STRING_MAGNETIC_FIELD_UNITS "Unități de câmp magnetic"

// Source menu items
#define STRING_CS137 "Implicit: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Radiu (γ)"
#define STRING_URANIUM_ORE "Minereu de uraniu (γ)"
#define STRING_URANIUM_GLASS "Sticlă cu uraniu (γ)"
#define STRING_DEPLETED_URANIUM "Uran sărăcit (γ)"
#define STRING_THORIUM_ORE "Minereu de toriu (γ)"
#define STRING_XRAYS "Radiație X (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Radiație de fond"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptiv rapid"
#define STRING_ADAPTIVE_PRECISION "Adaptiv cu precizie"

// Average menu items
#define STRING_UNLIMITED "Nelimitat"
#define STRING_50_CONFIDENCE "±50% încredere"
#define STRING_20_CONFIDENCE "±20% încredere"
#define STRING_10_CONFIDENCE "±10% încredere"
#define STRING_5_CONFIDENCE "±5% încredere"
#define STRING_2_CONFIDENCE "±2% încredere"
#define STRING_1_CONFIDENCE "±1% încredere"

// Geiger tube menu items
#define STRING_TUBE_TYPE "Tipul tubului"
#define STRING_SENSITIVITY "Sensibilitate (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Comp. timp mort"
#define STRING_HVPROFILE "Profil HV"

// Geiger tube type strings
#define STRING_J305 "J305 (107 mm)"
#define STRING_M4011 "M4011 (87 mm)"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Tub implicit"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Setare fabrică"
#define STRING_ACCURACY "Acuratețe"
#define STRING_ENERGY_SAVING "Economisire energie"
#define STRING_CUSTOM "Personalizat"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "Frecvență PWM"
#define STRING_PWMDUTY_CYCLE "Ciclu de lucru PWM"

// Data log menu items
#define STRING_LOGGING_MODE "Mod înregistrare"
#define STRING_RESET "Resetare"

// Data log options menu items
#define STRING_EVERY_HOUR "Fiecare oră"
#define STRING_EVERY10_MINUTES "Fiecare 10 minute"
#define STRING_EVERY_MINUTE "Fiecare minut"
#define STRING_EVERY10_SECONDS "Fiecare 10 secunde"
#define STRING_EVERY_SECOND "Fiecare secundă"

// Display menu items
#define STRING_THEME "Temă"
#define STRING_BRIGHTNESS "Luminozitate"
#define STRING_CONTRAST "Contrast"
#define STRING_SLEEP "Repaus"

// Display theme menu items
#define STRING_THEME_DAY "Zi"
#define STRING_THEME_DUSK "Amurg"
#define STRING_THEME_NIGHT "Noapte"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nivel"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Întotdeauna oprit"
#define STRING_ALWAYS_ON "Întotdeauna pornit"

// Sound menu items
#define STRING_ALERTSTYLE "Stil de alertă"
#define STRING_ALERTVOLUME "Volum alertă"
#define STRING_VOICEVOLUME "Volum voce"

// Sound alert style menu items
#define STRING_SHORT "Scurt"
#define STRING_LONG "Lung"

// Sound pulses menu items
#define STRING_CLICKS "Clicuri"
#define STRING_CHIRPS "Ciripituri"
#define STRING_BEEPS "Bipuri"

// Time and date menu items
#define STRING_TIME_ZONE "Fus orar"
#define STRING_YEAR "An"
#define STRING_MONTH "Lună"
#define STRING_DAY "Zi"
#define STRING_HOUR "Oră"
#define STRING_MINUTE "Minut"
#define STRING_TIME_FORMAT "Format oră"

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24 ore"
#define STRING_12_HOUR "12 ore"

// Power menu items
#define STRING_BATTERY "Baterie"
#define STRING_USB_AUTO_POWER_ON "Pornire automată USB"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alcalină"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumeric"
#define STRING_HEXADECIMAL "Hexazecimal"
#define STRING_DECIMAL "Zecimal"
#define STRING_BINARY "Binar"
#define STRING_100_SIDED_DIE "Zar 100 fețe (0-99)"
#define STRING_20_SIDED_DIE "Zar 20 fețe"
#define STRING_12_SIDED_DIE "Zar 12 fețe"
#define STRING_10_SIDED_DIE "Zar 10 fețe (0-9)"
#define STRING_8_SIDED_DIE "Zar 8 fețe"
#define STRING_6_SIDED_DIE "Zar 6 fețe"
#define STRING_4_SIDED_DIE "Zar 4 fețe"
#define STRING_COIN_FLIP "Aruncare monedă (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Joacă alb"
#define STRING_PLAY_BLACK "Joacă negru"
#define STRING_CONTINUE_GAME "Continuă joc"
#define STRING_NEW_GAME "Joc nou"
#define STRING_STRENGTH "Dificultate"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Nivel"

// Statistics view items
#define STRING_TUBE_LIFE "Viața tubului"
#define STRING_TUBE "Tub"
#define STRING_LIFE "Durată"
#define STRING_DEAD_TIME "Timp mort"
#define STRING_DEVICE_ID "ID dispozitiv"
#define STRING_DEVICE "Dispozitiv"
#define STRING_ID "ID"
