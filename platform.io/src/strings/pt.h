/*
 * Rad Pro
 * UI strings: Portuguese
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
#define STRING_COUNT "pulso"
#define STRING_COUNTS "pulsos"

// Common time menu items
#define STRING_24_HOURS "24 horas"
#define STRING_12_HOURS "12 horas"
#define STRING_6_HOURS "6 horas"
#define STRING_3_HOURS "3 horas"
#define STRING_1_HOUR "1 hora"
#define STRING_30_MINUTES "30 minutos"
#define STRING_10_MINUTES "10 minutos"
#define STRING_5_MINUTES "5 minutos"
#define STRING_2_MINUTES "2 minutos"
#define STRING_1_MINUTE "1 minuto"
#define STRING_30_SECONDS "30 segundos"
#define STRING_10_SECONDS "10 segundos"
#define STRING_5_SECONDS "5 segundos"
#define STRING_1_SECOND "1 segundo"

// Common menu items
#define STRING_OFF "Desligado"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Muito baixo"
#define STRING_LOW "Baixo"
#define STRING_MEDIUM "Médio"
#define STRING_HIGH "Alto"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING ""
#define STRING_NOTIFICATION_SUCCESS "SUCESSO"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "AVISO\n\nFalha na soma de verificação do firmware."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "AVISO\n\nValores incorretos podem danificar o dispositivo."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Redefinir o registro de dados?\n\nTodos os registros serão excluídos."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCESSO\n\nO registro de dados foi redefinido."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Instantâneo"
#define STRING_AVERAGE "Média"
#define STRING_CUMULATIVE "Cumulativo"
#define STRING_HISTORY "Histórico"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 sem"
#define STRING_HISTORY_1_MONTH "1 mês"
#define STRING_HISTORY_1_YEAR "1 a"

// Measurement view secondary view strings
#define STRING_TIME "Tempo"
#define STRING_MAX "Máx"
#define STRING_RATE "Taxa"
#define STRING_DOSE "Dose"

// Measurement view info strings
#define STRING_ALERT_ALARM "ALARME"
#define STRING_ALERT_WARNING "AVISO"
#define STRING_ALERT_FAULT "FALHA"
#define STRING_ALERT_MAX "MÁX"
#define STRING_ALERT_DONE "CONCLUÍDO"

// Settings menu items
#define STRING_SETTINGS "Configurações"
#define STRING_PULSES "Pulsos"
#define STRING_ALERTS "Alertas"
#define STRING_MEASUREMENTS "Medições"
#define STRING_GEIGER_TUBE "Tubo Geiger"
#define STRING_DATALOG "Registro de dados"
#define STRING_DISPLAY "Tela"
#define STRING_DATE_AND_TIME "Data e hora"
#define STRING_BATTERY_TYPE "Tipo de bateria"
#define STRING_RANDOM_GENERATOR "Gerador aleatório"
#define STRING_GAME "Jogo"
#define STRING_STATISTICS "Estatísticas"
#define STRING_DATAMODE "Modo de dados"

// Pulses menu items
#define STRING_SOUND "Som"
#define STRING_VOICE "Voz"
#define STRING_VIBRATION "Vibração"
#define STRING_PULSE_LED "LED de pulso"
#define STRING_DISPLAY_FLASH "Flash da tela"
#define STRING_THRESHOLD "Limiar"

// Alarms menu items
#define STRING_RATE_WARNING "Aviso de taxa"
#define STRING_RATE_ALARM "Alarme de taxa"
#define STRING_DOSE_WARNING "Aviso de dose"
#define STRING_DOSE_ALARM "Alarme de dose"
#define STRING_INDICATION "Indicação"

// Measurements menu items
#define STRING_UNITS "Unidades"
#define STRING_SECONDARY_UNITS "Unidades secundárias"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptativo rápido"
#define STRING_ADAPTIVE_PRECISION "Adaptativo preciso"

// Average menu items
#define STRING_UNLIMITED "Ilimitado"
#define STRING_50_CONFIDENCE "±50% confiança"
#define STRING_20_CONFIDENCE "±20% confiança"
#define STRING_10_CONFIDENCE "±10% confiança"
#define STRING_5_CONFIDENCE "±5% confiança"
#define STRING_2_CONFIDENCE "±2% confiança"
#define STRING_1_CONFIDENCE "±1% confiança"

// Geiger tube menu items
#define STRING_SENSITIVITY "Sensibilidade"
#define STRING_DEAD_TIME_COMPENSATION "Comp. tempo morto"
#define STRING_HVPROFILE "Perfil HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (padrão)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Padrão de fábrica"
#define STRING_ACCURACY "Precisão"
#define STRING_ENERGY_SAVING "Economia de energia"
#define STRING_CUSTOM "Personalizado"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Frequência PWM"
#define STRING_PWMDUTY_CYCLE "Ciclo de trabalho PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Modo de registro"
#define STRING_RESET "Redefinir"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "A cada hora"
#define STRING_EVERY10_MINUTES "A cada 10 minutos"
#define STRING_EVERY_MINUTE "A cada minuto"
#define STRING_EVERY10_SECONDS "A cada 10 segundos"
#define STRING_EVERY_SECOND "A cada segundo"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Brilho"
#define STRING_CONTRAST "Contraste"
#define STRING_SLEEP "Suspender"

// Display - Theme menu items
#define STRING_THEME_DAY "Dia"
#define STRING_THEME_DUSK "Crepúsculo"
#define STRING_THEME_NIGHT "Noite"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nível"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Sempre desligado"
#define STRING_ALWAYS_ON "Sempre ligado"

// Sound menu items
#define STRING_ALERTSTYLE "Estilo de alerta"
#define STRING_ALERTVOLUME "Volume do alerta"
#define STRING_VOICEVOLUME "Volume da voz"

// Sound Alarms length
#define STRING_SHORT "Curto"
#define STRING_LONG "Longo"

// Sound pulses menu items
#define STRING_CLICKS "Cliques"
#define STRING_CHIRPS "Chirps"
#define STRING_BEEPS "Bipes"

// Time and date menu items
#define STRING_TIME_ZONE "Fuso horário"
#define STRING_YEAR "Ano"
#define STRING_MONTH "Mês"
#define STRING_DAY "Dia"
#define STRING_HOUR "Hora"
#define STRING_MINUTE "Minuto"
#define STRING_TIME_FORMAT "Formato de hora"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 horas"
#define STRING_12_HOUR "12 horas"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alcalina"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumérico"
#define STRING_HEXADECIMAL "Hexadecimal"
#define STRING_DECIMAL "Decimal"
#define STRING_BINARY "Binário"
#define STRING_100_SIDED_DIE "Dado de 100 lados (0-99)"
#define STRING_20_SIDED_DIE "Dado de 20 lados"
#define STRING_12_SIDED_DIE "Dado de 12 lados"
#define STRING_10_SIDED_DIE "Dado de 10 lados (0-9)"
#define STRING_8_SIDED_DIE "Dado de 8 lados"
#define STRING_6_SIDED_DIE "Dado de 6 lados"
#define STRING_4_SIDED_DIE "Dado de 4 lados"
#define STRING_COIN_FLIP "Cara ou coroa (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Jogar com brancas"
#define STRING_PLAY_BLACK "Jogar com pretas"
#define STRING_CONTINUE_GAME "Continuar jogo"
#define STRING_NEW_GAME "Novo jogo"
#define STRING_STRENGTH "Nível"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Nível"

// Statistics view items
#define STRING_TUBE_LIFE "Vida do tubo"
#define STRING_TUBE "Tubo"
#define STRING_LIFE "Vida"
#define STRING_DEAD_TIME "T. morto"
#define STRING_DEVICE_ID "ID do dispositivo"
#define STRING_DEVICE "Dispositivo"
#define STRING_ID "ID"
#define STRING_BATTERY "Bateria"
