/*
 * Rad Pro
 * UI strings: Indonesian
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
#define STRING_COUNT "cacah"
#define STRING_COUNTS "cacah"

// Time strings
#define STRING_24_HOURS "24 jam"
#define STRING_12_HOURS "12 jam"
#define STRING_6_HOURS "6 jam"
#define STRING_3_HOURS "3 jam"
#define STRING_1_HOUR "1 jam"
#define STRING_30_MINUTES "30 menit"
#define STRING_10_MINUTES "10 menit"
#define STRING_5_MINUTES "5 menit"
#define STRING_2_MINUTES "2 menit"
#define STRING_1_MINUTE "1 menit"
#define STRING_30_SECONDS "30 detik"
#define STRING_10_SECONDS "10 detik"
#define STRING_5_SECONDS "5 detik"
#define STRING_1_SECOND "1 detik"

// Strength strings
#define STRING_OFF "Mati"
#define STRING_VERY_LOW "Sangat rendah"
#define STRING_LOW "Rendah"
#define STRING_MEDIUM "Sedang"
#define STRING_HIGH "Tinggi"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "PERINGATAN\n\nKegagalan checksum firmware."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "PERINGATAN\n\nNilai yang salah dapat merusak perangkat."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Setel ulang log data?\n\nSemua rekaman akan dihapus."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUKSES\n\nLog data telah disetel ulang."

// Measurements title strings
#define STRING_INSTANTANEOUS "Seketika"
#define STRING_AVERAGE "Rata-rata"
#define STRING_CUMULATIVE "Kumulatif"
#define STRING_HISTORY "Riwayat"
#define STRING_ELECTRIC_FIELD "Medan listrik"
#define STRING_MAGNETIC_FIELD "Medan magnet"

// Measurements secondary view strings
#define STRING_TIME "Waktu"
#define STRING_MAX "Maks"
#define STRING_RATE "Laju"
#define STRING_DOSE "Dosis"

// Measurements info strings
#define STRING_ALERT_FAULT "KESALAHAN"
#define STRING_ALERT_DONE "SELESAI"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 mnt"
#define STRING_HISTORY_1_HOUR "1 jm"
#define STRING_HISTORY_1_DAY "1 hr"
#define STRING_HISTORY_1_WEEK "1 mg"
#define STRING_HISTORY_1_MONTH "1 bln"
#define STRING_HISTORY_1_YEAR "1 thn"

// Settings menu items
#define STRING_SETTINGS "Pengaturan"
#define STRING_PULSES "Pulsa"
#define STRING_ALERTS "Peringatan"
#define STRING_MEASUREMENTS "Pengukuran"
#define STRING_GEIGER_TUBE "Tabung Geiger"
#define STRING_DATALOG "Log data"
#define STRING_DISPLAY "Tampilan"
#define STRING_DATE_AND_TIME "Tanggal dan waktu"
#define STRING_POWER "Daya"
#define STRING_RANDOM_GENERATOR "Pembangkit acak"
#define STRING_GAME "Permainan"
#define STRING_STATISTICS "Statistik"
#define STRING_DATAMODE "Mode data"

// Pulses menu items
#define STRING_SOUND "Suara"
#define STRING_VOICE "Suara"
#define STRING_VIBRATION "Getaran"
#define STRING_PULSE_LED "LED pulsa"
#define STRING_DISPLAY_FLASH "Kedip tampilan"
#define STRING_THRESHOLD "Ambang"

// Alerts menu items
#define STRING_INDICATION "Indikasi"
#define STRING_RATE_WARNING "Peringatan laju"
#define STRING_RATE_ALARM "Alarm laju"
#define STRING_DOSE_WARNING "Peringatan dosis"
#define STRING_DOSE_ALARM "Alarm dosis"
#define STRING_ELECTRIC_FIELD_ALARM "Alarm medan listrik"
#define STRING_MAGNETIC_FIELD_ALARM "Alarm medan magnet"

// Measurements menu items
#define STRING_SOURCE "Sumber"
#define STRING_DOSE_UNITS "Satuan dosis"
#define STRING_SECONDARY_DOSE_UNITS "Satuan dosis sekunder"
#define STRING_MAGNETIC_FIELD_UNITS "Satuan medan magnet"

// Source menu items
#define STRING_CS137 "Bawaan: Cs-137 (γ)s"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Radium (γ)"
#define STRING_URANIUM_ORE "Bijih uranium (γ)"
#define STRING_URANIUM_GLASS "Kaca uranium (γ)"
#define STRING_DEPLETED_URANIUM "Uranium terdeplesi (γ)"
#define STRING_THORIUM_ORE "Bijih torium (γ)"
#define STRING_XRAYS "Sinar-X (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Radiasi alam"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptif cepat"
#define STRING_ADAPTIVE_PRECISION "Adaptif presisi"

// Average menu items
#define STRING_UNLIMITED "Tak terbatas"
#define STRING_50_CONFIDENCE "±50% keyakinan"
#define STRING_20_CONFIDENCE "±20% keyakinan"
#define STRING_10_CONFIDENCE "±10% keyakinan"
#define STRING_5_CONFIDENCE "±5% keyakinan"
#define STRING_2_CONFIDENCE "±2% keyakinan"
#define STRING_1_CONFIDENCE "±1% keyakinan"

// Geiger tube menu items
#define STRING_TUBE_TYPE "Jenis tabung"
#define STRING_SENSITIVITY "Sensitivitas (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Kompensasi waktu mati"
#define STRING_HVPROFILE "Profil HV"

// Geiger tube type strings
#define STRING_J305 "J305"
#define STRING_M4011 "M4011/J321"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Tabung bawaan"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Default pabrik"
#define STRING_ACCURACY "Akurasi"
#define STRING_ENERGY_SAVING "Hemat energi"
#define STRING_CUSTOM "Kustom"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "Frekuensi PWM"
#define STRING_PWMDUTY_CYCLE "Siklus tugas PWM"

// Data log menu items
#define STRING_LOGGING_MODE "Mode pencatatan"
#define STRING_RESET "Setel ulang"

// Data log options menu items
#define STRING_EVERY_HOUR "Setiap jam"
#define STRING_EVERY10_MINUTES "Setiap 10 Menit"
#define STRING_EVERY_MINUTE "Setiap Menit"
#define STRING_EVERY10_SECONDS "Setiap 10 Detik"
#define STRING_EVERY_SECOND "Setiap Detik"

// Display menu items
#define STRING_THEME "Tema"
#define STRING_BRIGHTNESS "Kecerahan"
#define STRING_CONTRAST "Kontras"
#define STRING_SLEEP "Tidur"

// Display theme menu items
#define STRING_THEME_DAY "Siang"
#define STRING_THEME_DUSK "Senja"
#define STRING_THEME_NIGHT "Malam"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Tingkat"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Selalu mati"
#define STRING_ALWAYS_ON "Selalu nyala"

// Sound menu items
#define STRING_ALERTSTYLE "Gaya peringatan"
#define STRING_ALERTVOLUME "Volume peringatan"
#define STRING_VOICEVOLUME "Volume suara"

// Sound alert style menu items
#define STRING_SHORT "Pendek"
#define STRING_LONG "Panjang"

// Sound pulses menu items
#define STRING_CLICKS "Klik"
#define STRING_CHIRPS "Cicit"
#define STRING_BEEPS "Bip"

// Time and date menu items
#define STRING_TIME_ZONE "Zona waktu"
#define STRING_YEAR "Tahun"
#define STRING_MONTH "Bulan"
#define STRING_DAY "Hari"
#define STRING_HOUR "Jam"
#define STRING_MINUTE "Menit"
#define STRING_TIME_FORMAT "Format waktu"

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24-jam"
#define STRING_12_HOUR "12-jam"

// Power menu items
#define STRING_BATTERY "Baterai"
#define STRING_USB_AUTO_POWER_ON "USB nyala otomatis"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkalin"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumerik"
#define STRING_HEXADECIMAL "Heksadesimal"
#define STRING_DECIMAL "Desimal"
#define STRING_BINARY "Biner"
#define STRING_100_SIDED_DIE "Dadu 100 sisi (0-99)"
#define STRING_20_SIDED_DIE "Dadu 20 sisi"
#define STRING_12_SIDED_DIE "Dadu 12 sisi"
#define STRING_10_SIDED_DIE "Dadu 10 sisi (0-9)"
#define STRING_8_SIDED_DIE "Dadu 8 sisi"
#define STRING_6_SIDED_DIE "Dadu 6 sisi"
#define STRING_4_SIDED_DIE "Dadu 4 sisi"
#define STRING_COIN_FLIP "Lempar koin (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Main putih"
#define STRING_PLAY_BLACK "Main hitam"
#define STRING_CONTINUE_GAME "Lanjutkan permainan"
#define STRING_NEW_GAME "Permainan baru"
#define STRING_STRENGTH "Kekuatan"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Tingkat"

// Statistics view items
#define STRING_TUBE_LIFE "Umur tabung"
#define STRING_TUBE "Tabung"
#define STRING_LIFE "Umur"
#define STRING_DEAD_TIME "Waktu mati"
#define STRING_DEVICE_ID "ID perangkat"
#define STRING_DEVICE "Perangkat"
#define STRING_ID "ID"
