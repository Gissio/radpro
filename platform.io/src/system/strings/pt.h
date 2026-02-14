/*
 * Rad Pro
 * UI strings: Portuguese
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

// Application name
#define STRING_APP_NAME "Rad Pro"

// Styling characters
#define STRING_EMPTY ""
#define STRING_NUMBERS "0123456789.:‒"
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
#define STRING_COUNT "pulso"
#define STRING_COUNTS "pulsos"

// Time strings
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

// Strength strings
#define STRING_OFF "Desligado"
#define STRING_VERY_LOW "Muito baixo"
#define STRING_LOW "Baixo"
#define STRING_MEDIUM "Médio"
#define STRING_HIGH "Alto"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "AVISO\n\nFalha na soma de verificação do firmware."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "AVISO\n\nValores incorretos podem danificar o dispositivo."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Redefinir o registro de dados?\n\nTodos os registros serão excluídos."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCESSO\n\nO registro de dados foi redefinido."

// Measurements title strings
#define STRING_INSTANTANEOUS "Instantâneo"
#define STRING_AVERAGE "Média"
#define STRING_CUMULATIVE "Cumulativo"
#define STRING_HISTORY "Histórico"
#define STRING_ELECTRIC_FIELD "Campo elétrico"
#define STRING_MAGNETIC_FIELD "Campo magnético"

// Measurements secondary view strings
#define STRING_TIME "Tempo"
#define STRING_MAX "Máx"
#define STRING_RATE "Taxa"
#define STRING_DOSE "Dose"

// Measurements info strings
#define STRING_ALERT_FAULT "FALHA"
#define STRING_ALERT_DONE "CONCLUÍDO"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 sem"
#define STRING_HISTORY_1_MONTH "1 mês"
#define STRING_HISTORY_1_YEAR "1 a"

// Settings menu items
#define STRING_SETTINGS "Configurações"
#define STRING_PULSES "Pulsos"
#define STRING_ALERTS "Alertas"
#define STRING_MEASUREMENTS "Medições"
#define STRING_GEIGER_TUBE "Tubo Geiger"
#define STRING_DATALOG "Registro de dados"
#define STRING_DISPLAY "Tela"
#define STRING_DATE_AND_TIME "Data e hora"
#define STRING_POWER "Energia"
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

// Alerts menu items
#define STRING_INDICATION "Indicação"
#define STRING_RATE_WARNING "Aviso de taxa"
#define STRING_RATE_ALARM "Alarme de taxa"
#define STRING_DOSE_WARNING "Aviso de dose"
#define STRING_DOSE_ALARM "Alarme de dose"
#define STRING_ELECTRIC_FIELD_ALARM "Alarme de campo elétrico"
#define STRING_MAGNETIC_FIELD_ALARM "Alarme de campo magnético"

// Measurements menu items
#define STRING_SOURCE "Fonte"
#define STRING_DOSE_UNITS "Unidades de dose"
#define STRING_SECONDARY_DOSE_UNITS "Unidades de dose secundárias"
#define STRING_MAGNETIC_FIELD_UNITS "Unidades de campo magnético"

// Source menu items
#define STRING_CS137 "Padrão: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Rádio (γ)"
#define STRING_URANIUM_ORE "Minério de urânio (γ)"
#define STRING_URANIUM_GLASS "Vidro de urânio (γ)"
#define STRING_DEPLETED_URANIUM "Urânio empobrecido (γ)"
#define STRING_THORIUM_ORE "Minério de tório (γ)"
#define STRING_XRAYS "Raios X (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Radiação de fundo"

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
#define STRING_TUBE_TYPE "Tipo de tubo"
#define STRING_SENSITIVITY "Sensibilidade (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Comp. tempo morto"
#define STRING_HVPROFILE "Perfil HV"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011/J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Tubo padrão"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Padrão de fábrica"
#define STRING_ACCURACY "Precisão"
#define STRING_ENERGY_SAVING "Economia de energia"
#define STRING_CUSTOM "Personalizado"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "Frequência PWM"
#define STRING_PWMDUTY_CYCLE "Ciclo de trabalho PWM"

// Data log menu items
#define STRING_LOGGING_MODE "Modo de registro"
#define STRING_RESET "Redefinir"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "Dia"
#define STRING_THEME_DUSK "Crepúsculo"
#define STRING_THEME_NIGHT "Noite"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nível"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Sempre desligado"
#define STRING_ALWAYS_ON "Sempre ligado"

// Sound menu items
#define STRING_ALERTSTYLE "Estilo de alerta"
#define STRING_ALERTVOLUME "Volume do alerta"
#define STRING_VOICEVOLUME "Volume da voz"

// Sound alert style menu items
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

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24 horas"
#define STRING_12_HOUR "12 horas"

// Power menu items
#define STRING_BATTERY "Bateria"
#define STRING_USB_AUTO_POWER_ON "Ligação automática USB"

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
