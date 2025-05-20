/*
 * Rad Pro
 * UI strings: Vietnamese
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

// Application name (should not be translated)
#define STRING_APP_NAME "Rad Pro"

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
#define STRING_COUNT "lần"
#define STRING_COUNTS "lần"

// Common time menu items
#define STRING_24_HOURS "24 giờ"
#define STRING_12_HOURS "12 giờ"
#define STRING_6_HOURS "6 giờ"
#define STRING_3_HOURS "3 giờ"
#define STRING_1_HOUR "1 giờ"
#define STRING_30_MINUTES "30 phút"
#define STRING_10_MINUTES "10 phút"
#define STRING_5_MINUTES "5 phút"
#define STRING_2_MINUTES "2 phút"
#define STRING_1_MINUTE "1 phút"
#define STRING_30_SECONDS "30 giây"
#define STRING_10_SECONDS "10 giây"
#define STRING_5_SECONDS "5 giây"
#define STRING_1_SECOND "1 giây"

// Common menu items
#define STRING_OFF "Tắt"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "CẢNH BÁO"
#define STRING_NOTIFICATION_SUCCESS "THÀNH CÔNG"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Lỗi kiểm tra firmware."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Giá trị sai hại thiết bị."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Đặt lại?"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Mọi bản ghi sẽ bị xóa."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Nhật ký dữ liệu đã reset."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Tức thời"
#define STRING_AVERAGE "Trung bình"
#define STRING_CUMULATIVE "Tích lũy"
#define STRING_HISTORY10_MIN "Lịch sử (10 phút)"
#define STRING_HISTORY1_H "Lịch sử (1 giờ)"
#define STRING_HISTORY24_H "Lịch sử (24 giờ)"

// Measurement view secondary view strings
#define STRING_TIME "Thời"
#define STRING_MAX "Max"
#define STRING_RATE "Tốc độ"
#define STRING_DOSE "Liều"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "BÁO"
#define STRING_INFO_FAULT "LỖI"
#define STRING_INFO_MAX "MAX"
#define STRING_INFO_OVER "VƯỢT"
#define STRING_INFO_DONE "XONG"

// Settings menu items
#define STRING_SETTINGS "Cài đặt"
#define STRING_PULSES "Xung"
#define STRING_ALARMS "Cảnh báo"
#define STRING_MEASUREMENTS "Đo lường"
#define STRING_GEIGER_TUBE "Ống Geiger"
#define STRING_DATA_LOG "Nhật ký dữ liệu"
#define STRING_DISPLAY "Màn hình"
#define STRING_DATE_AND_TIME "Ngày và giờ"
#define STRING_BATTERY_TYPE "Loại pin"
#define STRING_RANDOM_GENERATOR "Tạo số ngẫu nhiên"
#define STRING_GAME "Trò chơi"
#define STRING_STATISTICS "Thống kê"
#define STRING_DATA_MODE "Chế độ dữ liệu"

// Pulses menu items
#define STRING_SOUND "Âm thanh"
#define STRING_VIBRATION "Rung"
#define STRING_ALERT_LED "LED cảnh báo"
#define STRING_PULSE_LED "LED xung"
#define STRING_DISPLAY_FLASH "Nhấp nháy màn hình"
#define STRING_THRESHOLD "Ngưỡng"

// Sound menu items
#define STRING_CLICKS "Tiếng click"
#define STRING_CHIRPS "Tiếng chirp"
#define STRING_BEEPS "Tiếng bíp"

// Alarms menu items
#define STRING_RATE_ALARM "Cảnh báo tốc độ"
#define STRING_DOSE_ALARM "Cảnh báo liều lượng"
#define STRING_OVERRANGE_ALARM "Cảnh báo vượt phạm vi"
#define STRING_INDICATION "Chỉ báo"

// Measurements menu items
#define STRING_UNITS "Đơn vị"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Thích nghi nhanh"
#define STRING_ADAPTIVE_PRECISION "Thích nghi chính xác"

// Average menu items
#define STRING_UNLIMITED "Không giới hạn"
#define STRING_50_CONFIDENCE "±50% độ tin cậy"
#define STRING_20_CONFIDENCE "±20% độ tin cậy"
#define STRING_10_CONFIDENCE "±10% độ tin cậy"
#define STRING_5_CONFIDENCE "±5% độ tin cậy"
#define STRING_2_CONFIDENCE "±2% độ tin cậy"
#define STRING_1_CONFIDENCE "±1% độ tin cậy"

// Geiger tube menu items
#define STRING_SENSITIVITY "Độ nhạy"
#define STRING_DEAD_TIME_COMPENSATION "Bù thời gian chết"
#define STRING_HVPROFILE "Hồ sơ HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (mặc định)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with non-latin scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Mặc định nhà máy"
#define STRING_ACCURACY "Độ chính xác"
#define STRING_ENERGY_SAVING "Tiết kiệm năng lượng"
#define STRING_CUSTOM "Tùy chỉnh"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Tần số PWM"
#define STRING_PWMDUTY_CYCLE "Chu kỳ nhiệm vụ PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Chế độ ghi"
#define STRING_RESET "Đặt lại"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Mỗi giờ"
#define STRING_EVERY10_MINUTES "Mỗi 10 phút"
#define STRING_EVERY_MINUTE "Mỗi phút"
#define STRING_EVERY10_SECONDS "Mỗi 10 giây"
#define STRING_EVERY_SECOND "Mỗi giây"

// Display menu items
#define STRING_THEME "Chủ đề"
#define STRING_BRIGHTNESS "Độ sáng"
#define STRING_CONTRAST "Độ tương phản"
#define STRING_SLEEP "Ngủ"

// Display - Theme menu items
#define STRING_THEME_DAY "Ngày"
#define STRING_THEME_DUSK "Hoàng hôn"
#define STRING_THEME_NIGHT "Đêm"

// Display - Brightness menu items
#define STRING_VERY_LOW "Rất thấp"
#define STRING_LOW "Thấp"
#define STRING_MEDIUM "Trung bình"
#define STRING_HIGH "Cao"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Cấp độ"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Luôn tắt"
#define STRING_ALWAYS_ON "Luôn bật"

// Time and date menu items
#define STRING_TIME_ZONE "Múi giờ"
#define STRING_YEAR "Năm"
#define STRING_MONTH "Tháng"
#define STRING_DAY "Ngày"
#define STRING_HOUR "Giờ"
#define STRING_MINUTE "Phút"
#define STRING_TIME_FORMAT "Định dạng giờ"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " AM"
#define STRING_PM " PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 giờ"
#define STRING_12_HOUR "12 giờ"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Kiềm"

// Random generator menu items
#define STRING_FULL_ASCII "Toàn bộ ASCII"
#define STRING_ALPHANUMERIC "Chữ và số"
#define STRING_HEXADECIMAL "Thập lục phân"
#define STRING_DECIMAL "Thập phân"
#define STRING_BINARY "Nhị phân"
#define STRING_100_SIDED_DIE "Xúc xắc 100 mặt (0-99)"
#define STRING_20_SIDED_DIE "Xúc xắc 20 mặt"
#define STRING_12_SIDED_DIE "Xúc xắc 12 mặt"
#define STRING_10_SIDED_DIE "Xúc xắc 10 mặt (0-9)"
#define STRING_8_SIDED_DIE "Xúc xắc 8 mặt"
#define STRING_6_SIDED_DIE "Xúc xắc 6 mặt"
#define STRING_4_SIDED_DIE "Xúc xắc 4 mặt"
#define STRING_COIN_FLIP "Tung đồng xu"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Mặt sấp"
#define STRING_COIN_TAIL "Mặt ngửa"

// Game menu items
#define STRING_PLAY_WHITE "Chơi trắng"
#define STRING_PLAY_BLACK "Chơi đen"
#define STRING_CONTINUE_GAME "Tiếp tục trò chơi"
#define STRING_NEW_GAME "Trò chơi mới"
#define STRING_STRENGTH "Độ khó"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Cấp độ"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Thời gian ống"
#define STRING_TUBE_LIFE_PULSES "Xung"
#define STRING_TUBE_DEAD_TIME "Thời gian trễ"
#define STRING_DEVICE_ID "ID thiết bị"
#define STRING_DEVICE_VOLTAGE "Điện áp"

#define STRING_TUBE "Ống"
#define STRING_LIFE_TIME "Thời gian"
#define STRING_LIFE_PULSES "Xung"
#define STRING_DEAD_TIME "Thời gian trễ"
#define STRING_DEVICE "Thiết bị"
#define STRING_ID "ID"
#define STRING_VOLTAGE "Điện áp"
