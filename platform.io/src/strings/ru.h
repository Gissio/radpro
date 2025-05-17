/*
 * Rad Pro
 * UI strings: Russian
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

// Application name (should only be translated with non-latin scripts)
#define STRING_APP_NAME "Рад Про"

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
#define STRING_COUNT "счёт"
#define STRING_COUNTS "счёты"

// Common time menu items
#define STRING_24_HOURS "24 часа"
#define STRING_12_HOURS "12 часов"
#define STRING_6_HOURS "6 часов"
#define STRING_3_HOURS "3 часа"
#define STRING_1_HOUR "1 час"
#define STRING_30_MINUTES "30 минут"
#define STRING_10_MINUTES "10 минут"
#define STRING_5_MINUTES "5 минут"
#define STRING_2_MINUTES "2 минуты"
#define STRING_1_MINUTE "1 минута"
#define STRING_30_SECONDS "30 секунд"
#define STRING_10_SECONDS "10 секунд"
#define STRING_5_SECONDS "5 секунд"
#define STRING_1_SECOND "1 секунда"

// Common menu items
#define STRING_OFF "Выкл"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "ПРЕДУПРЕЖДЕНИЕ"
#define STRING_NOTIFICATION_SUCCESS "УСПЕХ"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Ошибка суммы прошивки."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Ошибки вредят устройству."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Сброс журнала?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Все записи будут удалены."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Журнал данных сброшен."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Мгновенный"
#define STRING_AVERAGE "Средний"
#define STRING_CUMULATIVE "Накопленный"
#define STRING_HISTORY10_MIN "История (10 мин)"
#define STRING_HISTORY1_H "История (1 ч)"
#define STRING_HISTORY24_H "История (24 ч)"

// Measurement view secondary view strings
#define STRING_TIME "Время"
#define STRING_MAX "Макс"
#define STRING_RATE "Скор."
#define STRING_DOSE "Доза"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ТРЕВОГА"
#define STRING_INFO_FAULT "ОШИБКА"
#define STRING_INFO_MAX "МАКС"
#define STRING_INFO_OVER "ПЕРЕГР"
#define STRING_INFO_DONE "ГОТОВО"

// Settings menu items
#define STRING_SETTINGS "Настройки"
#define STRING_PULSES "Импульсы"
#define STRING_ALARMS "Сигналы"
#define STRING_MEASUREMENTS "Измерения"
#define STRING_GEIGER_TUBE "Счётчик Гейгера"
#define STRING_DATA_LOG "Журнал данных"
#define STRING_DISPLAY "Дисплей"
#define STRING_DATE_AND_TIME "Дата и время"
#define STRING_BATTERY_TYPE "Тип батареи"
#define STRING_RANDOM_GENERATOR "Генератор случайных чисел"
#define STRING_GAME "Игра"
#define STRING_STATISTICS "Статистика"
#define STRING_DATA_MODE "Режим данных"

// Pulses menu items
#define STRING_SOUND "Звук"
#define STRING_VIBRATION "Вибрация"
#define STRING_ALERT_LED "Светодиод тревоги"
#define STRING_PULSE_LED "Светодиод импульса"
#define STRING_DISPLAY_FLASH "Мигание дисплея"
#define STRING_THRESHOLD "Порог"

// Sound menu items
#define STRING_CLICKS "Щелчки"
#define STRING_CHIRPS "Чирпы"
#define STRING_BEEPS "Гудки"

// Alarms menu items
#define STRING_RATE_ALARM "Тревога скорости"
#define STRING_DOSE_ALARM "Тревога дозы"
#define STRING_OVERRANGE_ALARM "Тревога превышения"
#define STRING_INDICATION "Индикация"

// Measurements menu items
#define STRING_UNITS "Единицы"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Адаптивный быстрый"
#define STRING_ADAPTIVE_PRECISION "Адаптивный точный"

// Average menu items
#define STRING_UNLIMITED "Неограниченный"
#define STRING_50_CONFIDENCE "±50% доверие"
#define STRING_20_CONFIDENCE "±20% доверие"
#define STRING_10_CONFIDENCE "±10% доверие"
#define STRING_5_CONFIDENCE "±5% доверие"
#define STRING_2_CONFIDENCE "±2% доверие"
#define STRING_1_CONFIDENCE "±1% доверие"

// Geiger tube menu items
#define STRING_SENSITIVITY "Чувствительность"
#define STRING_DEAD_TIME_COMPENSATION "Компенсация мёртвого времени"
#define STRING_HVPROFILE "Профиль HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (по умолч.)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "СБМ-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Заводской по умолчанию"
#define STRING_ACCURACY "Точность"
#define STRING_ENERGY_SAVING "Энергосбережение"
#define STRING_CUSTOM "Пользовательский"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Частота ШИМ"
#define STRING_PWMDUTY_CYCLE "Скважность ШИМ"

// Data Log menu items
#define STRING_LOGGING_MODE "Режим записи"
#define STRING_RESET "Сброс"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Каждый час"
#define STRING_EVERY10_MINUTES "Каждые 10 минут"
#define STRING_EVERY_MINUTE "Каждую минуту"
#define STRING_EVERY10_SECONDS "Каждые 10 секунд"
#define STRING_EVERY_SECOND "Каждую секунду"

// Display menu items
#define STRING_THEME "Тема"
#define STRING_BRIGHTNESS "Яркость"
#define STRING_CONTRAST "Контраст"
#define STRING_SLEEP "Спящий режим"

// Display - Theme menu items
#define STRING_THEME_DAY "День"
#define STRING_THEME_DUSK "Сумерки"
#define STRING_THEME_NIGHT "Ночь"

// Display - Brightness menu items
#define STRING_VERY_LOW "Очень низкая"
#define STRING_LOW "Низкая"
#define STRING_MEDIUM "Средняя"
#define STRING_HIGH "Высокая"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Уровень"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Всегда выкл"
#define STRING_ALWAYS_ON "Всегда вкл"

// Time and date menu items
#define STRING_TIME_ZONE "Часовой пояс"
#define STRING_YEAR "Год"
#define STRING_MONTH "Месяц"
#define STRING_DAY "День"
#define STRING_HOUR "Час"
#define STRING_MINUTE "Минута"
#define STRING_TIME_FORMAT "Формат времени"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-часовой"
#define STRING_12_HOUR "12-часовой"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Щелочная"

// Random generator menu items
#define STRING_FULL_ASCII "Полный ASCII"
#define STRING_ALPHANUMERIC "Буквенно-цифровой"
#define STRING_HEXADECIMAL "Шестнадцатеричный"
#define STRING_DECIMAL "Десятичный"
#define STRING_BINARY "Двоичный"
#define STRING_100_SIDED_DIE "100-гранный кубик (0-99)"
#define STRING_20_SIDED_DIE "20-гранный кубик"
#define STRING_12_SIDED_DIE "12-гранный кубик"
#define STRING_10_SIDED_DIE "10-гранный кубик (0-9)"
#define STRING_8_SIDED_DIE "8-гранный кубик"
#define STRING_6_SIDED_DIE "6-гранный кубик"
#define STRING_4_SIDED_DIE "4-гранный кубик"
#define STRING_COIN_FLIP "Подброс монеты"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Орёл"
#define STRING_COIN_TAIL "Решка"

// Game menu items
#define STRING_PLAY_WHITE "Играть белыми"
#define STRING_PLAY_BLACK "Играть чёрными"
#define STRING_CONTINUE_GAME "Продолжить игру"
#define STRING_NEW_GAME "Новая игра"
#define STRING_STRENGTH "Сила"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Уровень"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Жизнь трубки"
#define STRING_TUBE_LIFE_PULSES "Импульсы"
#define STRING_TUBE_DEAD_TIME "Мёртвое время"
#define STRING_DEVICE_ID "ID устройства"
#define STRING_DEVICE_VOLTAGE "Напряжение"

#define STRING_TUBE "Трубка"
#define STRING_LIFE_TIME "жизнь"
#define STRING_LIFE_PULSES "Импульсы"
#define STRING_DEAD_TIME "Мёртвое в."
#define STRING_DEVICE "Устройство"
#define STRING_ID "ID"
#define STRING_VOLTAGE "Вольтаж"
