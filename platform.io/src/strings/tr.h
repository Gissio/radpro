/*
 * Rad Pro
 * UI strings: Turkish
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
#define STRING_COUNT "sayım"
#define STRING_COUNTS "sayım"

// Common time menu items
#define STRING_24_HOURS "24 saat"
#define STRING_12_HOURS "12 saat"
#define STRING_6_HOURS "6 saat"
#define STRING_3_HOURS "3 saat"
#define STRING_1_HOUR "1 saat"
#define STRING_30_MINUTES "30 dakika"
#define STRING_10_MINUTES "10 dakika"
#define STRING_5_MINUTES "5 dakika"
#define STRING_2_MINUTES "2 dakika"
#define STRING_1_MINUTE "1 dakika"
#define STRING_30_SECONDS "30 saniye"
#define STRING_10_SECONDS "10 saniye"
#define STRING_5_SECONDS "5 saniye"
#define STRING_1_SECOND "1 saniye"

// Common menu items
#define STRING_OFF "Kapalı"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Çok düşük"
#define STRING_LOW "Düşük"
#define STRING_MEDIUM "Orta"
#define STRING_HIGH "Yüksek"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "UYARI\n\nYazılım sağlama toplaması hatası."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "UYARI\n\nYanlış değerler cihaza zarar verebilir."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Veri günlüğünü sıfırla?\n\nTüm kayıtlar silinecek."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "BAŞARILI\n\nVeri günlüğü sıfırlandı."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Anlık"
#define STRING_AVERAGE "Ortalama"
#define STRING_CUMULATIVE "Toplam"
#define STRING_HISTORY "Geçmiş"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 dk"
#define STRING_HISTORY_1_HOUR "1 s"
#define STRING_HISTORY_1_DAY "1 g"
#define STRING_HISTORY_1_WEEK "1 h"
#define STRING_HISTORY_1_MONTH "1 ay"
#define STRING_HISTORY_1_YEAR "1 y"

// Measurement view secondary view strings
#define STRING_TIME "Zaman"
#define STRING_MAX "Maks"
#define STRING_RATE "Oran"
#define STRING_DOSE "Doz"

// Measurement view info strings
#define STRING_ALERT_ALARM "ALARM"
#define STRING_ALERT_WARNING "UYARI"
#define STRING_ALERT_FAULT "ARIZA"
#define STRING_ALERT_MAX "MAKS"
#define STRING_ALERT_DONE "BİTTİ"

// Settings menu items
#define STRING_SETTINGS "Ayarlar"
#define STRING_PULSES "Naberler"
#define STRING_ALERTS "Uyarılar"
#define STRING_MEASUREMENTS "Ölçümler"
#define STRING_GEIGER_TUBE "Geiger tüpü"
#define STRING_DATALOG "Veri günlüğü"
#define STRING_DISPLAY "Ekran"
#define STRING_DATE_AND_TIME "Tarih ve saat"
#define STRING_BATTERY_TYPE "Pil tipi"
#define STRING_RANDOM_GENERATOR "Rastgele üreteç"
#define STRING_GAME "Oyun"
#define STRING_STATISTICS "İstatistikler"
#define STRING_DATAMODE "Veri modu"

// Pulses menu items
#define STRING_SOUND "Ses"
#define STRING_VOICE "Konuşma"
#define STRING_VIBRATION "Titreşim"
#define STRING_PULSE_LED "Naber LED'i"
#define STRING_DISPLAY_FLASH "Ekran flaşı"
#define STRING_THRESHOLD "Eşik"

// Alarms menu items
#define STRING_RATE_WARNING "Oran uyarısı"
#define STRING_RATE_ALARM "Oran alarmı"
#define STRING_DOSE_WARNING "Doz uyarısı"
#define STRING_DOSE_ALARM "Doz alarmı"
#define STRING_INDICATION "Gösterge"

// Measurements menu items
#define STRING_UNITS "Birimler"
#define STRING_SECONDARY_UNITS "İkincil birimler"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Uyarlanabilir hızlı"
#define STRING_ADAPTIVE_PRECISION "Uyarlanabilir hassas"

// Average menu items
#define STRING_UNLIMITED "Sınırsız"
#define STRING_50_CONFIDENCE "±%50 güven"
#define STRING_20_CONFIDENCE "±%20 güven"
#define STRING_10_CONFIDENCE "±%10 güven"
#define STRING_5_CONFIDENCE "±%5 güven"
#define STRING_2_CONFIDENCE "±%2 güven"
#define STRING_1_CONFIDENCE "±%1 güven"

// Geiger tube menu items
#define STRING_SENSITIVITY "Hassasiyet"
#define STRING_DEAD_TIME_COMPENSATION "Ölü zaman telafisi"
#define STRING_HVPROFILE "HV profili"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (varsayılan)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Fabrika varsayılanı"
#define STRING_ACCURACY "Doğruluk"
#define STRING_ENERGY_SAVING "Enerji tasarrufu"
#define STRING_CUSTOM "Özel"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM frekansı"
#define STRING_PWMDUTY_CYCLE "PWM görev döngüsü"

// Data Log menu items
#define STRING_LOGGING_MODE "Kayıt modu"
#define STRING_RESET "Sıfırla"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Her saat"
#define STRING_EVERY10_MINUTES "Her 10 dakika"
#define STRING_EVERY_MINUTE "Her dakika"
#define STRING_EVERY10_SECONDS "Her 10 saniye"
#define STRING_EVERY_SECOND "Her saniye"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Parlaklık"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Uyku"

// Display - Theme menu items
#define STRING_THEME_DAY "Gün"
#define STRING_THEME_DUSK "Alacakaranlık"
#define STRING_THEME_NIGHT "Gece"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Seviye"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Her zaman kapalı"
#define STRING_ALWAYS_ON "Her zaman açık"

// Sound menu items
#define STRING_ALERTSTYLE "Uyarı stili"
#define STRING_ALERTVOLUME "Uyarı ses seviyesi"
#define STRING_VOICEVOLUME "Ses seviyesi"

// Sound Alarms length
#define STRING_SHORT "Kısa"
#define STRING_LONG "Uzun"

// Sound pulses menu items
#define STRING_CLICKS "Tıklamalar"
#define STRING_CHIRPS "Cıvıltılar"
#define STRING_BEEPS "Bip sesleri"

// Time and date menu items
#define STRING_TIME_ZONE "Zaman dilimi"
#define STRING_YEAR "Yıl"
#define STRING_MONTH "Ay"
#define STRING_DAY "Gün"
#define STRING_HOUR "Saat"
#define STRING_MINUTE "Dakika"
#define STRING_TIME_FORMAT "Saat formatı"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 saat"
#define STRING_12_HOUR "12 saat"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalin"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanümerik"
#define STRING_HEXADECIMAL "Onaltılık"
#define STRING_DECIMAL "Ondalık"
#define STRING_BINARY "İkili"
#define STRING_100_SIDED_DIE "100 yüzlü zar (0-99)"
#define STRING_20_SIDED_DIE "20 yüzlü zar"
#define STRING_12_SIDED_DIE "12 yüzlü zar"
#define STRING_10_SIDED_DIE "10 yüzlü zar (0-9)"
#define STRING_8_SIDED_DIE "8 yüzlü zar"
#define STRING_6_SIDED_DIE "6 yüzlü zar"
#define STRING_4_SIDED_DIE "4 yüzlü zar"
#define STRING_COIN_FLIP "Yazı tura (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Beyaz oyna"
#define STRING_PLAY_BLACK "Siyah oyna"
#define STRING_CONTINUE_GAME "Oyuna devam et"
#define STRING_NEW_GAME "Yeni oyun"
#define STRING_STRENGTH "Güç"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Seviye"

// Statistics view items
#define STRING_TUBE_LIFE "Tüp ömrü"
#define STRING_TUBE "Tüp"
#define STRING_LIFE "Ömür"
#define STRING_DEAD_TIME "Ölü zaman"
#define STRING_DEVICE_ID "Cihaz ID"
#define STRING_DEVICE "Cihaz"
#define STRING_ID "ID"
#define STRING_BATTERY "Pil"
