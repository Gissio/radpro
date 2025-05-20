/*
 * Rad Pro
 * UI strings: Greek
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

// Application name (transliterated for non-Latin scripts)
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
#define STRING_COUNT "παλμός"
#define STRING_COUNTS "παλμοί"

// Common time menu items
#define STRING_24_HOURS "24 ώρες"
#define STRING_12_HOURS "12 ώρες"
#define STRING_6_HOURS "6 ώρες"
#define STRING_3_HOURS "3 ώρες"
#define STRING_1_HOUR "1 ώρα"
#define STRING_30_MINUTES "30 λεπτά"
#define STRING_10_MINUTES "10 λεπτά"
#define STRING_5_MINUTES "5 λεπτά"
#define STRING_2_MINUTES "2 λεπτά"
#define STRING_1_MINUTE "1 λεπτό"
#define STRING_30_SECONDS "30 δευτερόλεπτα"
#define STRING_10_SECONDS "10 δευτερόλεπτα"
#define STRING_5_SECONDS "5 δευτερόλεπτα"
#define STRING_1_SECOND "1 δευτερόλεπτο"

// Common menu items
#define STRING_OFF "Ανενεργό"

// Notification headings (upper case)
#define STRING_NOTIFICATION_WARNING "ΠΡΟΕΙΔΟΠΟΙΗΣΗ"
#define STRING_NOTIFICATION_SUCCESS "ΕΠΙΤΥΧΙΑ"

// Firmware checksum failure notification (shorter than 30 characters)
#define STRING_NOTIFICATION_FIRMWARE_CHECKSUM_FAILURE "Σφάλμα υλικολογισμικού."

// HV profile menu items (shorter than 30 characters)
#define STRING_NOTIFICATION_HVCUSTOM "Λάθος τιμές βλάπτουν."

// Data log reset notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_TITLE "Επαναφορά;"
#define STRING_NOTIFICATION_DATALOG_RESET_SUBTITLE "Όλα τα αρχεία διαγράφονται."

// Data log reset success notification (shorter than 30 characters)
#define STRING_NOTIFICATION_DATALOG_RESET_SUCCESS "Δεδομένα επαναφέρθηκαν."

// Measurement view UI heading strings
#define STRING_INSTANTANEOUS "Στιγμιαία"
#define STRING_AVERAGE "Μέσος όρος"
#define STRING_CUMULATIVE "Σωρευτική"
#define STRING_HISTORY10_MIN "Ιστορικό (10 λεπτά)"
#define STRING_HISTORY1_H "Ιστορικό (1 ώρα)"
#define STRING_HISTORY24_H "Ιστορικό (24 ώρες)"

// Measurement view secondary view strings
#define STRING_TIME "Χρόνος"
#define STRING_MAX "Μαξ"
#define STRING_RATE "Ρυθμός"
#define STRING_DOSE "Δόση"

// Measurement view info strings (upper case, up to 5 letters)
#define STRING_INFO_ALARM "ΣΥΝΑΓ"
#define STRING_INFO_FAULT "ΣΦΑΛΜΑ"
#define STRING_INFO_MAX "ΜΑΞ"
#define STRING_INFO_OVER "ΥΠΕΡ"
#define STRING_INFO_DONE "ΟΛΟΚ"

// Settings menu items
#define STRING_SETTINGS "Ρυθμίσεις"
#define STRING_PULSES "Παλμοί"
#define STRING_ALARMS "Συναγερμοί"
#define STRING_MEASUREMENTS "Μετρήσεις"
#define STRING_GEIGER_TUBE "Σωλήνας Geiger"
#define STRING_DATA_LOG "Αρχείο δεδομένων"
#define STRING_DISPLAY "Οθόνη"
#define STRING_DATE_AND_TIME "Ημερομηνία και ώρα"
#define STRING_BATTERY_TYPE "Τύπος μπαταρίας"
#define STRING_RANDOM_GENERATOR "Γεννήτρια τυχαίων"
#define STRING_GAME "Παιχνίδι"
#define STRING_STATISTICS "Στατιστικά"
#define STRING_DATA_MODE "Λειτουργία δεδομένων"

// Pulses menu items
#define STRING_SOUND "Ήχος"
#define STRING_VIBRATION "Δόνηση"
#define STRING_ALERT_LED "LED ειδοποίησης"
#define STRING_PULSE_LED "LED παλμού"
#define STRING_DISPLAY_FLASH "Αναβόσβηση οθόνης"
#define STRING_THRESHOLD "Κατώφλι"

// Sound menu items
#define STRING_CLICKS "Κλικ"
#define STRING_CHIRPS "Κελαηδίσματα"
#define STRING_BEEPS "Μπιπ"

// Alarms menu items
#define STRING_RATE_ALARM "Συναγερμός ρυθμού"
#define STRING_DOSE_ALARM "Συναγερμός δόσης"
#define STRING_OVERRANGE_ALARM "Συναγερμός υπέρβασης"
#define STRING_INDICATION "Ένδειξη"

// Measurements menu items
#define STRING_UNITS "Μονάδες"

// Instantaneous menu items
#define STRING_ADAPTIVE_FAST "Προσαρμοστική γρήγορη"
#define STRING_ADAPTIVE_PRECISION "Προσαρμοστική ακρίβεια"

// Average menu items
#define STRING_UNLIMITED "Απεριόριστο"
#define STRING_50_CONFIDENCE "±50% εμπιστοσύνη"
#define STRING_20_CONFIDENCE "±20% εμπιστοσύνη"
#define STRING_10_CONFIDENCE "±10% εμπιστοσύνη"
#define STRING_5_CONFIDENCE "±5% εμπιστοσύνη"
#define STRING_2_CONFIDENCE "±2% εμπιστοσύνη"
#define STRING_1_CONFIDENCE "±1% εμπιστοσύνη"

// Geiger tube menu items
#define STRING_SENSITIVITY "Ευαισθησία"
#define STRING_DEAD_TIME_COMPENSATION "Αντιστάθμιση νεκρού χρόνου"
#define STRING_HVPROFILE "Προφίλ HV"

// Suffix for indicating the default Geiger tube (as short as possible)
#define STRING_DEFAULT " (προεπιλογή)"

// Chinese Geiger tubes (should not be translated)
#define STRING_J305 "J305"
#define STRING_J321 "J321"
#define STRING_J613 "J613"
#define STRING_J614 "J614"
#define STRING_M4011 "M4011"

// Russian Geiger tubes (transliterated for non-Latin scripts)
#define STRING_SBM20 "Ες Μπε Εμ-20"

// HV profile menu items
#define STRING_FACTORY_DEFAULT "Εργοστασιακή προεπιλογή"
#define STRING_ACCURACY "Ακρίβεια"
#define STRING_ENERGY_SAVING "Εξοικονόμηση ενέργειας"
#define STRING_CUSTOM "Προσαρμοσμένο"

// HV profile - custom menu items
#define STRING_PWMFREQUENCY "Συχνότητα PWM"
#define STRING_PWMDUTY_CYCLE "Κύκλος εργασίας PWM"

// Data Log menu items
#define STRING_LOGGING_MODE "Λειτουργία καταγραφής"
#define STRING_RESET "Επαναφορά"

// Data log - Logging menu items
#define STRING_EVERY_HOUR "Κάθε ώρα"
#define STRING_EVERY10_MINUTES "Κάθε 10 λεπτά"
#define STRING_EVERY_MINUTE "Κάθε λεπτό"
#define STRING_EVERY10_SECONDS "Κάθε 10 δευτερόλεπτα"
#define STRING_EVERY_SECOND "Κάθε δευτερόλεπτο"

// Display menu items
#define STRING_THEME "Θέμα"
#define STRING_BRIGHTNESS "Φωτεινότητα"
#define STRING_CONTRAST "Αντίθεση"
#define STRING_SLEEP "Αναστολή"

// Display - Theme menu items
#define STRING_THEME_DAY "Ημέρα"
#define STRING_THEME_DUSK "Λυκόφως"
#define STRING_THEME_NIGHT "Νύχτα"

// Display - Brightness menu items
#define STRING_VERY_LOW "Πολύ χαμηλή"
#define STRING_LOW "Χαμηλή"
#define STRING_MEDIUM "Μεσαία"
#define STRING_HIGH "Υψηλή"

// Display - Contrast menu items prefix (followed by a number from 1 to 8)
#define STRING_CONTRAST_LEVEL "Επίπεδο"

// Display - Sleep menu items
#define STRING_ALWAYS_OFF "Πάντα απενεργοποιημένη"
#define STRING_ALWAYS_ON "Πάντα ενεργοποιημένη"

// Time and date menu items
#define STRING_TIME_ZONE "Ζώνη ώρας"
#define STRING_YEAR "Έτος"
#define STRING_MONTH "Μήνας"
#define STRING_DAY "Ημέρα"
#define STRING_HOUR "Ώρα"
#define STRING_MINUTE "Λεπτό"
#define STRING_TIME_FORMAT "Μορφή ώρας"

// Time and date > Time zone menu items (should not be translated)
#define STRING_UTCMINUS "UTC-"
#define STRING_UTCPLUS "UTC+"

// Time and date > Hour menu items (should not be translated)
#define STRING_AM " ΠΜ"
#define STRING_PM " ΜΜ"

// Time and date > Time format menu items
#define STRING_24_HOUR "24ωρη"
#define STRING_12_HOUR "12ωρη"

// Battery type menu items
#define STRING_NI_MH "Ni-MH"
#define STRING_ALKALINE "Αλκαλική"

// Random generator menu items
#define STRING_FULL_ASCII "Πλήρες ASCII"
#define STRING_ALPHANUMERIC "Αλφαριθμητικό"
#define STRING_HEXADECIMAL "Εξαδεκαδικό"
#define STRING_DECIMAL "Δεκαδικό"
#define STRING_BINARY "Δυαδικό"
#define STRING_100_SIDED_DIE "Ζάρι 100 όψεων (0-99)"
#define STRING_20_SIDED_DIE "Ζάρι 20 όψεων"
#define STRING_12_SIDED_DIE "Ζάρι 12 όψεων"
#define STRING_10_SIDED_DIE "Ζάρι 10 όψεων (0-9)"
#define STRING_8_SIDED_DIE "Ζάρι 8 όψεων"
#define STRING_6_SIDED_DIE "Ζάρι 6 όψεων"
#define STRING_4_SIDED_DIE "Ζάρι 4 όψεων"
#define STRING_COIN_FLIP "Ρίψη νομίσματος"

// Random generator coin head/tail
#define STRING_COIN_HEAD "Κεφαλή"
#define STRING_COIN_TAIL "Ουρά"

// Game menu items
#define STRING_PLAY_WHITE "Παίξε λευκά"
#define STRING_PLAY_BLACK "Παίξε μαύρα"
#define STRING_CONTINUE_GAME "Συνέχισε παιχνίδι"
#define STRING_NEW_GAME "Νέο παιχνίδι"
#define STRING_STRENGTH "Δύναμη"

// Game strength menu items prefix (followed by a number from 1 to 8)
#define STRING_GAME_LEVEL "Επίπεδο"

// Statistics view items
#define STRING_TUBE_LIFE_TIME "Ζωή σωλήνα"
#define STRING_TUBE_LIFE_PULSES "Παλμοί"
#define STRING_TUBE_DEAD_TIME "Νεκρός χρόνος"
#define STRING_DEVICE_ID "ID συσκευής"
#define STRING_DEVICE_VOLTAGE "Τάση"

#define STRING_TUBE "Σωλήνας"
#define STRING_LIFE_TIME "Ζωή"
#define STRING_LIFE_PULSES "Παλμοί"
#define STRING_DEAD_TIME "Νεκρός χ."
#define STRING_DEVICE "Συσκευή"
#define STRING_ID "ID"
#define STRING_VOLTAGE "Τάση"
