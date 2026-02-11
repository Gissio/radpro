/*
 * Rad Pro
 * UI strings: Korean
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

// Application name (translated with non-latin script)
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
#define STRING_COUNT "회"
#define STRING_COUNTS "회"

// Time strings
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

// Strength strings
#define STRING_OFF "꺼짐"
#define STRING_VERY_LOW "매우 낮음"
#define STRING_LOW "낮음"
#define STRING_MEDIUM "중간"
#define STRING_HIGH "높음"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "경고\n\n펌웨어 체크섬 실패."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "경고\n\n잘못된 값은 장치를 손상시킬 수 있습니다."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "데이터 로그를 초기화하시겠습니까?\n\n모든 기록이 삭제됩니다."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "성공\n\n데이터 로그가 초기화되었습니다."

// Measurements title strings
#define STRING_INSTANTANEOUS "즉시"
#define STRING_AVERAGE "평균"
#define STRING_CUMULATIVE "누적"
#define STRING_HISTORY "기록"
#define STRING_ELECTRIC_FIELD "전기장"
#define STRING_MAGNETIC_FIELD "자기장"

// Measurements secondary view strings
#define STRING_TIME "시간"
#define STRING_MAX "최대"
#define STRING_RATE "속도"
#define STRING_DOSE "선량"

// Measurements info strings
#define STRING_ALERT_FAULT "고장"
#define STRING_ALERT_MAX "최대"
#define STRING_ALERT_DONE "완료"
#define STRING_ALERT_UNAVAILABLE "사용 불가"

// History period strings
#define STRING_HISTORY_10_MINUTES "10분"
#define STRING_HISTORY_1_HOUR "1시간"
#define STRING_HISTORY_1_DAY "1일"
#define STRING_HISTORY_1_WEEK "1주"
#define STRING_HISTORY_1_MONTH "1개월"
#define STRING_HISTORY_1_YEAR "1년"

// Settings menu items
#define STRING_SETTINGS "설정"
#define STRING_PULSES "펄스"
#define STRING_ALERTS "알림"
#define STRING_MEASUREMENTS "측정"
#define STRING_GEIGER_TUBE "가이거 튜브"
#define STRING_DATALOG "데이터 로그"
#define STRING_DISPLAY "디스플레이"
#define STRING_DATE_AND_TIME "날짜 및 시간"
#define STRING_POWER "전원"
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

// Alerts menu items
#define STRING_INDICATION "표시"
#define STRING_RATE_WARNING "속도 경고"
#define STRING_RATE_ALARM "속도 경보"
#define STRING_DOSE_WARNING "선량 경고"
#define STRING_DOSE_ALARM "선량 경보"
#define STRING_ELECTRIC_FIELD_ALARM "전기장 경보"
#define STRING_MAGNETIC_FIELD_ALARM "자기장 경보"

// Measurements menu items
#define STRING_SOURCE "선원"
#define STRING_DOSE_UNITS "선량 단위"
#define STRING_SECONDARY_DOSE_UNITS "보조 선량 단위"
#define STRING_MAGNETIC_FIELD_UNITS "자기장 단위"

// Source menu items
#define STRING_CS137 "기본값: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "라듐 (γ)"
#define STRING_URANIUM_ORE "우라늄 광석 (γ)"
#define STRING_URANIUM_GLASS "우라늄 유리 (γ)"
#define STRING_DEPLETED_URANIUM "감손 우라늄 (γ)"
#define STRING_THORIUM_ORE "토륨 광석 (γ)"
#define STRING_XRAYS "엑스선 (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "자연방사선"

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
#define STRING_TUBE_TYPE "튜브 유형"
#define STRING_SENSITIVITY "감도 (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "데드 타임 보상"
#define STRING_HVPROFILE "HV 프로파일"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011/J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "튜브 기본값"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "공장 기본값"
#define STRING_ACCURACY "정확도"
#define STRING_ENERGY_SAVING "에너지 절약"
#define STRING_CUSTOM "사용자 지정"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "PWM 주파수"
#define STRING_PWMDUTY_CYCLE "PWM 듀티 사이클"

// Data log menu items
#define STRING_LOGGING_MODE "로깅 모드"
#define STRING_RESET "초기화"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "낮"
#define STRING_THEME_DUSK "황혼"
#define STRING_THEME_NIGHT "밤"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "레벨"

// Display sleep menu items
#define STRING_ALWAYS_OFF "항상 꺼짐"
#define STRING_ALWAYS_ON "항상 켜짐"

// Sound menu items
#define STRING_ALERTSTYLE "알림 스타일"
#define STRING_ALERTVOLUME "알림 음량"
#define STRING_VOICEVOLUME "음성 음량"

// Sound alert style menu items
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

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24시간"
#define STRING_12_HOUR "12시간"

// Power menu items
#define STRING_BATTERY "배터리"
#define STRING_USB_AUTO_POWER_ON "USB 자동 전원 켜기"

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
