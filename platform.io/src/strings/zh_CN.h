/*
 * Rad Pro
 * UI strings: Simplified Chinese
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
#define STRING_COUNT "次"
#define STRING_COUNTS "次"

// Common time menu items
#define STRING_24_HOURS "24小时"
#define STRING_12_HOURS "12小时"
#define STRING_6_HOURS "6小时"
#define STRING_3_HOURS "3小时"
#define STRING_1_HOUR "1小时"
#define STRING_30_MINUTES "30分钟"
#define STRING_10_MINUTES "10分钟"
#define STRING_5_MINUTES "5分钟"
#define STRING_2_MINUTES "2分钟"
#define STRING_1_MINUTE "1分钟"
#define STRING_30_SECONDS "30秒"
#define STRING_10_SECONDS "10秒"
#define STRING_5_SECONDS "5秒"
#define STRING_1_SECOND "1秒"

// Common menu items
#define STRING_OFF "关"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "极低"
#define STRING_LOW "低"
#define STRING_MEDIUM "中"
#define STRING_HIGH "高"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING ""
#define STRING_NOTIFICATION_SUCCESS ""

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "警告\n\n固件\t校验和\t失败。"

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "警告\n\n错误的值\t可能会\t损坏设备。"

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "重置\t数据记录？\n\n所有记录\t将被删除"

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "成功\n\n数据\t日志\t已重置。"

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "瞬时"
#define STRING_AVERAGE "平均"
#define STRING_CUMULATIVE "累积"
#define STRING_HISTORY "历史"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10分钟"
#define STRING_HISTORY_1_HOUR "1小时"
#define STRING_HISTORY_1_DAY "1天"
#define STRING_HISTORY_1_WEEK "1周"
#define STRING_HISTORY_1_MONTH "1个月"
#define STRING_HISTORY_1_YEAR "1年"

// Measurement view secondary view strings
#define STRING_TIME "时间"
#define STRING_MAX "最大"
#define STRING_RATE "速率"
#define STRING_DOSE "剂量"

// Measurement view info strings
#define STRING_ALERT_ALARM "警报"
#define STRING_ALERT_WARNING "警告"
#define STRING_ALERT_FAULT "故障"
#define STRING_ALERT_MAX "最大"
#define STRING_ALERT_DONE "完成"

// Settings menu items
#define STRING_SETTINGS "设置"
#define STRING_PULSES "脉冲"
#define STRING_ALERTS "警报"
#define STRING_MEASUREMENTS "测量"
#define STRING_GEIGER_TUBE "盖革管"
#define STRING_DATALOG "数据记录"
#define STRING_DISPLAY "显示"
#define STRING_DATE_AND_TIME "日期时间"
#define STRING_BATTERY_TYPE "电池类型"
#define STRING_RANDOM_GENERATOR "随机生成"
#define STRING_GAME "游戏"
#define STRING_STATISTICS "统计"
#define STRING_DATAMODE "数据模式"

// Pulses menu items
#define STRING_SOUND "声音"
#define STRING_VOICE "语音"
#define STRING_VIBRATION "振动"
#define STRING_PULSE_LED "脉冲灯"
#define STRING_DISPLAY_FLASH "屏幕闪烁"
#define STRING_THRESHOLD "阈值"

// Alarms menu items
#define STRING_RATE_WARNING "速率警告"
#define STRING_RATE_ALARM "速率报警"
#define STRING_DOSE_WARNING "剂量警告"
#define STRING_DOSE_ALARM "剂量报警"
#define STRING_INDICATION "指示"

// Measurements menu items
#define STRING_UNITS "单位"
#define STRING_SECONDARY_UNITS "次要单位"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "自适应快速"
#define STRING_ADAPTIVE_PRECISION "自适应精确"

// Average menu items
#define STRING_UNLIMITED "无限制"
#define STRING_50_CONFIDENCE "±50%置信"
#define STRING_20_CONFIDENCE "±20%置信"
#define STRING_10_CONFIDENCE "±10%置信"
#define STRING_5_CONFIDENCE "±5%置信"
#define STRING_2_CONFIDENCE "±2%置信"
#define STRING_1_CONFIDENCE "±1%置信"

// Geiger tube menu items
#define STRING_SENSITIVITY "灵敏度"
#define STRING_DEAD_TIME_COMPENSATION "死时间补偿"
#define STRING_HVPROFILE "高压配置"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT "(默认)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "出厂默认"
#define STRING_ACCURACY "精确"
#define STRING_ENERGY_SAVING "节能"
#define STRING_CUSTOM "自定义"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM频率"
#define STRING_PWMDUTY_CYCLE "PWM占空比"

// Data Log menu items
#define STRING_LOGGING_MODE "记录模式"
#define STRING_RESET "重置"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "每小时"
#define STRING_EVERY10_MINUTES "每10分钟"
#define STRING_EVERY_MINUTE "每分钟"
#define STRING_EVERY10_SECONDS "每10秒"
#define STRING_EVERY_SECOND "每秒"

// Display menu items
#define STRING_THEME "主题"
#define STRING_BRIGHTNESS "亮度"
#define STRING_CONTRAST "对比度"
#define STRING_SLEEP "休眠"

// Display - Theme menu items
#define STRING_THEME_DAY "白天"
#define STRING_THEME_DUSK "黄昏"
#define STRING_THEME_NIGHT "夜间"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "级别"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "始终关闭"
#define STRING_ALWAYS_ON "始终开启"

// Sound menu items
#define STRING_ALERTSTYLE "警报样式"
#define STRING_ALERTVOLUME "警报音量"
#define STRING_VOICEVOLUME "语音音量"

// Sound Alarms length
#define STRING_SHORT "短"
#define STRING_LONG "长"

// Sound pulses menu items
#define STRING_CLICKS "点击"
#define STRING_CHIRPS "啁啾"
#define STRING_BEEPS "蜂鸣"

// Time and date menu items
#define STRING_TIME_ZONE "时区"
#define STRING_YEAR "年"
#define STRING_MONTH "月"
#define STRING_DAY "日"
#define STRING_HOUR "小时"
#define STRING_MINUTE "分钟"
#define STRING_TIME_FORMAT "时间格式"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24小时"
#define STRING_12_HOUR "12小时"

// Battery type menu items
#define STRING_NI_MH "镍氢"
#define STRING_ALKALINE "碱性"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "字母数字"
#define STRING_HEXADECIMAL "十六进制"
#define STRING_DECIMAL "十进制"
#define STRING_BINARY "二进制"
#define STRING_100_SIDED_DIE "100面骰子(0-99)"
#define STRING_20_SIDED_DIE "20面骰子"
#define STRING_12_SIDED_DIE "12面骰子"
#define STRING_10_SIDED_DIE "10面骰子(0-9)"
#define STRING_8_SIDED_DIE "8面骰子"
#define STRING_6_SIDED_DIE "6面骰子"
#define STRING_4_SIDED_DIE "4面骰子"
#define STRING_COIN_FLIP "抛硬币 (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "执白"
#define STRING_PLAY_BLACK "执黑"
#define STRING_CONTINUE_GAME "继续游戏"
#define STRING_NEW_GAME "新游戏"
#define STRING_STRENGTH "强度"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "级别"

// Statistics view items
#define STRING_TUBE_LIFE "管寿命"
#define STRING_TUBE "管"
#define STRING_LIFE "寿命"
#define STRING_DEAD_TIME "死时间"
#define STRING_DEVICE_ID "设备ID"
#define STRING_DEVICE "设备"
#define STRING_ID "ID"
#define STRING_BATTERY "电池"
