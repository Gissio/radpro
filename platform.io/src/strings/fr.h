/*
 * Rad Pro
 * UI strings: French
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
#define STRING_COUNT "imp."
#define STRING_COUNTS "imp."

// Common time menu items
#define STRING_24_HOURS "24 heures"
#define STRING_12_HOURS "12 heures"
#define STRING_6_HOURS "6 heures"
#define STRING_3_HOURS "3 heures"
#define STRING_1_HOUR "1 heure"
#define STRING_30_MINUTES "30 minutes"
#define STRING_10_MINUTES "10 minutes"
#define STRING_5_MINUTES "5 minutes"
#define STRING_2_MINUTES "2 minutes"
#define STRING_1_MINUTE "1 minute"
#define STRING_30_SECONDS "30 secondes"
#define STRING_10_SECONDS "10 secondes"
#define STRING_5_SECONDS "5 secondes"
#define STRING_1_SECOND "1 seconde"

// Common menu items
#define STRING_OFF "Éteint"

// Sound volume and display brightness menu items
#define STRING_VERY_LOW "Très faible"
#define STRING_LOW "Faible"
#define STRING_MEDIUM "Moyen"
#define STRING_HIGH "Élevé"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "AVERTISSEMENT\n\nÉchec de la vérification du micrologiciel."

// HV profile menu items
#define STRING_NOTIFICATION_HVCUSTOM "AVERTISSEMENT\n\nValeurs incorrectes peuvent endommager l'appareil."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Réinitialiser le journal de données ?\n\nTous les enregistrements seront supprimés."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCCÈS\n\nLe journal de données a été réinitialisé."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Instantané"
#define STRING_AVERAGE "Moyen"
#define STRING_CUMULATIVE "Cumulatif"
#define STRING_HISTORY "Historique"

// Measurement history view time periods
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 j"
#define STRING_HISTORY_1_WEEK "1 sem"
#define STRING_HISTORY_1_MONTH "1 m"
#define STRING_HISTORY_1_YEAR "1 a"

// Measurement view secondary view strings
#define STRING_TIME "Temps"
#define STRING_MAX "Max"
#define STRING_RATE "Taux"
#define STRING_DOSE "Dose"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_ALERT_ALARM "ALARME"
#define STRING_ALERT_WARNING "AVERTISSEMENT"
#define STRING_ALERT_FAULT "DÉFAUT"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_OVERFLOW "DÉPASSEMENT"
#define STRING_ALERT_DONE "TERMINÉ"

// Settings menu items
#define STRING_SETTINGS "Paramètres"
#define STRING_PULSES "Impulsions"
#define STRING_ALERTS "Alertes"
#define STRING_MEASUREMENTS "Mesures"
#define STRING_GEIGER_TUBE "Tube Geiger"
#define STRING_DATALOG "Journal de données"
#define STRING_DISPLAY "Affichage"
#define STRING_DATE_AND_TIME "Date et heure"
#define STRING_BATTERY_TYPE "Type de batterie"
#define STRING_RANDOM_GENERATOR "Générateur aléatoire"
#define STRING_GAME "Jeu"
#define STRING_STATISTICS "Statistiques"
#define STRING_DATAMODE "Mode données"

// Pulses menu items
#define STRING_SOUND "Son"
#define STRING_VOICE "Voix"
#define STRING_VIBRATION "Vibration"
#define STRING_PULSE_LED "LED impulsion"
#define STRING_DISPLAY_FLASH "Flash affichage"
#define STRING_THRESHOLD "Seuil"

// Alarms menu items
#define STRING_RATE_WARNING "Avert. de taux"
#define STRING_RATE_ALARM "Alarme de taux"
#define STRING_DOSE_WARNING "Avert. de dose"
#define STRING_DOSE_ALARM "Alarme de dose"
#define STRING_INDICATION "Indication"

// Measurements menu items
#define STRING_UNITS "Unités"
#define STRING_SECONDARY_UNITS "Unités secondaires"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Adaptatif rapide"
#define STRING_ADAPTIVE_PRECISION "Adaptatif précis"

// Average menu items
#define STRING_UNLIMITED "Illimité"
#define STRING_50_CONFIDENCE "±50% confiance"
#define STRING_20_CONFIDENCE "±20% confiance"
#define STRING_10_CONFIDENCE "±10% confiance"
#define STRING_5_CONFIDENCE "±5% confiance"
#define STRING_2_CONFIDENCE "±2% confiance"
#define STRING_1_CONFIDENCE "±1% confiance"

// Geiger tube menu items
#define STRING_SENSITIVITY "Sensibilité"
#define STRING_DEAD_TIME_COMPENSATION "Comp. temps mort"
#define STRING_HVPROFILE "Profil HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (défaut)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (should only be translated with cyrillic scripts)
#define STRING_SBM20 "SBM-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Par défaut usine"
#define STRING_ACCURACY "Précision"
#define STRING_ENERGY_SAVING "Économie énergie"
#define STRING_CUSTOM "Personnalisé"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Fréquence PWM"
#define STRING_PWMDUTY_CYCLE "Cycle travail PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Mode journalisation"
#define STRING_RESET "Réinitialiser"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Toutes les heures"
#define STRING_EVERY10_MINUTES "Toutes les 10 minutes"
#define STRING_EVERY_MINUTE "Chaque minute"
#define STRING_EVERY10_SECONDS "Toutes les 10 secondes"
#define STRING_EVERY_SECOND "Chaque seconde"

// Display menu items
#define STRING_THEME "Thème"
#define STRING_BRIGHTNESS "Luminosité"
#define STRING_CONTRAST "Contraste"
#define STRING_SLEEP "Veille"

// Display - Theme menu items
#define STRING_THEME_DAY "Jour"
#define STRING_THEME_DUSK "Crépuscule"
#define STRING_THEME_NIGHT "Nuit"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Niveau"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Toujours éteint"
#define STRING_ALWAYS_ON "Toujours allumé"

// Sound menu items
#define STRING_ALERTSTYLE "Style d'alerte"
#define STRING_ALERTVOLUME "Volume de l'alerte"
#define STRING_VOICEVOLUME "Volume de la voix"

// Sound Alarms length
#define STRING_SHORT "Court"
#define STRING_LONG "Long"

// Sound pulses menu items
#define STRING_CLICKS "Clics"
#define STRING_CHIRPS "Chirps"
#define STRING_BEEPS "Bips"

// Time and date menu items
#define STRING_TIME_ZONE "Fuseau horaire"
#define STRING_YEAR "Année"
#define STRING_MONTH "Mois"
#define STRING_DAY "Jour"
#define STRING_HOUR "Heure"
#define STRING_MINUTE "Minute"
#define STRING_TIME_FORMAT "Format heure"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time and date > Time format menu items
#define STRING_24_HOUR "24 heures"
#define STRING_12_HOUR "12 heures"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Alcaline"

// Random generator menu items
#define STRING_ASCII "ASCII"
#define STRING_ALPHANUMERIC "Alphanumérique"
#define STRING_HEXADECIMAL "Hexadécimal"
#define STRING_DECIMAL "Décimal"
#define STRING_BINARY "Binaire"
#define STRING_100_SIDED_DIE "Dé 100 faces (0-99)"
#define STRING_20_SIDED_DIE "Dé 20 faces"
#define STRING_12_SIDED_DIE "Dé 12 faces"
#define STRING_10_SIDED_DIE "Dé 10 faces (0-9)"
#define STRING_8_SIDED_DIE "Dé 8 faces"
#define STRING_6_SIDED_DIE "Dé 6 faces"
#define STRING_4_SIDED_DIE "Dé 4 faces"
#define STRING_COIN_FLIP "Lancer pièce (0-1)"

// Game menu items
#define STRING_PLAY_WHITE "Jouer blanc"
#define STRING_PLAY_BLACK "Jouer noir"
#define STRING_CONTINUE_GAME "Continuer partie"
#define STRING_NEW_GAME "Nouvelle partie"
#define STRING_STRENGTH "Force"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Niveau"

// Statistics view items
#define STRING_TUBE_LIFE "Durée vie tube"
#define STRING_TUBE "Tube"
#define STRING_LIFE "Durée vie"
#define STRING_DEAD_TIME "Temps mort"
#define STRING_DEVICE_ID "ID de l'appareil"
#define STRING_DEVICE "Appareil"
#define STRING_ID "ID"
#define STRING_BATTERY "Batterie"
