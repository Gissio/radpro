/*
 * Rad Pro
 * UI strings: Japanese
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
#define STRING_COUNT "回"
#define STRING_COUNTS "回"

// Common time menu items
#define STRING_24_HOURS "24時間"
#define STRING_12_HOURS "12時間"
#define STRING_6_HOURS "6時間"
#define STRING_3_HOURS "3時間"
#define STRING_1_HOUR "1時間"
#define STRING_30_MINUTES "30分"
#define STRING_10_MINUTES "10分"
#define STRING_5_MINUTES "5分"
#define STRING_2_MINUTES "2分"
#define STRING_1_MINUTE "1分"
#define STRING_30_SECONDS "30秒"
#define STRING_10_SECONDS "10秒"
#define STRING_5_SECONDS "5秒"
#define STRING_1_SECOND "1秒"

// Common menu items
#define STRING_OFF "オフ"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "極低"
#define STRING_LOW "低"
#define STRING_MEDIUM "中"
#define STRING_HIGH "高"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "警告\n\nファームウェア\tの\tチェックサム\tエラー。"

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "警告\n\n誤った\t値\tは\tデバイス\tを\t損傷する\t可能性\tが\tあります。"

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "データログ\tを\tリセット\tしますか？\n\nすべて\tの\t記録\tが\t削除されます。"

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "成功\n\nデータログ\tが\tリセット\tされました。"

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "瞬間線量率"
#define STRING_AVERAGE "平均線量率"
#define STRING_CUMULATIVE "累積線量"
#define STRING_HISTORY "履歴"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10分"
#define STRING_HISTORY_1_HOUR "1時間"
#define STRING_HISTORY_1_DAY "1日"
#define STRING_HISTORY_1_WEEK "1週"
#define STRING_HISTORY_1_MONTH "1ヶ月"
#define STRING_HISTORY_1_YEAR "1年"

// Measurement view secondary view strings
#define STRING_TIME "時間"
#define STRING_MAX "最大"
#define STRING_RATE "線量率"
#define STRING_DOSE "線量"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_ALERT_ALARM "アラーム"
#define STRING_ALERT_WARNING "警告"
#define STRING_ALERT_FAULT "故障"
#define STRING_ALERT_MAX "最大"
#define STRING_ALERT_OVERFLOW "オーバー"
#define STRING_ALERT_DONE "完了"

// Settings menu items
#define STRING_SETTINGS "設定"
#define STRING_PULSES "パルス"
#define STRING_ALERTS "アラート"
#define STRING_MEASUREMENTS "測定"
#define STRING_GEIGER_TUBE "ガイガーチューブ"
#define STRING_DATALOG "データログ"
#define STRING_DISPLAY "表示"
#define STRING_DATE_AND_TIME "日時"
#define STRING_BATTERY_TYPE "電池種"
#define STRING_RANDOM_GENERATOR "乱数生成"
#define STRING_GAME "ゲーム"
#define STRING_STATISTICS "統計"
#define STRING_DATAMODE "データモード"

// Pulses menu items
#define STRING_SOUND "音"
#define STRING_VOICE "音声"
#define STRING_VIBRATION "振動"
#define STRING_PULSE_LED "パルスLED"
#define STRING_DISPLAY_FLASH "表示点滅"
#define STRING_THRESHOLD "閾値"

// Alarms menu items
#define STRING_RATE_WARNING "計数率警告"
#define STRING_RATE_ALARM "計数率警報"
#define STRING_DOSE_WARNING "線量警告"
#define STRING_DOSE_ALARM "線量警報"
#define STRING_INDICATION "表示"

// Measurements menu items
#define STRING_UNITS "単位"
#define STRING_SECONDARY_UNITS "副次単位"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "適応高速"
#define STRING_ADAPTIVE_PRECISION "適応精密"

// Average menu items
#define STRING_UNLIMITED "無制限"
#define STRING_50_CONFIDENCE "±50%信頼"
#define STRING_20_CONFIDENCE "±20%信頼"
#define STRING_10_CONFIDENCE "±10%信頼"
#define STRING_5_CONFIDENCE "±5%信頼"
#define STRING_2_CONFIDENCE "±2%信頼"
#define STRING_1_CONFIDENCE "±1%信頼"

// Geiger tube menu items
#define STRING_SENSITIVITY "感度"
#define STRING_DEAD_TIME_COMPENSATION "死時間補正"
#define STRING_HVPROFILE "HVプロファイル"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT "(標準)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "工場標準"
#define STRING_ACCURACY "精度"
#define STRING_ENERGY_SAVING "省エネ"
#define STRING_CUSTOM "カスタム"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM周波数"
#define STRING_PWMDUTY_CYCLE "PWMデューティ"

// Data Log menu items
#define STRING_LOGGING_MODE "ログモード"
#define STRING_RESET "リセット"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "毎時"
#define STRING_EVERY10_MINUTES "10分毎"
#define STRING_EVERY_MINUTE "毎分"
#define STRING_EVERY10_SECONDS "10秒毎"
#define STRING_EVERY_SECOND "毎秒"

// Display menu items
#define STRING_THEME "テーマ"
#define STRING_BRIGHTNESS "明るさ"
#define STRING_CONTRAST "コントラスト"
#define STRING_SLEEP "スリープ"

// Display - Theme menu items
#define STRING_THEME_DAY "昼"
#define STRING_THEME_DUSK "夕暮"
#define STRING_THEME_NIGHT "夜"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "レベル"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "常オフ"
#define STRING_ALWAYS_ON "常オン"

// Sound menu items
#define STRING_ALERTSTYLE "アラートスタイル"
#define STRING_ALERTVOLUME "アラート音量"
#define STRING_VOICEVOLUME "音声ボリューム"

// Sound Alarms length
#define STRING_SHORT "短い"
#define STRING_LONG "長い"

// Sound pulses menu items
#define STRING_CLICKS "クリック"
#define STRING_CHIRPS "チャープ"
#define STRING_BEEPS "ビープ"

// Time and date menu items
#define STRING_TIME_ZONE "時間帯"
#define STRING_YEAR "年"
#define STRING_MONTH "月"
#define STRING_DAY "日"
#define STRING_HOUR "時"
#define STRING_MINUTE "分"
#define STRING_TIME_FORMAT "時間形式"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24時間"
#define STRING_12_HOUR "12時間"

// Battery type menu items
#define STRING_NI_MH "ニッケル水素"
#define STRING_ALKALINE "アルカリ"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "英数字"
#define STRING_HEXADECIMAL "16進数"
#define STRING_DECIMAL "10進数"
#define STRING_BINARY "2進数"
#define STRING_100_SIDED_DIE "100面ダイス(0-99)"
#define STRING_20_SIDED_DIE "20面ダイス"
#define STRING_12_SIDED_DIE "12面ダイス"
#define STRING_10_SIDED_DIE "10面ダイス(0-9)"
#define STRING_8_SIDED_DIE "8面ダイス"
#define STRING_6_SIDED_DIE "6面ダイス"
#define STRING_4_SIDED_DIE "4面ダイス"
#define STRING_COIN_FLIP "コイン投げ (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "白をプレイ"
#define STRING_PLAY_BLACK "黒をプレイ"
#define STRING_CONTINUE_GAME "ゲーム続行"
#define STRING_NEW_GAME "新ゲーム"
#define STRING_STRENGTH "強さ"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "レベル"

// Statistics view items
#define STRING_TUBE_LIFE "チューブ寿命"
#define STRING_TUBE "チューブ"
#define STRING_LIFE "寿命"
#define STRING_DEAD_TIME "死時間"
#define STRING_DEVICE_ID "デバイスID"
#define STRING_DEVICE "機器"
#define STRING_ID "ID"
#define STRING_BATTERY "バッテリー"
