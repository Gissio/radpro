/*
 * Rad Pro
 * UI strings: Ukrainian
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
#define STRING_COUNT "имп."
#define STRING_COUNTS "имп."

// Common time menu items
#define STRING_24_HOURS "24 години"
#define STRING_12_HOURS "12 годин"
#define STRING_6_HOURS "6 годин"
#define STRING_3_HOURS "3 години"
#define STRING_1_HOUR "1 година"
#define STRING_30_MINUTES "30 хвилин"
#define STRING_10_MINUTES "10 хвилин"
#define STRING_5_MINUTES "5 хвилин"
#define STRING_2_MINUTES "2 хвилини"
#define STRING_1_MINUTE "1 хвилина"
#define STRING_30_SECONDS "30 секунд"
#define STRING_10_SECONDS "10 секунд"
#define STRING_5_SECONDS "5 секунд"
#define STRING_1_SECOND "1 секунда"

// Common menu items
#define STRING_OFF "Вимкнено"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Дуже низька"
#define STRING_LOW "Низька"
#define STRING_MEDIUM "Середня"
#define STRING_HIGH "Висока"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ПОПЕРЕДЖЕННЯ\n\nПомилка контрольної суми прошивки."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "ПОПЕРЕДЖЕННЯ\n\nНеправильні значення можуть пошкодити пристрій."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Скинути журнал даних?\n\nУсі записи буде видалено."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "УСПІХ\n\nЖурнал даних скинуто."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Миттєвий"
#define STRING_AVERAGE "Середній"
#define STRING_CUMULATIVE "Накопичувальний"
#define STRING_HISTORY "Історія"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 хв"
#define STRING_HISTORY_1_HOUR "1 год"
#define STRING_HISTORY_1_DAY "1 д"
#define STRING_HISTORY_1_WEEK "1 тиж"
#define STRING_HISTORY_1_MONTH "1 міс"
#define STRING_HISTORY_1_YEAR "1 р"

// Measurement view secondary view strings
#define STRING_TIME "Час"
#define STRING_MAX "Макс"
#define STRING_RATE "Швидкість"
#define STRING_DOSE "Доза"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_ALERT_ALARM "ТРИВОГА"
#define STRING_ALERT_WARNING "ПОПЕРЕДЖЕННЯ"
#define STRING_ALERT_FAULT "ПОМИЛКА"
#define STRING_ALERT_MAX "МАКС"
#define STRING_ALERT_OVERFLOW "ПЕРЕВИЩЕННЯ"
#define STRING_ALERT_DONE "ЗАВЕРШЕНО"

// Settings menu items
#define STRING_SETTINGS "Налаштування"
#define STRING_PULSES "Імпульси"
#define STRING_ALERTS "Сповіщення"
#define STRING_MEASUREMENTS "Вимірювання"
#define STRING_GEIGER_TUBE "Лічильник Гейгера"
#define STRING_DATALOG "Журнал даних"
#define STRING_DISPLAY "Дисплей"
#define STRING_DATE_AND_TIME "Дата і час"
#define STRING_BATTERY_TYPE "Тип батареї"
#define STRING_RANDOM_GENERATOR "Генератор випадкових чисел"
#define STRING_GAME "Гра"
#define STRING_STATISTICS "Статистика"
#define STRING_DATAMODE "Режим даних"

// Pulses menu items
#define STRING_SOUND "Звук"
#define STRING_VOICE "Голос"
#define STRING_VIBRATION "Вібрація"
#define STRING_PULSE_LED "Світлодіод імпульсу"
#define STRING_DISPLAY_FLASH "Спалах дисплея"
#define STRING_THRESHOLD "Поріг"

// Alarms menu items
#define STRING_RATE_WARNING "Попер. швидкості"
#define STRING_RATE_ALARM "Сигнал швидкості"
#define STRING_DOSE_WARNING "Попер. дози"
#define STRING_DOSE_ALARM "Сигнал дози"
#define STRING_INDICATION "Індикація"

// Measurements menu items
#define STRING_UNITS "Одиниці"
#define STRING_SECONDARY_UNITS "Вторинні одиниці"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Адаптивний швидкий"
#define STRING_ADAPTIVE_PRECISION "Адаптивна точність"

// Average menu items
#define STRING_UNLIMITED "Необмежено"
#define STRING_50_CONFIDENCE "±50% довіра"
#define STRING_20_CONFIDENCE "±20% довіра"
#define STRING_10_CONFIDENCE "±10% довіра"
#define STRING_5_CONFIDENCE "±5% довіра"
#define STRING_2_CONFIDENCE "±2% довіра"
#define STRING_1_CONFIDENCE "±1% довіра"

// Geiger tube menu items
#define STRING_SENSITIVITY "Чутливість"
#define STRING_DEAD_TIME_COMPENSATION "Компенсація мертвого часу"
#define STRING_HVPROFILE "HV профіль"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (за замовч.)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "СБМ-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Заводські налаштування"
#define STRING_ACCURACY "Точність"
#define STRING_ENERGY_SAVING "Енергозбереження"
#define STRING_CUSTOM "Користувацький"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Частота ШІМ"
#define STRING_PWMDUTY_CYCLE "Робочий цикл ШІМ"

// Data Log menu items
#define STRING_LOGGING_MODE "Режим журналювання"
#define STRING_RESET "Скинути"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Кожну годину"
#define STRING_EVERY10_MINUTES "Кожні 10 хвилин"
#define STRING_EVERY_MINUTE "Кожну хвилину"
#define STRING_EVERY10_SECONDS "Кожні 10 секунд"
#define STRING_EVERY_SECOND "Кожну секунду"

// Display menu items
#define STRING_THEME "Тема"
#define STRING_BRIGHTNESS "Яскравість"
#define STRING_CONTRAST "Контрастність"
#define STRING_SLEEP "Сплячий режим"

// Display - Theme menu items
#define STRING_THEME_DAY "День"
#define STRING_THEME_DUSK "Сутінки"
#define STRING_THEME_NIGHT "Ніч"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Рівень"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Завжди вимкнено"
#define STRING_ALWAYS_ON "Завжди увімкнено"

// Sound menu items
#define STRING_ALERTSTYLE "Стиль сповіщення"
#define STRING_ALERTVOLUME "Гучність сповіщення"
#define STRING_VOICEVOLUME "Гучність голосу"

// Sound Alarms length
#define STRING_SHORT "Короткий"
#define STRING_LONG "Довгий"

// Sound pulses menu items
#define STRING_CLICKS "Клацання"
#define STRING_CHIRPS "Щебетання"
#define STRING_BEEPS "Гудки"

// Time and date menu items
#define STRING_TIME_ZONE "Часовий пояс"
#define STRING_YEAR "Рік"
#define STRING_MONTH "Місяць"
#define STRING_DAY "День"
#define STRING_HOUR "Година"
#define STRING_MINUTE "Хвилина"
#define STRING_TIME_FORMAT "Формат часу"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-годинний"
#define STRING_12_HOUR "12-годинний"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Лужна"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Буквено-цифровий"
#define STRING_HEXADECIMAL "Шістнадцятковий"
#define STRING_DECIMAL "Десятковий"
#define STRING_BINARY "Двійковий"
#define STRING_100_SIDED_DIE "100-гранний кубик (0-99)"
#define STRING_20_SIDED_DIE "20-гранний кубик"
#define STRING_12_SIDED_DIE "12-гранний кубик"
#define STRING_10_SIDED_DIE "10-гранний кубик (0-9)"
#define STRING_8_SIDED_DIE "8-гранний кубик"
#define STRING_6_SIDED_DIE "6-гранний кубик"
#define STRING_4_SIDED_DIE "4-гранний кубик"
#define STRING_COIN_FLIP "Підкидання монети (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Грати білими"
#define STRING_PLAY_BLACK "Грати чорними"
#define STRING_CONTINUE_GAME "Продовжити гру"
#define STRING_NEW_GAME "Нова гра"
#define STRING_STRENGTH "Сила"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Рівень"

// Statistics view items
#define STRING_TUBE_LIFE "Робота дет."
#define STRING_DEAD_TIME "Мертвий час"

#define STRING_TUBE "Детектор"
#define STRING_LIFE "Життя"
#define STRING_DEVICE "Пристрій"
#define STRING_ID "ID"
#define STRING_BATTERY "Батарея"
