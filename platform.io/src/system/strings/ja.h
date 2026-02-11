/*
 * Rad Pro
 * UI strings: Japanese
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

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "回"
#define STRING_COUNTS "回"

// Time strings
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

// Strength strings
#define STRING_OFF "オフ"
#define STRING_VERY_LOW "極低"
#define STRING_LOW "低"
#define STRING_MEDIUM "中"
#define STRING_HIGH "高"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "警告\n\nファームウェア\tの\tチェックサム\tエラー。"

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "警告\n\n誤った\t値\tは\tデバイス\tを\t損傷する\t可能性\tが\tあります。"

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "データログ\tを\tリセット\tしますか？\n\nすべて\tの\t記録\tが\t削除されます。"

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "成功\n\nデータログ\tが\tリセット\tされました。"

// Measurements title strings
#define STRING_INSTANTANEOUS "瞬間線量率"
#define STRING_AVERAGE "平均線量率"
#define STRING_CUMULATIVE "累積線量"
#define STRING_HISTORY "履歴"
#define STRING_ELECTRIC_FIELD "電界"
#define STRING_MAGNETIC_FIELD "磁界"

// Measurements secondary view strings
#define STRING_TIME "時間"
#define STRING_MAX "最大"
#define STRING_RATE "線量率"
#define STRING_DOSE "線量"

// Measurements info strings
#define STRING_ALERT_FAULT "故障"
#define STRING_ALERT_MAX "最大"
#define STRING_ALERT_DONE "完了"
#define STRING_ALERT_UNAVAILABLE "利用不可"

// History period strings
#define STRING_HISTORY_10_MINUTES "10分"
#define STRING_HISTORY_1_HOUR "1時間"
#define STRING_HISTORY_1_DAY "1日"
#define STRING_HISTORY_1_WEEK "1週"
#define STRING_HISTORY_1_MONTH "1ヶ月"
#define STRING_HISTORY_1_YEAR "1年"

// Settings menu items
#define STRING_SETTINGS "設定"
#define STRING_PULSES "パルス"
#define STRING_ALERTS "アラート"
#define STRING_MEASUREMENTS "測定"
#define STRING_GEIGER_TUBE "ガイガーチューブ"
#define STRING_DATALOG "データログ"
#define STRING_DISPLAY "表示"
#define STRING_DATE_AND_TIME "日時"
#define STRING_POWER "電源"
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

// Alerts menu items
#define STRING_INDICATION "表示"
#define STRING_RATE_WARNING "計数率警告"
#define STRING_RATE_ALARM "計数率警報"
#define STRING_DOSE_WARNING "線量警告"
#define STRING_DOSE_ALARM "線量警報"
#define STRING_ELECTRIC_FIELD_ALARM "電界アラーム"
#define STRING_MAGNETIC_FIELD_ALARM "磁界アラーム"

// Measurements menu items
#define STRING_SOURCE "線源"
#define STRING_DOSE_UNITS "線量単位"
#define STRING_SECONDARY_DOSE_UNITS "副線量単位"
#define STRING_MAGNETIC_FIELD_UNITS "磁界単位"

// Source menu items
#define STRING_CS137 "既定: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "ラジウム (γ)"
#define STRING_URANIUM_ORE "ウラン鉱石 (γ)"
#define STRING_URANIUM_GLASS "ウランガラス (γ)"
#define STRING_DEPLETED_URANIUM "劣化ウラン (γ)"
#define STRING_THORIUM_ORE "トリウム鉱石 (γ)"
#define STRING_XRAYS "X線 (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "背景放射線"

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
#define STRING_TUBE_TYPE "管の種類"
#define STRING_SENSITIVITY "感度 (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "死時間補正"
#define STRING_HVPROFILE "HVプロファイル"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011/J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "管のデフォルト"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "工場標準"
#define STRING_ACCURACY "精度"
#define STRING_ENERGY_SAVING "省エネ"
#define STRING_CUSTOM "カスタム"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "PWM周波数"
#define STRING_PWMDUTY_CYCLE "PWMデューティ"

// Data log menu items
#define STRING_LOGGING_MODE "ログモード"
#define STRING_RESET "リセット"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "昼"
#define STRING_THEME_DUSK "夕暮"
#define STRING_THEME_NIGHT "夜"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "レベル"

// Display sleep menu items
#define STRING_ALWAYS_OFF "常オフ"
#define STRING_ALWAYS_ON "常オン"

// Sound menu items
#define STRING_ALERTSTYLE "アラートスタイル"
#define STRING_ALERTVOLUME "アラート音量"
#define STRING_VOICEVOLUME "音声ボリューム"

// Sound alert style menu items
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

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24時間"
#define STRING_12_HOUR "12時間"

// Power menu items
#define STRING_BATTERY "バッテリー"
#define STRING_USB_AUTO_POWER_ON "USB自動電源オン"

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
