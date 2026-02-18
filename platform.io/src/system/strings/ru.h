/*
 * Rad Pro
 * UI strings: Russian
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
#define STRING_NANO "н"
#define STRING_MICRO "мк"
#define STRING_MILLI "м"
#define STRING_KILO "к"
#define STRING_MEGA "М"
#define STRING_GIGA "Г"
#define STRING_SIEVERT "Зиверт"
#define STRING_SV "Зв"
#define STRING_SVH "Зв/ч"
#define STRING_REM "бэр"
#define STRING_REMH "бэр/ч"
#define STRING_CPM "имп/мин"
#define STRING_CPS "имп/с"
#define STRING_CPMUSVH "(имп/мин)/(мкЗв/ч)"
#define STRING_MICROSECONDS "мкс"
#define STRING_KHZ "кГц"
#define STRING_VOLT_UNIT "В"
#define STRING_VOLT_PER_METER_UNIT "В/м"
#define STRING_TESLA "Тесла"
#define STRING_TESLA_UNIT "Тл"
#define STRING_GAUSS "Гаусс"
#define STRING_GAUSS_UNIT "Гс"

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "имп"
#define STRING_COUNTS "имп"

// Time strings
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

// Strength strings
#define STRING_OFF "Выкл"
#define STRING_VERY_LOW "Очень низкая"
#define STRING_LOW "Низкая"
#define STRING_MEDIUM "Средняя"
#define STRING_HIGH "Высокая"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ПРЕДУПРЕЖДЕНИЕ\n\nСбой контрольной суммы прошивки."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "ПРЕДУПРЕЖДЕНИЕ\n\nНеправильные значения могут повредить устройство."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Сбросить журнал данных?\n\nВсе записи будут удалены."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "УСПЕХ\n\nЖурнал данных сброшен."

// Measurements title strings
#define STRING_INSTANTANEOUS "Мгновенное"
#define STRING_AVERAGE "Среднее"
#define STRING_CUMULATIVE "Накопленная"
#define STRING_HISTORY "История"
#define STRING_ELECTRIC_FIELD "Электрическое поле"
#define STRING_MAGNETIC_FIELD "Магнитное поле"

// Measurements secondary view strings
#define STRING_TIME "Время"
#define STRING_MAX "Макс"
#define STRING_RATE "Скорост"
#define STRING_DOSE "Доза"

// Measurements info strings
#define STRING_ALERT_FAULT "НЕИСПРАВНОСТЬ"
#define STRING_ALERT_DONE "ГОТОВО"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 мин"
#define STRING_HISTORY_1_HOUR "1 ч"
#define STRING_HISTORY_1_DAY "1 д"
#define STRING_HISTORY_1_WEEK "1 нед"
#define STRING_HISTORY_1_MONTH "1 мес"
#define STRING_HISTORY_1_YEAR "1 г"

// Settings menu items
#define STRING_SETTINGS "Настройки"
#define STRING_PULSES "Импульсы"
#define STRING_ALERTS "Оповещения"
#define STRING_MEASUREMENTS "Измерения"
#define STRING_GEIGER_TUBE "Счётчик Гейгера"
#define STRING_DATALOG "Журнал данных"
#define STRING_DISPLAY "Дисплей"
#define STRING_DATE_AND_TIME "Дата и время"
#define STRING_POWER "Питание"
#define STRING_RANDOM_GENERATOR "Генератор случайных чисел"
#define STRING_GAME "Игра"
#define STRING_STATISTICS "Статистика"
#define STRING_DATAMODE "Режим данных"

// Pulses menu items
#define STRING_SOUND "Звук"
#define STRING_VOICE "Голос"
#define STRING_VIBRATION "Вибрация"
#define STRING_PULSE_LED "Светодиод импульса"
#define STRING_DISPLAY_FLASH "Мигание дисплея"
#define STRING_THRESHOLD "Порог тревоги"

// Alerts menu items
#define STRING_INDICATION "Индикация"
#define STRING_RATE_WARNING "Предупр. скорости"
#define STRING_RATE_ALARM "Сигнал скорости"
#define STRING_DOSE_WARNING "Предупр. дозы"
#define STRING_DOSE_ALARM "Сигнал дозы"
#define STRING_ELECTRIC_FIELD_ALARM "Тревога эл. поля"
#define STRING_MAGNETIC_FIELD_ALARM "Тревога магн. поля"

// Measurements menu items
#define STRING_SOURCE "Источник"
#define STRING_DOSE_UNITS "Единицы дозы"
#define STRING_SECONDARY_DOSE_UNITS "Вторичные единицы дозы"
#define STRING_MAGNETIC_FIELD_UNITS "Единицы магн. поля"

// Source menu items
#define STRING_CS137 "По умолч.: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Радий (γ)"
#define STRING_URANIUM_ORE "Урановая руда (γ)"
#define STRING_URANIUM_GLASS "Урановое стекло (γ)"
#define STRING_DEPLETED_URANIUM "Обеднённый уран (γ)"
#define STRING_THORIUM_ORE "Ториевая руда (γ)"
#define STRING_XRAYS "Рентген (60 кВ)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Радиационный фон"

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
#define STRING_TUBE_TYPE "Тип трубки"
#define STRING_SENSITIVITY "Чувствительность (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Комп. мёртвого времени"
#define STRING_HVPROFILE "Профиль HV"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011"
#define STRING_J321 "J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "СБМ20"
#define STRING_SI3BG "СИ3БГ"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Трубка по умолчанию"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Заводской по умолчанию"
#define STRING_ACCURACY "Точность"
#define STRING_ENERGY_SAVING "Энергосбережение"
#define STRING_CUSTOM "Пользовательский"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "Частота ШИМ"
#define STRING_PWMDUTY_CYCLE "Скважность ШИМ"

// Data log menu items
#define STRING_LOGGING_MODE "Режим записи"
#define STRING_RESET "Очистить"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "День"
#define STRING_THEME_DUSK "Сумерки"
#define STRING_THEME_NIGHT "Ночь"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Уровень"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Всегда выкл"
#define STRING_ALWAYS_ON "Всегда вкл"

// Sound menu items
#define STRING_ALERTSTYLE "Стиль оповещения"
#define STRING_ALERTVOLUME "Громкость оповещения"
#define STRING_VOICEVOLUME "Громкость голоса"

// Sound alert style menu items
#define STRING_SHORT "Короткий"
#define STRING_LONG "Длинный"

// Sound pulses menu items
#define STRING_CLICKS "Щелчки"
#define STRING_CHIRPS "Чирпы"
#define STRING_BEEPS "Гудки"

// Time and date menu items
#define STRING_TIME_ZONE "Часовой пояс"
#define STRING_YEAR "Год"
#define STRING_MONTH "Месяц"
#define STRING_DAY "День"
#define STRING_HOUR "Час"
#define STRING_MINUTE "Минута"
#define STRING_TIME_FORMAT "Формат времени"

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24-часовой"
#define STRING_12_HOUR "12-часовой"

// Power menu items
#define STRING_BATTERY "Батарея"
#define STRING_USB_AUTOPOWER_ON "USB автовключение"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Щелочная"

// Random generator menu items
#define STRING_ASCII "ASCII"
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
#define STRING_COIN_FLIP "Подброс монеты (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Играть белыми"
#define STRING_PLAY_BLACK "Играть чёрными"
#define STRING_CONTINUE_GAME "Продолжить игру"
#define STRING_NEW_GAME "Новая игра"
#define STRING_STRENGTH "Сложность"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Уровень"

// Statistics view items
#define STRING_TUBE_LIFE "Работа детектора"
#define STRING_TUBE "Детектор"
#define STRING_LIFE "Наработка"
#define STRING_DEAD_TIME "Мёртвое время"
#define STRING_DEVICE_ID "ИД устройства"
#define STRING_DEVICE "Устройство"
#define STRING_ID "ИД"
