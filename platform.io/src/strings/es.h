/*
 * Rad Pro
 * UI strings: Spanish
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
#define STRING_OFF "Apagado"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Muy bajo"
#define STRING_LOW "Bajo"
#define STRING_MEDIUM "Medio"
#define STRING_HIGH "Alto"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ADVERTENCIA\n\nFallo en la suma de verificación del firmware."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "ADVERTENCIA\n\nValores incorrectos pueden dañar el dispositivo."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "¿Restablecer el registro de datos?\n\nTodos los registros serán eliminados."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "ÉXITO\n\nEl registro de datos ha sido restablecido."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Instantáneo"
#define STRING_AVERAGE "Promedio"
#define STRING_CUMULATIVE "Acumulado"
#define STRING_HISTORY "Historial"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 sem"
#define STRING_HISTORY_1_MONTH "1 mes"
#define STRING_HISTORY_1_YEAR "1 a"

// Measurement view secondary view strings
#define STRING_TIME "Tiempo"
#define STRING_MAX "Máx"
#define STRING_RATE "Tasa"
#define STRING_DOSE "Dosis"

// Measurement view info strings
#define STRING_ALERT_ALARM "ALARMA"
#define STRING_ALERT_WARNING "ADVERTENCIA"
#define STRING_ALERT_FAULT "FALLO"
#define STRING_ALERT_MAX "MÁX"
#define STRING_ALERT_DONE "LISTO"

// Settings menu items
#define STRING_SETTINGS "Configuración"
#define STRING_PULSES "Pulsos"
#define STRING_ALERTS "Alertas"
#define STRING_MEASUREMENTS "Mediciones"
#define STRING_GEIGER_TUBE "Tubo Geiger"
#define STRING_DATALOG "Registro de datos"
#define STRING_DISPLAY "Pantalla"
#define STRING_DATE_AND_TIME "Fecha y hora"
#define STRING_BATTERY_TYPE "Tipo de batería"
#define STRING_RANDOM_GENERATOR "Generador aleatorio"
#define STRING_GAME "Juego"
#define STRING_STATISTICS "Estadísticas"
#define STRING_DATAMODE "Modo de datos"

// Pulses menu items
#define STRING_SOUND "Sonido"
#define STRING_VOICE "Voz"
#define STRING_VIBRATION "Vibración"
#define STRING_PULSE_LED "LED de pulso"
#define STRING_DISPLAY_FLASH "Destello de pantalla"
#define STRING_THRESHOLD "Umbral"

// Alarms menu items
#define STRING_RATE_WARNING "Advertencia de tasa"
#define STRING_RATE_ALARM "Alarma de tasa"
#define STRING_DOSE_WARNING "Advertencia de dosis"
#define STRING_DOSE_ALARM "Alarma de dosis"
#define STRING_INDICATION "Indicación"

// Measurements menu items
#define STRING_UNITS "Unidades"
#define STRING_SECONDARY_UNITS "Unidades secundarias"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptativo rápido"
#define STRING_ADAPTIVE_PRECISION "Adaptativo preciso"

// Average menu items
#define STRING_UNLIMITED "Ilimitado"
#define STRING_50_CONFIDENCE "±50% confianza"
#define STRING_20_CONFIDENCE "±20% confianza"
#define STRING_10_CONFIDENCE "±10% confianza"
#define STRING_5_CONFIDENCE "±5% confianza"
#define STRING_2_CONFIDENCE "±2% confianza"
#define STRING_1_CONFIDENCE "±1% confianza"

// Geiger tube menu items
#define STRING_SENSITIVITY "Sensibilidad"
#define STRING_DEAD_TIME_COMPENSATION "Comp. tiempo muerto"
#define STRING_HVPROFILE "Perfil HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (predet.)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Predet. de fábrica"
#define STRING_ACCURACY "Precisión"
#define STRING_ENERGY_SAVING "Ahorro de energía"
#define STRING_CUSTOM "Personalizado"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Frecuencia PWM"
#define STRING_PWMDUTY_CYCLE "Ciclo de trabajo PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Modo de registro"
#define STRING_RESET "Restablecer"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Cada hora"
#define STRING_EVERY10_MINUTES "Cada 10 minutos"
#define STRING_EVERY_MINUTE "Cada minuto"
#define STRING_EVERY10_SECONDS "Cada 10 segundos"
#define STRING_EVERY_SECOND "Cada segundo"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Brillo"
#define STRING_CONTRAST "Contraste"
#define STRING_SLEEP "Apagado automático"

// Display - Theme menu items
#define STRING_THEME_DAY "Día"
#define STRING_THEME_DUSK "Atardecer"
#define STRING_THEME_NIGHT "Noche"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nivel"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Siempre apagado"
#define STRING_ALWAYS_ON "Siempre encendido"

// Sound menu items
#define STRING_ALERTSTYLE "Estilo de alerta"
#define STRING_ALERTVOLUME "Volumen de alerta"
#define STRING_VOICEVOLUME "Volumen de voz"

// Sound Alarms length
#define STRING_SHORT "Corto"
#define STRING_LONG "Largo"

// Sound pulses menu items
#define STRING_CLICKS "Clics"
#define STRING_CHIRPS "Chirps"
#define STRING_BEEPS "Pitidos"

// Time and date menu items
#define STRING_TIME_ZONE "Zona horaria"
#define STRING_YEAR "Año"
#define STRING_MONTH "Mes"
#define STRING_DAY "Día"
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
#define STRING_BINARY "Binario"
#define STRING_100_SIDED_DIE "Dado de 100 caras (0-99)"
#define STRING_20_SIDED_DIE "Dado de 20 caras"
#define STRING_12_SIDED_DIE "Dado de 12 caras"
#define STRING_10_SIDED_DIE "Dado de 10 caras (0-9)"
#define STRING_8_SIDED_DIE "Dado de 8 caras"
#define STRING_6_SIDED_DIE "Dado de 6 caras"
#define STRING_4_SIDED_DIE "Dado de 4 caras"
#define STRING_COIN_FLIP "Lanzar moneda (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Jugar blancas"
#define STRING_PLAY_BLACK "Jugar negras"
#define STRING_CONTINUE_GAME "Continuar juego"
#define STRING_NEW_GAME "Juego nuevo"
#define STRING_STRENGTH "Dificultad"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Nivel"

// Statistics view items
#define STRING_TUBE_LIFE "Vida del tubo"
#define STRING_TUBE "Tubo"
#define STRING_LIFE "Vida"
#define STRING_DEAD_TIME "T. muerto"
#define STRING_DEVICE_ID "ID del dispositivo"
#define STRING_DEVICE "Dispositivo"
#define STRING_ID "ID"
#define STRING_BATTERY "Batería"
