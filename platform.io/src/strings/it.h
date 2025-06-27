/*
 * Rad Pro
 * UI strings: Italian
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
#define STRING_24_HOURS "24 ore"
#define STRING_12_HOURS "12 ore"
#define STRING_6_HOURS "6 ore"
#define STRING_3_HOURS "3 ore"
#define STRING_1_HOUR "1 ora"
#define STRING_30_MINUTES "30 minuti"
#define STRING_10_MINUTES "10 minuti"
#define STRING_5_MINUTES "5 minuti"
#define STRING_2_MINUTES "2 minuti"
#define STRING_1_MINUTE "1 minuto"
#define STRING_30_SECONDS "30 secondi"
#define STRING_10_SECONDS "10 secondi"
#define STRING_5_SECONDS "5 secondi"
#define STRING_1_SECOND "1 secondo"

// Common menu items
#define STRING_OFF "Spento"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "AVVISO"
#define STRING_NOTIFICATION_SUCCESS "SUCCESSO"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Errore checksum firmware."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Valori errati danneggeranno."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Reimpostare?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Tutti i dati saranno eliminati."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Log dati reimpostato."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Istantaneo"
#define STRING_AVERAGE "Medio"
#define STRING_CUMULATIVE "Cumulativo"
#define STRING_HISTORY10_MIN "Storico (10 min)"
#define STRING_HISTORY1_H "Storico (1 h)"
#define STRING_HISTORY24_H "Storico (24 h)"

// Measurement view secondary view strings
#define STRING_TIME "Tempo"
#define STRING_MAX "Max"
#define STRING_RATE "Tasso"
#define STRING_DOSE "Dose"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ALLARME"
#define STRING_INFO_FAULT "GUASTO"
#define STRING_INFO_MAX "MAX"
#define STRING_INFO_OVER "SOVR."
#define STRING_INFO_DONE "PRONTO"

// Settings menu items
#define STRING_SETTINGS "Impostazioni"
#define STRING_PULSES "Impulsi"
#define STRING_ALARMS "Allarmi"
#define STRING_MEASUREMENTS "Misure"
#define STRING_GEIGER_TUBE "Tubo Geiger"
#define STRING_DATA_LOG "Log dati"
#define STRING_DISPLAY "Display"
#define STRING_DATE_AND_TIME "Data e ora"
#define STRING_BATTERY_TYPE "Tipo di batteria"
#define STRING_RANDOM_GENERATOR "Generatore casuale"
#define STRING_GAME "Gioco"
#define STRING_STATISTICS "Statistiche"
#define STRING_DATA_MODE "Modalità dati"

// Pulses menu items
#define STRING_SOUND "Suono"
#define STRING_VIBRATION "Vibrazione"
#define STRING_PULSE_LED "LED impulso"
#define STRING_DISPLAY_FLASH "Lampeggio display"
#define STRING_THRESHOLD "Soglia"

// Sound menu items
#define STRING_CLICKS "Clic"
#define STRING_CHIRPS "Chirp"
#define STRING_BEEPS "Bip"

// Alarms menu items
#define STRING_RATE_ALARM "Allarme tasso"
#define STRING_DOSE_ALARM "Allarme dose"
#define STRING_INDICATION "Indicazione"
#define STRING_VOICE "Voce"
#define STRING_VOLUME "Volume"

// Measurements menu items
#define STRING_UNITS "Unità"
#define STRING_SECONDARY_UNITS "Unità secondarie"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adattivo veloce"
#define STRING_ADAPTIVE_PRECISION "Adattivo preciso"

// Average menu items
#define STRING_UNLIMITED "Illimitato"
#define STRING_50_CONFIDENCE "±50% confidenza"
#define STRING_20_CONFIDENCE "±20% confidenza"
#define STRING_10_CONFIDENCE "±10% confidenza"
#define STRING_5_CONFIDENCE "±5% confidenza"
#define STRING_2_CONFIDENCE "±2% confidenza"
#define STRING_1_CONFIDENCE "±1% confidenza"

// Geiger tube menu items
#define STRING_SENSITIVITY "Sensibilità"
#define STRING_DEAD_TIME_SHORT_COMPENSATION "Comp. tempo morto"
#define STRING_HVPROFILE "Profilo alta tensione"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (predef.)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Valore di fabbrica"
#define STRING_ACCURACY "Precisione"
#define STRING_ENERGY_SAVING "Risparmio energetico"
#define STRING_CUSTOM "Personalizzato"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Frequenza PWM"
#define STRING_PWMDUTY_CYCLE "Duty cycle PWM"

// Tube/battery voltage
#define STRING_VOLTAGE "Tensione"

// Data Log menu items
#define STRING_LOGGING_MODE "Modalità registrazione"
#define STRING_RESET "Reimposta"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Ogni ora"
#define STRING_EVERY10_MINUTES "Ogni 10 minuti"
#define STRING_EVERY_MINUTE "Ogni minuto"
#define STRING_EVERY10_SECONDS "Ogni 10 secondi"
#define STRING_EVERY_SECOND "Ogni secondo"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Luminosità"
#define STRING_CONTRAST "Contrasto"
#define STRING_SLEEP "Sospensione"

// Display - Theme menu items
#define STRING_THEME_DAY "Giorno"
#define STRING_THEME_DUSK "Crepuscolo"
#define STRING_THEME_NIGHT "Notte"

// Display - Brightness menu items
#define STRING_VERY_LOW "Molto bassa"
#define STRING_LOW "Bassa"
#define STRING_MEDIUM "Media"
#define STRING_HIGH "Alta"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Livello"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Sempre spento"
#define STRING_ALWAYS_ON "Sempre acceso"

// Time and date menu items
#define STRING_TIME_ZONE "Fuso orario"
#define STRING_YEAR "Anno"
#define STRING_MONTH "Mese"
#define STRING_DAY "Giorno"
#define STRING_HOUR "Ora"
#define STRING_MINUTE "Minuto"
#define STRING_TIME_FORMAT "Formato ora"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 ore"
#define STRING_12_HOUR "12 ore"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alcalina"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerico"
#define STRING_HEXADECIMAL "Esadecimale"
#define STRING_DECIMAL "Decimale"
#define STRING_BINARY "Binario"
#define STRING_100_SIDED_DIE "Dado a 100 facce (0-99)"
#define STRING_20_SIDED_DIE "Dado a 20 facce"
#define STRING_12_SIDED_DIE "Dado a 12 facce"
#define STRING_10_SIDED_DIE "Dado a 10 facce (0-9)"
#define STRING_8_SIDED_DIE "Dado a 8 facce"
#define STRING_6_SIDED_DIE "Dado a 6 facce"
#define STRING_4_SIDED_DIE "Dado a 4 facce"
#define STRING_COIN_FLIP "Lancio moneta"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Testa"
#define STRING_COIN_TAIL "Croce"

// Game menu items
#define STRING_PLAY_WHITE "Gioca bianco"
#define STRING_PLAY_BLACK "Gioca nero"
#define STRING_CONTINUE_GAME "Continua partita"
#define STRING_NEW_GAME "Nuova partita"
#define STRING_STRENGTH "Difficoltà"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Livello"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Durata tubo"
#define STRING_DEAD_TIME "Tempo morto"
#define STRING_DEVICE_ID "ID dispositivo"

#define STRING_TUBE "Tubo"
#define STRING_LIFE_TIME "Durata"
#define STRING_DEAD_TIME_SHORT "T. morto"
#define STRING_DEVICE "Dispositivo"
#define STRING_ID "ID"
#define STRING_VOLTAGE_SHORT "Tensione"
