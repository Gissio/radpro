/*
 * Rad Pro
 * UI strings: Danish
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
#define STRING_COUNT "tæl."
#define STRING_COUNTS "tæl."

// Time strings
#define STRING_24_HOURS "24 timer"
#define STRING_12_HOURS "12 timer"
#define STRING_6_HOURS "6 timer"
#define STRING_3_HOURS "3 timer"
#define STRING_1_HOUR "1 time"
#define STRING_30_MINUTES "30 minutter"
#define STRING_10_MINUTES "10 minutter"
#define STRING_5_MINUTES "5 minutter"
#define STRING_2_MINUTES "2 minutter"
#define STRING_1_MINUTE "1 minut"
#define STRING_30_SECONDS "30 sekunder"
#define STRING_10_SECONDS "10 sekunder"
#define STRING_5_SECONDS "5 sekunder"
#define STRING_1_SECOND "1 sekund"

// Strength strings
#define STRING_OFF "Fra"
#define STRING_VERY_LOW "Meget lav"
#define STRING_LOW "Lav"
#define STRING_MEDIUM "Mellem"
#define STRING_HIGH "Høj"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "ADVARSEL\n\nFirmware kontrolsumfejl."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "ADVARSEL\n\nForkerte værdier kan skade enheden."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Nulstil datalog?\n\nAlle optegnelser vil blive slettet."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCCES\n\nDataloggen er nulstillet."

// Measurements title strings
#define STRING_INSTANTANEOUS "Øjeblikkelig"
#define STRING_AVERAGE "Gennemsnit"
#define STRING_CUMULATIVE "Kumulativ"
#define STRING_HISTORY "Historik"
#define STRING_ELECTRIC_FIELD "Elektrisk felt"
#define STRING_MAGNETIC_FIELD "Magnetisk felt"

// Measurements secondary view strings
#define STRING_TIME "Tid"
#define STRING_MAX "Maks"
#define STRING_RATE "Hastighed"
#define STRING_DOSE "Dosis"

// Measurements info strings
#define STRING_ALERT_FAULT "FEJL"
#define STRING_ALERT_DONE "FÆRDIG"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 t"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 u"
#define STRING_HISTORY_1_MONTH "1 m"
#define STRING_HISTORY_1_YEAR "1 å"

// Settings menu items
#define STRING_SETTINGS "Indstillinger"
#define STRING_PULSES "Impulser"
#define STRING_ALERTS "Alarmer"
#define STRING_MEASUREMENTS "Målinger"
#define STRING_GEIGER_TUBE "Geiger-rør"
#define STRING_DATALOG "Datalog"
#define STRING_DISPLAY "Skærm"
#define STRING_DATE_AND_TIME "Dato og tid"
#define STRING_POWER "Strøm"
#define STRING_RANDOM_GENERATOR "Tilfældsgenerator"
#define STRING_GAME "Spil"
#define STRING_STATISTICS "Statistik"
#define STRING_DATAMODE "Datatilstand"

// Pulses menu items
#define STRING_SOUND "Lyd"
#define STRING_VOICE "Stemme"
#define STRING_VIBRATION "Vibration"
#define STRING_PULSE_LED "Puls-LED"
#define STRING_DISPLAY_FLASH "Skærmblink"
#define STRING_THRESHOLD "Tærskel"

// Alerts menu items
#define STRING_INDICATION "Indikation"
#define STRING_RATE_WARNING "Hastighedsadvarsel"
#define STRING_RATE_ALARM "Hastighedsalarm"
#define STRING_DOSE_WARNING "Dosisadvarsel"
#define STRING_DOSE_ALARM "Dosisalarm"
#define STRING_ELECTRIC_FIELD_ALARM "Alarm for elektrisk felt"
#define STRING_MAGNETIC_FIELD_ALARM "Alarm for magnetisk felt"

// Measurements menu items
#define STRING_SOURCE "Kilde"
#define STRING_DOSE_UNITS "Doseenheder"
#define STRING_SECONDARY_DOSE_UNITS "Sekundære doseenheder"
#define STRING_MAGNETIC_FIELD_UNITS "Magnetfeltenheder"

// Source menu items
#define STRING_CS137 "Standard: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Radium (γ)"
#define STRING_URANIUM_ORE "Uranmalm (γ)"
#define STRING_URANIUM_GLASS "Uranglas (γ)"
#define STRING_DEPLETED_URANIUM "Forarmet uran (γ)"
#define STRING_THORIUM_ORE "Thoriummalm (γ)"
#define STRING_XRAYS "Røntgen (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Baggrundsstråling"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptiv hurtig"
#define STRING_ADAPTIVE_PRECISION "Adaptiv præcision"

// Average menu items
#define STRING_UNLIMITED "Ubegrænset"
#define STRING_50_CONFIDENCE "±50% tillid"
#define STRING_20_CONFIDENCE "±20% tillid"
#define STRING_10_CONFIDENCE "±10% tillid"
#define STRING_5_CONFIDENCE "±5% tillid"
#define STRING_2_CONFIDENCE "±2% tillid"
#define STRING_1_CONFIDENCE "±1% tillid"

// Geiger tube menu items
#define STRING_TUBE_TYPE "Rørtype"
#define STRING_SENSITIVITY "Følsomhed (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Dødtidskompensation"
#define STRING_HVPROFILE "HV-profil"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011"
#define STRING_J321 "J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Rør standard"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Fabriksstandard"
#define STRING_ACCURACY "Nøjagtighed"
#define STRING_ENERGY_SAVING "Energibesparende"
#define STRING_CUSTOM "Brugerdefineret"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "PWM-frekvens"
#define STRING_PWMDUTY_CYCLE "PWM-arbejdsforhold"

// Data log menu items
#define STRING_LOGGING_MODE "Logtilstand"
#define STRING_RESET "Nulstil"

// Data log options menu items
#define STRING_EVERY_HOUR "Hver time"
#define STRING_EVERY10_MINUTES "Hver 10 minutter"
#define STRING_EVERY_MINUTE "Hvert minut"
#define STRING_EVERY10_SECONDS "Hvert 10 sekunder"
#define STRING_EVERY_SECOND "Hvert sekund"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Lysstyrke"
#define STRING_CONTRAST "Kontrast"
#define STRING_SLEEP "Søvn"

// Display theme menu items
#define STRING_THEME_DAY "Dag"
#define STRING_THEME_DUSK "Skumring"
#define STRING_THEME_NIGHT "Nat"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Niveau"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Altid fra"
#define STRING_ALWAYS_ON "Altid tændt"

// Sound menu items
#define STRING_ALERTSTYLE "Alarmstil"
#define STRING_ALERTVOLUME "Alarmvolumen"
#define STRING_VOICEVOLUME "Stemmevolumen"

// Sound alert style menu items
#define STRING_SHORT "Kort"
#define STRING_LONG "Lang"

// Sound pulses menu items
#define STRING_CLICKS "Klik"
#define STRING_CHIRPS "Pib"
#define STRING_BEEPS "Bip"

// Time and date menu items
#define STRING_TIME_ZONE "Tidszone"
#define STRING_YEAR "År"
#define STRING_MONTH "Måned"
#define STRING_DAY "Dag"
#define STRING_HOUR "Time"
#define STRING_MINUTE "Minut"
#define STRING_TIME_FORMAT "Tidsformat"

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24-timers"
#define STRING_12_HOUR "12-timers"

// Power menu items
#define STRING_BATTERY "Batteri"
#define STRING_USB_AUTO_POWER_ON "USB auto tænding"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkaline"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerisk"
#define STRING_HEXADECIMAL "Hexadecimal"
#define STRING_DECIMAL "Decimal"
#define STRING_BINARY "Binær"
#define STRING_100_SIDED_DIE "100-sidet terning (0-99)"
#define STRING_20_SIDED_DIE "20-sidet terning"
#define STRING_12_SIDED_DIE "12-sidet terning"
#define STRING_10_SIDED_DIE "10-sidet terning (0-9)"
#define STRING_8_SIDED_DIE "8-sidet terning"
#define STRING_6_SIDED_DIE "6-sidet terning"
#define STRING_4_SIDED_DIE "4-sidet terning"
#define STRING_COIN_FLIP "Møntkast (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Spil hvid"
#define STRING_PLAY_BLACK "Spil sort"
#define STRING_CONTINUE_GAME "Fortsæt spil"
#define STRING_NEW_GAME "Nyt spil"
#define STRING_STRENGTH "Styrke"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Niveau"

// Statistics view items
#define STRING_TUBE_LIFE "Rørlevetid"
#define STRING_TUBE "Rør"
#define STRING_LIFE "Levetid"
#define STRING_DEAD_TIME "Dødtid"
#define STRING_DEVICE_ID "Enheds-ID"
#define STRING_DEVICE "Enhed"
#define STRING_ID "ID"
