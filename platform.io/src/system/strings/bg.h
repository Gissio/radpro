/*
 * Rad Pro
 * UI strings: Bulgarian
 *
 * (C) 2022-2026 Gissio
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
#define STRING_COUNT "имп."
#define STRING_COUNTS "имп."

// Time strings
#define STRING_24_HOURS "24 часа"
#define STRING_12_HOURS "12 часа"
#define STRING_6_HOURS "6 часа"
#define STRING_3_HOURS "3 часа"
#define STRING_1_HOUR "1 час"
#define STRING_30_MINUTES "30 минути"
#define STRING_10_MINUTES "10 минути"
#define STRING_5_MINUTES "5 минути"
#define STRING_2_MINUTES "2 минути"
#define STRING_1_MINUTE "1 минута"
#define STRING_30_SECONDS "30 секунди"
#define STRING_10_SECONDS "10 секунди"
#define STRING_5_SECONDS "5 секунди"
#define STRING_1_SECOND "1 секунда"

// Strength strings
#define STRING_OFF "Изключено"
#define STRING_VERY_LOW "Много ниска"
#define STRING_LOW "Ниска"
#define STRING_MEDIUM "Средна"
#define STRING_HIGH "Висока"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ВНИМАНИЕ\n\nНеуспех при проверка на фърмуера."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "ВНИМАНИЕ\n\nГрешни стойности могат да повредят устройството."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Нулиране на дневника с данни?\n\nВсички записи ще бъдат изтрити."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "УСПЕХ\n\nДневникът с данни е нулиран."

// Measurements title strings
#define STRING_INSTANTANEOUS "Моментно"
#define STRING_AVERAGE "Средно"
#define STRING_CUMULATIVE "С натрупване"
#define STRING_HISTORY "История"
#define STRING_ELECTRIC_FIELD "Електрическо поле"
#define STRING_MAGNETIC_FIELD "Магнитно поле"

// Measurements secondary view strings
#define STRING_TIME "Време"
#define STRING_MAX "Макс"
#define STRING_RATE "Скорост"
#define STRING_DOSE "Доза"
#define STRING_POWER_DENSITY "Плътност на мощността"

// Measurements info strings
#define STRING_ALERT_FAULT "ГРЕШКА"
#define STRING_ALERT_MAX "МАКС"
#define STRING_ALERT_DONE "ГОТОВО"
#define STRING_ALERT_UNAVAILABLE "НЕДОСТЪПНО"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 мин"
#define STRING_HISTORY_1_HOUR "1 ч"
#define STRING_HISTORY_1_DAY "1 д"
#define STRING_HISTORY_1_WEEK "1 седм"
#define STRING_HISTORY_1_MONTH "1 мес"
#define STRING_HISTORY_1_YEAR "1 г"

// Settings menu items
#define STRING_SETTINGS "Настройки"
#define STRING_PULSES "Импулси"
#define STRING_ALERTS "Сигнали"
#define STRING_MEASUREMENTS "Измервания"
#define STRING_GEIGER_TUBE "G-M брояч"
#define STRING_DATALOG "Журнал с данни"
#define STRING_DISPLAY "Дисплей"
#define STRING_DATE_AND_TIME "Дата и час"
#define STRING_POWER "Захранване"
#define STRING_RANDOM_GENERATOR "Случаен генератор"
#define STRING_GAME "Игра"
#define STRING_STATISTICS "Статистика"
#define STRING_DATAMODE "Режим на данни"

// Pulses menu items
#define STRING_SOUND "Звук"
#define STRING_VOICE "Глас"
#define STRING_VIBRATION "Вибрация"
#define STRING_PULSE_LED "Светодиод за импулс"
#define STRING_DISPLAY_FLASH "Мигане на дисплея"
#define STRING_THRESHOLD "Праг"

// Alerts menu items
#define STRING_INDICATION "Индикация"
#define STRING_RATE_WARNING "Предупр. за скорост"
#define STRING_RATE_ALARM "Аларма за скорост"
#define STRING_DOSE_WARNING "Предупр. за доза"
#define STRING_DOSE_ALARM "Аларма за доза"
#define STRING_ELECTRIC_FIELD_ALARM "Аларма Е-поле"
#define STRING_MAGNETIC_FIELD_ALARM "Аларма М-поле"

// Measurements menu items
#define STRING_SOURCE "Източник"
#define STRING_DOSE_UNITS "Единици за доза"
#define STRING_SECONDARY_DOSE_UNITS "Вторични единици за доза"
#define STRING_MAGNETIC_FIELD_UNITS "Единици за магнитно поле"

// Source menu items
#define STRING_CS137 "По подразб.: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Радий (γ)"
#define STRING_URANIUM_ORE "Уранова руда (γ)"
#define STRING_URANIUM_GLASS "Ураново стъкло (γ)"
#define STRING_DEPLETED_URANIUM "Обеднен уран (γ)"
#define STRING_THORIUM_ORE "Ториeва руда (γ)"
#define STRING_XRAYS "Рентген (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Фонова радиация"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Адаптивно-бързо"
#define STRING_ADAPTIVE_PRECISION "Адаптивно-прецизно"

// Average menu items
#define STRING_UNLIMITED "Неограничено"
#define STRING_50_CONFIDENCE "±50% достоверност"
#define STRING_20_CONFIDENCE "±20% достоверност"
#define STRING_10_CONFIDENCE "±10% достоверност"
#define STRING_5_CONFIDENCE "±5% достоверност"
#define STRING_2_CONFIDENCE "±2% достоверност"
#define STRING_1_CONFIDENCE "±1% достоверност"

// Geiger tube menu items
#define STRING_TUBE_TYPE "Тип на тръбата"
#define STRING_SENSITIVITY "Чувствителност (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Комп. на мъртво време"
#define STRING_HVPROFILE "HV профил"

// Geiger tube type strings
#define STRING_J305 "J305 (107 mm)"
#define STRING_M4011 "M4011 (87 mm)"
#define STRING_HH614 "HH614"
#define STRING_LND7317 "LND 7317"

// Geiger tube type strings that should be translated with cyrillic scripts
#define STRING_SBM20 "СБМ20"
#define STRING_SI3BG "СИ3БГ"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Тръба по подразбиране"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Фабричен"
#define STRING_ACCURACY "Прецизен"
#define STRING_ENERGY_SAVING "Енергоспестяващ"
#define STRING_CUSTOM "Потребителски"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "ШИМ честота"
#define STRING_PWMDUTY_CYCLE "ШИМ коеф. запълване"

// Data log menu items
#define STRING_LOGGING_MODE "Режим на запис"
#define STRING_RESET "Изтриване"

// Data log options menu items
#define STRING_EVERY_HOUR "На всеки час"
#define STRING_EVERY10_MINUTES "На всеки 10 минути"
#define STRING_EVERY_MINUTE "На всяка минута"
#define STRING_EVERY10_SECONDS "На всеки 10 секунди"
#define STRING_EVERY_SECOND "На всяка секунда"

// Display menu items
#define STRING_THEME "Тема"
#define STRING_BRIGHTNESS "Яркост"
#define STRING_CONTRAST "Контраст"
#define STRING_SLEEP "Заспиване"

// Display theme menu items
#define STRING_THEME_DAY "Ден"
#define STRING_THEME_DUSK "Здрач"
#define STRING_THEME_NIGHT "Нощ"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Ниво"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Винаги изключен"
#define STRING_ALWAYS_ON "Винаги включен"

// Sound menu items
#define STRING_ALERTSTYLE "Стил на сигнала"
#define STRING_ALERTVOLUME "Сила на звука на сигнала"
#define STRING_VOICEVOLUME "Сила на гласа"

// Sound alert style menu items
#define STRING_SHORT "Кратък"
#define STRING_LONG "Дълъг"

// Sound pulses menu items
#define STRING_CLICKS "Щракане"
#define STRING_CHIRPS "Цвъркане"
#define STRING_BEEPS "Бипкане"

// Time and date menu items
#define STRING_TIME_ZONE "Часова зона"
#define STRING_YEAR "Година"
#define STRING_MONTH "Месец"
#define STRING_DAY "Ден"
#define STRING_HOUR "Час"
#define STRING_MINUTE "Минута"
#define STRING_TIME_FORMAT "Формат на часа"

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24-часов"
#define STRING_12_HOUR "12-часов"

// Power menu items
#define STRING_BATTERY "Батерия"
#define STRING_USB_AUTO_POWER_ON "USB автоматично включване"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Алкална"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Буквено-цифрен"
#define STRING_HEXADECIMAL "Шестнадесетичен"
#define STRING_DECIMAL "Десетичен"
#define STRING_BINARY "Двоичен"
#define STRING_100_SIDED_DIE "100-стенен зар (0-99)"
#define STRING_20_SIDED_DIE "20-стенен зар"
#define STRING_12_SIDED_DIE "12-стенен зар"
#define STRING_10_SIDED_DIE "10-стенен зар (0-9)"
#define STRING_8_SIDED_DIE "8-стенен зар"
#define STRING_6_SIDED_DIE "6-стенен зар"
#define STRING_4_SIDED_DIE "4-стенен зар"
#define STRING_COIN_FLIP "Хвърляне на монета (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Играй с белите"
#define STRING_PLAY_BLACK "Играй с черните"
#define STRING_CONTINUE_GAME "Продължи играта"
#define STRING_NEW_GAME "Нова игра"
#define STRING_STRENGTH "Трудност"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Ниво"

// Statistics view items
#define STRING_TUBE_LIFE "Живот на детектора"
#define STRING_TUBE "Детектор"
#define STRING_LIFE "Живот"
#define STRING_DEAD_TIME "Мъртво време"
#define STRING_DEVICE_ID "ИД на устройството"
#define STRING_DEVICE "Устройство"
#define STRING_ID "ИД"
