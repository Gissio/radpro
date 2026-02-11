/*
 * Rad Pro
 * UI strings: Italian
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

// Strength strings
#define STRING_OFF "Spento"
#define STRING_VERY_LOW "Molto bassa"
#define STRING_LOW "Bassa"
#define STRING_MEDIUM "Media"
#define STRING_HIGH "Alta"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ATTENZIONE\n\nErrore di checksum del firmware."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "ATTENZIONE\n\nValori errati possono danneggiare il dispositivo."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Ripristinare il registro dati?\n\nTutti i record saranno eliminati."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCCESSO\n\nIl registro dati è stato ripristinato."

// Measurements title strings
#define STRING_INSTANTANEOUS "Istantaneo"
#define STRING_AVERAGE "Medio"
#define STRING_CUMULATIVE "Cumulativo"
#define STRING_HISTORY "Storico"
#define STRING_ELECTRIC_FIELD "Campo elettrico"
#define STRING_MAGNETIC_FIELD "Campo magnetico"

// Measurements secondary view strings
#define STRING_TIME "Tempo"
#define STRING_MAX "Max"
#define STRING_RATE "Tasso"
#define STRING_DOSE "Dose"

// Measurements info strings
#define STRING_ALERT_FAULT "GUASTO"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_DONE "COMPLETATO"
#define STRING_ALERT_UNAVAILABLE "NON DISPONIBILE"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 g"
#define STRING_HISTORY_1_WEEK "1 sett"
#define STRING_HISTORY_1_MONTH "1 m"
#define STRING_HISTORY_1_YEAR "1 a"

// Settings menu items
#define STRING_SETTINGS "Impostazioni"
#define STRING_PULSES "Impulsi"
#define STRING_ALERTS "Avvisi"
#define STRING_MEASUREMENTS "Misure"
#define STRING_GEIGER_TUBE "Tubo Geiger"
#define STRING_DATALOG "Registro dati"
#define STRING_DISPLAY "Display"
#define STRING_DATE_AND_TIME "Data e ora"
#define STRING_POWER "Alimentazione"
#define STRING_RANDOM_GENERATOR "Generatore casuale"
#define STRING_GAME "Gioco"
#define STRING_STATISTICS "Statistiche"
#define STRING_DATAMODE "Modalità dati"

// Pulses menu items
#define STRING_SOUND "Suono"
#define STRING_VOICE "Voce"
#define STRING_VIBRATION "Vibrazione"
#define STRING_PULSE_LED "LED impulso"
#define STRING_DISPLAY_FLASH "Lampeggio display"
#define STRING_THRESHOLD "Soglia"

// Alerts menu items
#define STRING_INDICATION "Indicazione"
#define STRING_RATE_WARNING "Avvert. tasso"
#define STRING_RATE_ALARM "Allarme tasso"
#define STRING_DOSE_WARNING "Avvert. dose"
#define STRING_DOSE_ALARM "Allarme dose"
#define STRING_ELECTRIC_FIELD_ALARM "Allarme campo elettrico"
#define STRING_MAGNETIC_FIELD_ALARM "Allarme campo magnetico"

// Measurements menu items
#define STRING_SOURCE "Sorgente"
#define STRING_DOSE_UNITS "Unità di dose"
#define STRING_SECONDARY_DOSE_UNITS "Unità di dose secondarie"
#define STRING_MAGNETIC_FIELD_UNITS "Unità di campo magnetico"

// Source menu items
#define STRING_CS137 "Predefinito: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Radio (γ)"
#define STRING_URANIUM_ORE "Minerale di uranio (γ)"
#define STRING_URANIUM_GLASS "Vetro all'uranio (γ)"
#define STRING_DEPLETED_URANIUM "Uranio impoverito (γ)"
#define STRING_THORIUM_ORE "Minerale di torio (γ)"
#define STRING_XRAYS "Raggi X (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Radiazione di fondo"

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
#define STRING_TUBE_TYPE "Tipo di tubo"
#define STRING_SENSITIVITY "Sensibilità (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Comp. tempo morto"
#define STRING_HVPROFILE "Profilo alta tensione"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011/J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Tubo predefinito"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Valore di fabbrica"
#define STRING_ACCURACY "Precisione"
#define STRING_ENERGY_SAVING "Risparmio energetico"
#define STRING_CUSTOM "Personalizzato"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "Frequenza PWM"
#define STRING_PWMDUTY_CYCLE "Duty cycle PWM"

// Data log menu items
#define STRING_LOGGING_MODE "Modalità registrazione"
#define STRING_RESET "Ripristina"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "Giorno"
#define STRING_THEME_DUSK "Crepuscolo"
#define STRING_THEME_NIGHT "Notte"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Livello"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Sempre spento"
#define STRING_ALWAYS_ON "Sempre acceso"

// Sound menu items
#define STRING_ALERTSTYLE "Stile avviso"
#define STRING_ALERTVOLUME "Volume avviso"
#define STRING_VOICEVOLUME "Volume voce"

// Sound alert style menu items
#define STRING_SHORT "Corto"
#define STRING_LONG "Lungo"

// Sound pulses menu items
#define STRING_CLICKS "Clic"
#define STRING_CHIRPS "Chirp"
#define STRING_BEEPS "Bip"

// Time and date menu items
#define STRING_TIME_ZONE "Fuso orario"
#define STRING_YEAR "Anno"
#define STRING_MONTH "Mese"
#define STRING_DAY "Giorno"
#define STRING_HOUR "Ora"
#define STRING_MINUTE "Minuto"
#define STRING_TIME_FORMAT "Formato ora"

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
#define STRING_BATTERY "Batteria"
#define STRING_USB_AUTO_POWER_ON "Accensione automatica USB"

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
#define STRING_COIN_FLIP "Lancio moneta (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Gioca bianco"
#define STRING_PLAY_BLACK "Gioca nero"
#define STRING_CONTINUE_GAME "Continua partita"
#define STRING_NEW_GAME "Nuova partita"
#define STRING_STRENGTH "Difficoltà"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Livello"

// Statistics view items
#define STRING_TUBE_LIFE "Durata tubo"
#define STRING_TUBE "Tubo"
#define STRING_LIFE "Durata"
#define STRING_DEAD_TIME "Tempo morto"
#define STRING_DEVICE_ID "ID dispositivo"
#define STRING_DEVICE "Dispositivo"
#define STRING_ID "ID"
