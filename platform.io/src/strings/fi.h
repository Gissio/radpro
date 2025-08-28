/*
 * Rad Pro
 * UI strings: Finnish
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
#define STRING_COUNT "pulssi"
#define STRING_COUNTS "pulssia"

// Common time menu items
#define STRING_24_HOURS "24 tuntia"
#define STRING_12_HOURS "12 tuntia"
#define STRING_6_HOURS "6 tuntia"
#define STRING_3_HOURS "3 tuntia"
#define STRING_1_HOUR "1 tunti"
#define STRING_30_MINUTES "30 minuuttia"
#define STRING_10_MINUTES "10 minuuttia"
#define STRING_5_MINUTES "5 minuuttia"
#define STRING_2_MINUTES "2 minuuttia"
#define STRING_1_MINUTE "1 minuutti"
#define STRING_30_SECONDS "30 sekuntia"
#define STRING_10_SECONDS "10 sekuntia"
#define STRING_5_SECONDS "5 sekuntia"
#define STRING_1_SECOND "1 sekunti"

// Common menu items
#define STRING_OFF "Pois"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Erittäin matala"
#define STRING_LOW "Matala"
#define STRING_MEDIUM "Keskitaso"
#define STRING_HIGH "Korkea"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "VAROITUS\n\nLaiteohjelmiston tarkistussumman epäonnistuminen."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "VAROITUS\n\nVäärät arvot voivat vahingoittaa laitetta."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Nollata dataloki?\n\nKaikki tiedot poistetaan."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "ONNISTUI\n\nDataloki on nollattu."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Hetkellinen"
#define STRING_AVERAGE "Keskiarvo"
#define STRING_CUMULATIVE "Kumulatiivinen"
#define STRING_HISTORY "Historia"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 pv"
#define STRING_HISTORY_1_WEEK "1 vk"
#define STRING_HISTORY_1_MONTH "1 kk"
#define STRING_HISTORY_1_YEAR "1 v"

// Measurement view secondary view strings
#define STRING_TIME "Aika"
#define STRING_MAX "Maks"
#define STRING_RATE "Nopeus"
#define STRING_DOSE "Annos"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_ALERT_ALARM "HÄLYTYS"
#define STRING_ALERT_WARNING "VAROITUS"
#define STRING_ALERT_FAULT "VIKA"
#define STRING_ALERT_MAX "MAKS"
#define STRING_ALERT_OVERFLOW "YLI"
#define STRING_ALERT_DONE "VALMIS"

// Settings menu items
#define STRING_SETTINGS "Asetukset"
#define STRING_PULSES "Pulssit"
#define STRING_ALERTS "Hälytykset"
#define STRING_MEASUREMENTS "Mittaukset"
#define STRING_GEIGER_TUBE "Geiger-putki"
#define STRING_DATALOG "Dataloki"
#define STRING_DISPLAY "Näyttö"
#define STRING_DATE_AND_TIME "Päiväys ja aika"
#define STRING_BATTERY_TYPE "Akun tyyppi"
#define STRING_RANDOM_GENERATOR "Satunnaisgeneraattori"
#define STRING_GAME "Peli"
#define STRING_STATISTICS "Tilastot"
#define STRING_DATAMODE "Datatila"

// Pulses menu items
#define STRING_SOUND "Ääni"
#define STRING_VOICE "Puhe"
#define STRING_VIBRATION "Tärinä"
#define STRING_PULSE_LED "Pulssi-LED"
#define STRING_DISPLAY_FLASH "Näytön vilkkuminen"
#define STRING_THRESHOLD "Kynnys"

// Alarms menu items
#define STRING_RATE_WARNING "Nopeusvaroitus"
#define STRING_RATE_ALARM "Nopeushälytys"
#define STRING_DOSE_WARNING "Annosvaroitus"
#define STRING_DOSE_ALARM "Annoshälytys"
#define STRING_INDICATION "Ilmoitus"

// Measurements menu items
#define STRING_UNITS "Yksiköt"
#define STRING_SECONDARY_UNITS "Toissijaiset yksiköt"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Mukautuva nopea"
#define STRING_ADAPTIVE_PRECISION "Mukautuva tarkkuus"

// Average menu items
#define STRING_UNLIMITED "Rajoittamaton"
#define STRING_50_CONFIDENCE "±50% luottamus"
#define STRING_20_CONFIDENCE "±20% luottamus"
#define STRING_10_CONFIDENCE "±10% luottamus"
#define STRING_5_CONFIDENCE "±5% luottamus"
#define STRING_2_CONFIDENCE "±2% luottamus"
#define STRING_1_CONFIDENCE "±1% luottamus"

// Geiger tube menu items
#define STRING_SENSITIVITY "Herkkyys"
#define STRING_DEAD_TIME_COMPENSATION "Kuolleen ajan kompensointi"
#define STRING_HVPROFILE "HV-profiili"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (oletus)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Tehdasasetus"
#define STRING_ACCURACY "Tarkkuus"
#define STRING_ENERGY_SAVING "Energiaa säästävä"
#define STRING_CUSTOM "Mukautettu"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "PWM-taajuus"
#define STRING_PWMDUTY_CYCLE "PWM-työsuhde"

// Data Log menu items
#define STRING_LOGGING_MODE "Lokitusmoodi"
#define STRING_RESET "Nollaus"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Joka tunti"
#define STRING_EVERY10_MINUTES "Joka 10 minuutti"
#define STRING_EVERY_MINUTE "Joka minuutti"
#define STRING_EVERY10_SECONDS "Joka 10 sekunti"
#define STRING_EVERY_SECOND "Joka sekunti"

// Display menu items
#define STRING_THEME "Teema"
#define STRING_BRIGHTNESS "Kirkkaus"
#define STRING_CONTRAST "Kontrasti"
#define STRING_SLEEP "Lepotila"

// Display - Theme menu items
#define STRING_THEME_DAY "Päivä"
#define STRING_THEME_DUSK "Hämärä"
#define STRING_THEME_NIGHT "Yö"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Taso"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Aina pois"
#define STRING_ALWAYS_ON "Aina päällä"

// Sound menu items
#define STRING_ALERTSTYLE "Hälytystyyli"
#define STRING_ALERTVOLUME "Hälytysäänenvoimakkuus"
#define STRING_VOICEVOLUME "Puheäänenvoimakkuus"

// Sound Alarms length
#define STRING_SHORT "Lyhyt"
#define STRING_LONG "Pitkä"

// Sound pulses menu items
#define STRING_CLICKS "Naksahdukset"
#define STRING_CHIRPS "Sirkutukset"
#define STRING_BEEPS "Piippaukset"

// Time and date menu items
#define STRING_TIME_ZONE "Aikavyöhyke"
#define STRING_YEAR "Vuosi"
#define STRING_MONTH "Kuukausi"
#define STRING_DAY "Päivä"
#define STRING_HOUR "Tunti"
#define STRING_MINUTE "Minuutti"
#define STRING_TIME_FORMAT "Aikamuoto"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24-tuntinen"
#define STRING_12_HOUR "12-tuntinen"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alkaliparisto"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alfanumeerinen"
#define STRING_HEXADECIMAL "Heksadesimaali"
#define STRING_DECIMAL "Desimaali"
#define STRING_BINARY "Binaari"
#define STRING_100_SIDED_DIE "100-tahkoinen noppa (0-99)"
#define STRING_20_SIDED_DIE "20-tahkoinen noppa"
#define STRING_12_SIDED_DIE "12-tahkoinen noppa"
#define STRING_10_SIDED_DIE "10-tahkoinen noppa (0-9)"
#define STRING_8_SIDED_DIE "8-tahkoinen noppa"
#define STRING_6_SIDED_DIE "6-tahkoinen noppa"
#define STRING_4_SIDED_DIE "4-tahkoinen noppa"
#define STRING_COIN_FLIP "Kolikonheitto (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Pelaa valkeilla"
#define STRING_PLAY_BLACK "Pelaa mustilla"
#define STRING_CONTINUE_GAME "Jatka peliä"
#define STRING_NEW_GAME "Uusi peli"
#define STRING_STRENGTH "Vahvuus"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Taso"

// Statistics view items
#define STRING_TUBE_LIFE "Putken käyttöikä"
#define STRING_TUBE "Putki"
#define STRING_LIFE "Käyttöikä"
#define STRING_DEAD_TIME "Kuollut aika"
#define STRING_DEVICE_ID "Laitteen ID"
#define STRING_DEVICE "Laite"
#define STRING_ID "ID"
#define STRING_BATTERY "Akku"
