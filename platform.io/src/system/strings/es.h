/*
 * Rad Pro
 * UI strings: Spanish
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
#define STRING_OFF "Apagado"
#define STRING_VERY_LOW "Muy bajo"
#define STRING_LOW "Bajo"
#define STRING_MEDIUM "Medio"
#define STRING_HIGH "Alto"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ADVERTENCIA\n\nFallo en la suma de verificación del firmware."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "ADVERTENCIA\n\nValores incorrectos pueden dañar el dispositivo."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "¿Restablecer el registro de datos?\n\nTodos los registros serán eliminados."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "ÉXITO\n\nEl registro de datos ha sido restablecido."

// Measurements title strings
#define STRING_INSTANTANEOUS "Instantáneo"
#define STRING_AVERAGE "Promedio"
#define STRING_CUMULATIVE "Acumulado"
#define STRING_HISTORY "Historial"
#define STRING_ELECTRIC_FIELD "Campo eléctrico"
#define STRING_MAGNETIC_FIELD "Campo magnético"

// Measurements secondary view strings
#define STRING_TIME "Tiempo"
#define STRING_MAX "Máx"
#define STRING_RATE "Tasa"
#define STRING_DOSE "Dosis"
#define STRING_POWER_DENSITY "Densidad de potencia"

// Measurements info strings
#define STRING_ALERT_FAULT "FALLO"
#define STRING_ALERT_MAX "MÁX"
#define STRING_ALERT_DONE "LISTO"
#define STRING_ALERT_UNAVAILABLE "NO DISPONIBLE"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 sem"
#define STRING_HISTORY_1_MONTH "1 mes"
#define STRING_HISTORY_1_YEAR "1 a"

// Settings menu items
#define STRING_SETTINGS "Configuración"
#define STRING_PULSES "Pulsos"
#define STRING_ALERTS "Alertas"
#define STRING_MEASUREMENTS "Mediciones"
#define STRING_GEIGER_TUBE "Tubo Geiger"
#define STRING_DATALOG "Registro de datos"
#define STRING_DISPLAY "Pantalla"
#define STRING_DATE_AND_TIME "Fecha y hora"
#define STRING_POWER "Energía"
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

// Alerts menu items
#define STRING_INDICATION "Indicación"
#define STRING_RATE_WARNING "Advertencia de tasa"
#define STRING_RATE_ALARM "Alarma de tasa"
#define STRING_DOSE_WARNING "Advertencia de dosis"
#define STRING_DOSE_ALARM "Alarma de dosis"
#define STRING_ELECTRIC_FIELD_ALARM "Alarma de campo eléctrico"
#define STRING_MAGNETIC_FIELD_ALARM "Alarma de campo magnético"

// Measurements menu items
#define STRING_SOURCE "Fuente"
#define STRING_DOSE_UNITS "Unidades de dosis"
#define STRING_SECONDARY_DOSE_UNITS "Unidades de dosis secundarias"
#define STRING_MAGNETIC_FIELD_UNITS "Unidades de campo magnético"

// Source menu items
#define STRING_CS137 "Predeterminado: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Radio (γ)"
#define STRING_URANIUM_ORE "Mineral de uranio (γ)"
#define STRING_URANIUM_GLASS "Vidrio de uranio (γ)"
#define STRING_DEPLETED_URANIUM "Uranio empobrecido (γ)"
#define STRING_THORIUM_ORE "Mineral de torio (γ)"
#define STRING_XRAYS "Rayos X (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Radiación de fondo"

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
#define STRING_TUBE_TYPE "Tipo de tubo"
#define STRING_SENSITIVITY "Sensibilidad (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Comp. tiempo muerto"
#define STRING_HVPROFILE "Perfil HV"

// Geiger tube type strings
#define STRING_J305 "J305 (107 mm)"
#define STRING_M4011 "M4011 (87 mm)"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Predet. del tubo"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Predet. de fábrica"
#define STRING_ACCURACY "Precisión"
#define STRING_ENERGY_SAVING "Ahorro de energía"
#define STRING_CUSTOM "Personalizado"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "Frecuencia PWM"
#define STRING_PWMDUTY_CYCLE "Ciclo de trabajo PWM"

// Data log menu items
#define STRING_LOGGING_MODE "Modo de registro"
#define STRING_RESET "Restablecer"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "Día"
#define STRING_THEME_DUSK "Atardecer"
#define STRING_THEME_NIGHT "Noche"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Nivel"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Siempre apagado"
#define STRING_ALWAYS_ON "Siempre encendido"

// Sound menu items
#define STRING_ALERTSTYLE "Estilo de alerta"
#define STRING_ALERTVOLUME "Volumen de alerta"
#define STRING_VOICEVOLUME "Volumen de voz"

// Sound alert style menu items
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
#define STRING_BATTERY "Batería"
#define STRING_USB_AUTO_POWER_ON "Encendido automático USB"

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
