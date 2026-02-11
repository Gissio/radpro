/*
 * Rad Pro
 * UI strings: Latvian
 *
 * (C) 2026 Oskars Galanders
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
#define STRING_COUNT "imp."
#define STRING_COUNTS "imp."

// Common time menu items
#define STRING_24_HOURS "24 stundas"
#define STRING_12_HOURS "12 stundas"
#define STRING_6_HOURS "6 stundas"
#define STRING_3_HOURS "3 stundas"
#define STRING_1_HOUR "1 stunda"
#define STRING_30_MINUTES "30 minūtes"
#define STRING_10_MINUTES "10 minūtes"
#define STRING_5_MINUTES "5 minūtes"
#define STRING_2_MINUTES "2 minūtes"
#define STRING_1_MINUTE "1 minūte"
#define STRING_30_SECONDS "30 sekundes"
#define STRING_10_SECONDS "10 sekundes"
#define STRING_5_SECONDS "5 sekundes"
#define STRING_1_SECOND "1 sekunde"

// Strength strings
#define STRING_OFF "Izslēgts"
#define STRING_VERY_LOW "Ļoti zems"
#define STRING_LOW "Zems"
#define STRING_MEDIUM "Vidējs"
#define STRING_HIGH "Augsts"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "BRĪDINĀJUMS\n\nProgrammaparatūras kontrolsummas kļūda."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "BRĪDINĀJUMS\n\nNepareizas vērtības var sabojāt ierīci."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Atiestatīt datu žurnālu?\n\nVisi ieraksti tiks dzēsti."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "GATAVS\n\nDatu žurnāls ir atiestatīts."

// Measurements title strings
#define STRING_INSTANTANEOUS "Pašreizējais"
#define STRING_AVERAGE "Vidējais"
#define STRING_CUMULATIVE "Kumulatīvais"
#define STRING_HISTORY "Vēsturiskais"
#define STRING_ELECTRIC_FIELD "Elektriskais lauks"
#define STRING_MAGNETIC_FIELD "Magnētiskais lauks"

// Measurements secondary view strings
#define STRING_TIME "Laiks"
#define STRING_MAX "Maks"
#define STRING_RATE "Dozas jauda"
#define STRING_DOSE "Deva"

// Measurements info strings
#define STRING_ALERT_FAULT "KĻŪDA"
#define STRING_ALERT_MAX "MAKS"
#define STRING_ALERT_DONE "GATAVS"
#define STRING_ALERT_UNAVAILABLE "NAV PIEEJAMS"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 st"
#define STRING_HISTORY_1_DAY "1 d"
#define STRING_HISTORY_1_WEEK "1 ned"
#define STRING_HISTORY_1_MONTH "1 mēn"
#define STRING_HISTORY_1_YEAR "1 g"

// Settings menu items
#define STRING_SETTINGS "Iestatījumi"
#define STRING_PULSES "Impulsi"
#define STRING_ALERTS "Brīdinājumi"
#define STRING_MEASUREMENTS "Mērījumi"
#define STRING_GEIGER_TUBE "Geigera caurule"
#define STRING_DATALOG "Datu žurnāls"
#define STRING_DISPLAY "Ekrāns"
#define STRING_DATE_AND_TIME "Datums un laiks"
#define STRING_POWER "Barošana"
#define STRING_RANDOM_GENERATOR "Nejaušo skaitļu ģenerators"
#define STRING_GAME "Spēle"
#define STRING_STATISTICS "Statistika"
#define STRING_DATAMODE "Datu režīms"

// Pulses menu items
#define STRING_SOUND "Skaņa"
#define STRING_VOICE "Balss"
#define STRING_VIBRATION "Vibrācija"
#define STRING_PULSE_LED "Pulsējošs LED"
#define STRING_DISPLAY_FLASH "Zibsnīgs ekrāns"
#define STRING_THRESHOLD "Slieksnis"

// Alerts menu items
#define STRING_INDICATION "Indikācija"
#define STRING_RATE_WARNING "Dozas jaudas brīdinājums"
#define STRING_RATE_ALARM "Dozas jaudas trauksme"
#define STRING_DOSE_WARNING "Devas brīdinājums"
#define STRING_DOSE_ALARM "Devas trauksme"
#define STRING_ELECTRIC_FIELD_ALARM "Elektriskā lauka trauksme"
#define STRING_MAGNETIC_FIELD_ALARM "Magnētiskā lauka trauksme"

// Measurements menu items
#define STRING_SOURCE "Avots"
#define STRING_DOSE_UNITS "Devas vienības"
#define STRING_SECONDARY_DOSE_UNITS "Sekundārās devas vienības"
#define STRING_MAGNETIC_FIELD_UNITS "Magnētiskā lauka vienības"

// Source menu items
#define STRING_CS137 "Noklusējums: Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Rādijs (γ)"
#define STRING_URANIUM_ORE "Urāna rūda (γ)"
#define STRING_URANIUM_GLASS "Urāna stikls (γ)"
#define STRING_DEPLETED_URANIUM "Nabadzīgais urāns (γ)"
#define STRING_THORIUM_ORE "Torija rūda (γ)"
#define STRING_XRAYS "Rentgen (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Fona starojums"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptīvs ātrs"
#define STRING_ADAPTIVE_PRECISION "Adaptīvs precīzs"

// Average menu items
#define STRING_UNLIMITED "Nav noteikts"
#define STRING_50_CONFIDENCE "±50% varbūtība"
#define STRING_20_CONFIDENCE "±20% varbūtība"
#define STRING_10_CONFIDENCE "±10% varbūtība"
#define STRING_5_CONFIDENCE "±5% varbūtība"
#define STRING_2_CONFIDENCE "±2% varbūtība"
#define STRING_1_CONFIDENCE "±1% varbūtība"

// Geiger tube menu items
#define STRING_TUBE_TYPE "Caurules tips"
#define STRING_SENSITIVITY "Jutība (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Mirušā laika kompensācija"
#define STRING_HVPROFILE "HV profils"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011/J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Caurules noklusējums"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Ražotāja noklusējums"
#define STRING_ACCURACY "Precizitāte"
#define STRING_ENERGY_SAVING "Enerģijas taupīšana"
#define STRING_CUSTOM "Pielāgoti"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "PWM frekvence"
#define STRING_PWMDUTY_CYCLE "PWM darbības cikls"

// Data log menu items
#define STRING_LOGGING_MODE "Žurnalēšanas režīms"
#define STRING_RESET "Atiestatīt"

// Data log options menu items
#define STRING_EVERY_HOUR "Katru stundu"
#define STRING_EVERY10_MINUTES "Ik pēc 10 minūtēm"
#define STRING_EVERY_MINUTE "Katru minūti"
#define STRING_EVERY10_SECONDS "Ik pēc 10 sekundēm"
#define STRING_EVERY_SECOND "Katru sekundi"

// Display menu items
#define STRING_THEME "Tēma"
#define STRING_BRIGHTNESS "Spilgtums"
#define STRING_CONTRAST "Kontrasts"
#define STRING_SLEEP "Miega režīms"

// Display theme menu items
#define STRING_THEME_DAY "Diena"
#define STRING_THEME_DUSK "Krēsla"
#define STRING_THEME_NIGHT "Nakts"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Līmenis"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Vienmēr izslēgts"
#define STRING_ALWAYS_ON "Vienmēr ieslēgts"

// Sound menu items
#define STRING_ALERTSTYLE "Brīdinājuma veids"
#define STRING_ALERTVOLUME "Brīdinājuma skaļums"
#define STRING_VOICEVOLUME "Balss skaļums"

// Sound alert style menu items
#define STRING_SHORT "Īss"
#define STRING_LONG "Garš"

// Sound pulses menu items
#define STRING_CLICKS "Klikšķi"
#define STRING_CHIRPS "Čirksti"
#define STRING_BEEPS "Pīkstieni"

// Time and date menu items
#define STRING_TIME_ZONE "Laika zona"
#define STRING_YEAR "Gads"
#define STRING_MONTH "Mēnesis"
#define STRING_DAY "Diena"
#define STRING_HOUR "Stunda"
#define STRING_MINUTE "Minūte"
#define STRING_TIME_FORMAT "Laika formāts"

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24 stundu"
#define STRING_12_HOUR "12 stundu"

// Power menu items
#define STRING_BATTERY "Baterija"
#define STRING_USB_AUTO_POWER_ON "USB automātiskā ieslēgšana"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Sārma"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Burtciparu"
#define STRING_HEXADECIMAL "Heksadecimāls"
#define STRING_DECIMAL "Decimāls"
#define STRING_BINARY "Binārs"
#define STRING_100_SIDED_DIE "100-skaldņu kauliņš (0-99)"
#define STRING_20_SIDED_DIE "20-skaldņu kauliņš"
#define STRING_12_SIDED_DIE "12-skaldņu kauliņš"
#define STRING_10_SIDED_DIE "10-skaldņu kauliņš (0-9)"
#define STRING_8_SIDED_DIE "8-skaldņu kauliņš"
#define STRING_6_SIDED_DIE "6-skaldņu kauliņš"
#define STRING_4_SIDED_DIE "4-skaldņu kauliņš"
#define STRING_COIN_FLIP "Monētas mešana (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Spēlēt ar baltajiem"
#define STRING_PLAY_BLACK "Spēlēt ar melnajiem"
#define STRING_CONTINUE_GAME "Turpināt spēli"
#define STRING_NEW_GAME "Jauna spēle"
#define STRING_STRENGTH "Grūtība"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Pakāpe"

// Statistics view items
#define STRING_TUBE_LIFE "Caurules darbības laiks"
#define STRING_TUBE "Caurule"
#define STRING_LIFE "Darbības laiks"
#define STRING_DEAD_TIME "Mirušais laiks"
#define STRING_DEVICE_ID "Ierīces ID"
#define STRING_DEVICE "Ierīce"
#define STRING_ID "ID"
