/*
 * Rad Pro
 * UI strings: French
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
#define STRING_W_M2 "W/m²"

// Units for indicating Geiger tube pulse count, as in "1 count" or "14 counts"
#define STRING_COUNT "imp."
#define STRING_COUNTS "imp."

// Time strings
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

// Strength strings
#define STRING_OFF "Éteint"
#define STRING_VERY_LOW "Très faible"
#define STRING_LOW "Faible"
#define STRING_MEDIUM "Moyen"
#define STRING_HIGH "Élevé"

// Firmware checksum failure notification
#define STRING_SPLASH_FIRMWARE_CHECKSUM_FAILURE "AVERTISSEMENT\n\nÉchec de la vérification du micrologiciel."

// Custom HV profile notification
#define STRING_NOTIFICATION_HVCUSTOM "AVERTISSEMENT\n\nValeurs incorrectes peuvent endommager l'appareil."

// Data log reset notification
#define STRING_NOTIFICATION_DATALOG_RESET_CONFIRM "Réinitialiser le journal de données ?\n\nTous les enregistrements seront supprimés."

// Data log reset success notification
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "SUCCÈS\n\nLe journal de données a été réinitialisé."

// Measurements title strings
#define STRING_INSTANTANEOUS "Instantané"
#define STRING_AVERAGE "Moyen"
#define STRING_CUMULATIVE "Cumulatif"
#define STRING_HISTORY "Historique"
#define STRING_ELECTRIC_FIELD "Champ électrique"
#define STRING_MAGNETIC_FIELD "Champ magnétique"

// Measurements secondary view strings
#define STRING_TIME "Temps"
#define STRING_MAX "Max"
#define STRING_RATE "Taux"
#define STRING_DOSE "Dose"
#define STRING_POWER_DENSITY "Densité de puissance"

// Measurements info strings
#define STRING_ALERT_FAULT "DÉFAUT"
#define STRING_ALERT_MAX "MAX"
#define STRING_ALERT_DONE "TERMINÉ"
#define STRING_ALERT_UNAVAILABLE "INDISPONIBLE"

// History period strings
#define STRING_HISTORY_10_MINUTES "10 min"
#define STRING_HISTORY_1_HOUR "1 h"
#define STRING_HISTORY_1_DAY "1 j"
#define STRING_HISTORY_1_WEEK "1 sem"
#define STRING_HISTORY_1_MONTH "1 m"
#define STRING_HISTORY_1_YEAR "1 a"

// Settings menu items
#define STRING_SETTINGS "Paramètres"
#define STRING_PULSES "Impulsions"
#define STRING_ALERTS "Alertes"
#define STRING_MEASUREMENTS "Mesures"
#define STRING_GEIGER_TUBE "Tube Geiger"
#define STRING_DATALOG "Journal de données"
#define STRING_DISPLAY "Affichage"
#define STRING_DATE_AND_TIME "Date et heure"
#define STRING_POWER "Alimentation"
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

// Alerts menu items
#define STRING_INDICATION "Indication"
#define STRING_RATE_WARNING "Avert. de taux"
#define STRING_RATE_ALARM "Alarme de taux"
#define STRING_DOSE_WARNING "Avert. de dose"
#define STRING_DOSE_ALARM "Alarme de dose"
#define STRING_ELECTRIC_FIELD_ALARM "Alarme de champ électrique"
#define STRING_MAGNETIC_FIELD_ALARM "Alarme de champ magnétique"

// Measurements menu items
#define STRING_SOURCE "Source"
#define STRING_DOSE_UNITS "Unités de dose"
#define STRING_SECONDARY_DOSE_UNITS "Unités de dose secondaires"
#define STRING_MAGNETIC_FIELD_UNITS "Unités de champ magnétique"

// Source menu items
#define STRING_CS137 "Par défaut : Cs-137 (γ)"
#define STRING_CO60 "Co-60 (γ)"
#define STRING_TC99M "Tc-99m (γ)"
#define STRING_I131 "I-131 (γ)"
#define STRING_LU177 "Lu-177 (γ)"
#define STRING_AM241 "Am-241 (γ)"
#define STRING_RADIUM "Radium (γ)"
#define STRING_URANIUM_ORE "Minerai d'uranium (γ)"
#define STRING_URANIUM_GLASS "Ouraline (γ)"
#define STRING_DEPLETED_URANIUM "Uranium appauvri (γ)"
#define STRING_THORIUM_ORE "Minerai de thorium (γ)"
#define STRING_XRAYS "Rayons X (60 kV)"
#define STRING_K40 "K-40 (γ)"
#define STRING_BACKGROUND_RADIATION "Rayonnement de fond"

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
#define STRING_TUBE_TYPE "Type de tube"
#define STRING_SENSITIVITY "Sensibilité (Cs-137)"
#define STRING_DEAD_TIME_COMPENSATION "Comp. temps mort"
#define STRING_HVPROFILE "Profil HV"

// Geiger tube type strings
#define STRING_J305 "J305 (107 mm)"
#define STRING_M4011 "M4011 (87 mm)"
#define STRING_HH614 "HH614"
#define STRING_SBM20 "SBM20"
#define STRING_SI3BG "SI3BG"
#define STRING_LND7317 "LND 7317"

// Geiger tube sensitivity strings
#define STRING_TUBE_DEFAULT "Tube par défaut"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Par défaut usine"
#define STRING_ACCURACY "Précision"
#define STRING_ENERGY_SAVING "Économie énergie"
#define STRING_CUSTOM "Personnalisé"

// Custom HV profile menu items
#define STRING_PWMFREQUENCY "Fréquence PWM"
#define STRING_PWMDUTY_CYCLE "Cycle travail PWM"

// Data log menu items
#define STRING_LOGGING_MODE "Mode journalisation"
#define STRING_RESET "Réinitialiser"

// Data log options menu items
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

// Display theme menu items
#define STRING_THEME_DAY "Jour"
#define STRING_THEME_DUSK "Crépuscule"
#define STRING_THEME_NIGHT "Nuit"

// Display contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Niveau"

// Display sleep menu items
#define STRING_ALWAYS_OFF "Toujours éteint"
#define STRING_ALWAYS_ON "Toujours allumé"

// Sound menu items
#define STRING_ALERTSTYLE "Style d'alerte"
#define STRING_ALERTVOLUME "Volume de l'alerte"
#define STRING_VOICEVOLUME "Volume de la voix"

// Sound alert style menu items
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

// Time zone menu items
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Hour menu items
#define STRING_AM "AM"
#define STRING_PM "PM"

// Time format menu items
#define STRING_24_HOUR "24 heures"
#define STRING_12_HOUR "12 heures"

// Power menu items
#define STRING_BATTERY "Batterie"
#define STRING_USB_AUTO_POWER_ON "Mise sous tension auto USB"

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
