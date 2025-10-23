/*
 * Rad Pro
 * UI strings: Korean
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

// Application name (translated with non-latin script)
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
#define STRING_COUNT "회"
#define STRING_COUNTS "회"

// Common time menu items
#define STRING_24_HOURS "24시간"
#define STRING_12_HOURS "12시간"
#define STRING_6_HOURS "6시간"
#define STRING_3_HOURS "3시간"
#define STRING_1_HOUR "1시간"
#define STRING_30_MINUTES "30분"
#define STRING_10_MINUTES "10분"
#define STRING_5_MINUTES "5분"
#define STRING_2_MINUTES "2분"
#define STRING_1_MINUTE "1분"
#define STRING_30_SECONDS "30초"
#define STRING_10_SECONDS "10초"
#define STRING_5_SECONDS "5초"
#define STRING_1_SECOND "1초"

// Common menu items
#define STRING_OFF "꺼짐"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "매우 낮음"
#define STRING_LOW "낮음"
#define STRING_MEDIUM "중간"
#define STRING_HIGH "높음"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING ""
#define STRING_NOTIFICATION_SUCCESS ""

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "경고\n\n펌웨어 체크섬 실패."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "경고\n\n잘못된 값은 장치를 손상시킬 수 있습니다."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "데이터 로그를 초기화하시겠습니까?\n\n모든 기록이 삭제됩니다."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "성공\n\n데이터 로그가 초기화되었습니다."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "즉시"
#define STRING_AVERAGE "평균"
#define STRING_CUMULATIVE "누적"
#define STRING_HISTORY "기록"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10분"
#define STRING_HISTORY_1_HOUR "1시간"
#define STRING_HISTORY_1_DAY "1일"
#define STRING_HISTORY_1_WEEK "1주"
#define STRING_HISTORY_1_MONTH "1개월"
#define STRING_HISTORY_1_YEAR "1년"

// Measurement view secondary view strings
#define STRING_TIME "시간"
#define STRING_MAX "최대"
#define STRING_RATE "속도"
#define STRING_DOSE "선량"

// Measurement view info strings
#define STRING_ALERT_ALARM "경보"
#define STRING_ALERT_WARNING "경고"
#define STRING_ALERT_FAULT "고장"
#define STRING_ALERT_MAX "최대"
#define STRING_ALERT_DONE "완료"

// Settings menu items
#define STRING_SETTINGS "설정"
#define STRING_PULSES "펄스"
#define STRING_ALERTS "알림"
#define STRING_MEASUREMENTS "측정"
#define STRING_GEIGER_TUBE "가이거 튜브"
#define STRING_DATALOG "데이터 로그"
#define STRING_DISPLAY "디스플레이"
#define STRING_DATE_AND_TIME "날짜 및 시간"
#define STRING_BATTERY_TYPE "배터리 유형"
#define STRING_RANDOM_GENERATOR "랜덤 생성기"
#define STRING_GAME "게임"
#define STRING_STATISTICS "통계"
#define STRING_DATAMODE "데이터 모드"

// Pulses menu items
#define STRING_SOUND "소리"
#define STRING_VOICE "음성"
#define STRING_VIBRATION "진동"
#define STRING_PULSE_LED "펄스 LED"
#define STRING_DISPLAY_FLASH "디스플레이 플래시"
#define STRING_THRESHOLD "임계값"

// Alarms menu items
#define STRING_RATE_WARNING "속도 경고"
#define STRING_RATE_ALARM "속도 경보"
#define STRING_DOSE_WARNING "선량 경고"
#define STRING_DOSE_ALARM "선량 경보"
#define STRING_INDICATION "표시"

// Measurements menu items
#define STRING_UNITS "단위"
#define STRING_SECONDARY_UNITS "보조 단위"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "적응형 빠름"
#define STRING_ADAPTIVE_PRECISION "적응형 정밀"

// Average menu items
#define STRING_UNLIMITED "무제한"
#define STRING_50_CONFIDENCE "±50% 신뢰도"
#define STRING_20_CONFIDENCE "±20% 신뢰도"
#define STRING_10_CONFIDENCE "±10% 신뢰도"
#define STRING_5_CONFIDENCE "±5% 신뢰도"
#define STRING_2_CONFIDENCE "±2% 신뢰도"
#define STRING_1_CONFIDENCE "±1% 신뢰도"

// Geiger tube menu items
#define STRING_SENSITIVITY "감도"
#define STRING_DEAD_TIME_COMPENSATION "데드 타임 보상"
#define STRING_HVPROFILE "HV 프로파일"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (기본)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (translated with non-latin script)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "공장 기본값"
#define STRING_ACCURACY "정확도"
#define STRING_ENERGY_SAVING "에너지 절약"
#define STRING_CUSTOM "사용자 지정"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM 주파수"
#define STRING_PWMDUTY_CYCLE "PWM 듀티 사이클"

// Data Log menu items
#define STRING_LOGGING_MODE "로깅 모드"
#define STRING_RESET "초기화"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "매시간"
#define STRING_EVERY10_MINUTES "10분마다"
#define STRING_EVERY_MINUTE "1분마다"
#define STRING_EVERY10_SECONDS "10초마다"
#define STRING_EVERY_SECOND "1초마다"

// Display menu items
#define STRING_THEME "테마"
#define STRING_BRIGHTNESS "밝기"
#define STRING_CONTRAST "대비"
#define STRING_SLEEP "절전"

// Display - Theme menu items
#define STRING_THEME_DAY "낮"
#define STRING_THEME_DUSK "황혼"
#define STRING_THEME_NIGHT "밤"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "레벨"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "항상 꺼짐"
#define STRING_ALWAYS_ON "항상 켜짐"

// Sound menu items
#define STRING_ALERTSTYLE "알림 스타일"
#define STRING_ALERTVOLUME "알림 음량"
#define STRING_VOICEVOLUME "음성 음량"

// Sound Alarms length
#define STRING_SHORT "짧은"
#define STRING_LONG "긴"

// Sound pulses menu items
#define STRING_CLICKS "클릭"
#define STRING_CHIRPS "짹짹"
#define STRING_BEEPS "삐"

// Time and date menu items
#define STRING_TIME_ZONE "시간대"
#define STRING_YEAR "년"
#define STRING_MONTH "월"
#define STRING_DAY "일"
#define STRING_HOUR "시"
#define STRING_MINUTE "분"
#define STRING_TIME_FORMAT "시간 형식"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24시간"
#define STRING_12_HOUR "12시간"

// Battery type menu items
#define STRING_NI_MH "니켈-금속수소"
#define STRING_ALKALINE "알카라인"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "알파벳+숫자"
#define STRING_HEXADECIMAL "16진수"
#define STRING_DECIMAL "10진수"
#define STRING_BINARY "2진수"
#define STRING_100_SIDED_DIE "100면 주사위 (0-99)"
#define STRING_20_SIDED_DIE "20면 주사위"
#define STRING_12_SIDED_DIE "12면 주사위"
#define STRING_10_SIDED_DIE "10면 주사위 (0-9)"
#define STRING_8_SIDED_DIE "8면 주사위"
#define STRING_6_SIDED_DIE "6면 주사위"
#define STRING_4_SIDED_DIE "4면 주사위"
#define STRING_COIN_FLIP "동전 던지기 (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "흰색 플레이"
#define STRING_PLAY_BLACK "검은색 플레이"
#define STRING_CONTINUE_GAME "게임 계속"
#define STRING_NEW_GAME "새 게임"
#define STRING_STRENGTH "난이도"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "레벨"

// Statistics view items
#define STRING_TUBE_LIFE "튜브 수명"
#define STRING_TUBE "튜브"
#define STRING_LIFE "수명"
#define STRING_DEAD_TIME "데드 타임"
#define STRING_DEVICE_ID "장치 ID"
#define STRING_DEVICE "기기"
#define STRING_ID "ID"
#define STRING_BATTERY "배터리"
