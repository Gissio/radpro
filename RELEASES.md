# Releases

## 3.0.3 "Punctilio"

* Added Lithuanian and Indonesian translations.
* Bosean FS-5000: Enabled MCU prefetch for optimizing speed.
* Bosean FS-600: Fixed an issue which caused start screen freezes.

## 3.0.2 "Icebreaker"

* Fixed web installer update for the Bosean FS-5000.
* Resolved a problem causing devices to become unresponsive after software updates.
* Corrected a bug on the GQ GMC-800 that prevented alerts and voice output from functioning properly.

## 3.0.1 "The glow fix"

* Removed the charging indicator on the FNIRSI GC-01 due to hardware limitations preventing differentiation between USB disconnected and USB charging states.
* Resolved an issue where the battery indicator failed to display during power-off on the Bosean FS-5000.
* Improved battery indicator response time.

## 3.0 "Sundry"

* **Geiger counter compatibility:** Added support for the GQ GMC-800 Geiger counter with software-enhanced pulse stretching for reliable pulse sound at high radiation levels.
* **Multilingual support:** Expanded to 27 languages, including Bulgarian, Chinese, Croatian, Czech, Danish, Dutch, English, Finnish, French, German, Greek, Hungarian, Italian, Japanese, Korean, Norwegian, Polish, Portuguese, Romanian, Russian, Slovakian, Slovenian, Spanish, Swedish, Turkish, Ukrainian, and Vietnamese.
* **User interface improvements**
  * Introduced history views for 1-week, 1-month, and 1-year timeframes, with data loaded from the data log on startup.
  * Added rate and dose warnings, with alert dismissal preserving alert state.
  * Highlighted warning and alarm zones in the instantaneous rate bar and history view.
  * Improved alert LED indication.
  * Added user-selectable secondary units and customizable alert sound styles.
  * Added pulse sound icon for toggle feedback.
  * Lock mode turns off display.
  * Increased font size for dice roll display.
  * Rescaled UI for improved legibility.
  * Updated to OpenBridge 6.0 design system.
* **Power management enhancements**
  * Prevented USB connection from powering on a turned-off device.
  * Automatic shutdown at critically low battery levels.
  * Enhanced battery indicator: green when charging, red when low (on color displays).
  * Added battery voltage statistics for all cells.
  * Improved voltage measurement accuracy for FS2011 (GD32F150R8/GD32F103R8) and FNIRSI GC-01.
* **Measurements enhancements**
  * Expanded averaging intervals: 3, 6, 12, 24 hours, and confidence intervals of ±50%, ±20%, ±10%, ±5%, ±2% and ±1%.
  * Extended sensitivity range to 1–10,000 cpm/µSv/h for low-sensitivity Geiger tubes (e.g., SI-3BG).
  * Expanded dead-time compensation to 5-500 µs for scintillation detector compatibility.
  * Adjusted SBM-20 sensitivity to 150.5 cpm/µSv/h.
  * Implemented dynamic loss-of-count detection based on selected sensitivity.
* **Data and connectivity**
  * Enhanced `GET datalog` command with additional options and empty records to indicate the start of new logging sessions.
  * Added `SET deviceTimeZone` and `GET deviceTimeZone` commands for timezone management.
  * Introduced `RESET datalog` command to clear data logs.
  * Upgraded `radpro-tool` with timezone synchronization and inproved error handling.
* **Removed features**
  * Removed background compensation due to negligible intrinsic activity in common Geiger tubes.
  * Removed `GET tubeBackgroundCompensation` command.
* **Bug fixes**
  * Fixed history and maximum value updates for 10-second, 30-second, and 60-second averaging intervals.
  * Resolved datalog writing errors when USB-powered but turned off.
  * Fixed datalog download issue causing restarts in high-radiation conditions.
  * Corrected AM/PM display in 12-hour format settings.
  * Eliminated game flicker on portrait-oriented devices.

## 2.1.1 "The dot mender"

* **Updates**
  * Renamed **Conversion factor** to **Sensitivity** (as sensitivity is cpm/radiation rate, while conversion factor is radiation rate/cpm).
  * Renamed `GET tubeConversionFactor` to `GET tubeSensitivity`.
  * Short press of power key on 5-key devices in measurement view activates display sleep.
  * On 3-key devices, holding left and right keys in measurement view toggles pulse sounds.
  * Lock mode now accessible from any screen.
  * Die rolls and coin flips generate single outcomes per run.
  * Added support for 100-sided and 10-sided die rolls and binary random number generation.
  * History calculated only when instantaneous rate confidence interval is below 75%.
  * Automatic dose saving every 60 minutes at critically low battery levels.
* **Bug fixes**
  * Fixed missing measurement dot on color display devices.
  * Corrected battery voltage measurement on Bosean FS-5000.

## 2.1 "Delicious trifles"

* **Updates**
  * Added alert dismissal via reset measurement key.
  * Configurable alert indications: sound, vibration, pulse LED, and display flash.
  * Added alert-enabled indicator.
  * Implemented keyboard lock mode, accessible from Settings.
  * Doses below 10,000 now displayed in standard notation.
  * Added new rate and dose alarm options, average timer options, and extended dead-time compensation (20–320 µs).
  * Enabled pulse sound toggle from measurement view on supported devices.
  * Added pulse chirp sound option and extended alert sound duration.
  * Improved menu navigation and updated UI font to Open Sans for better readability.

* **Fixes**
  * Fixed unsightly flash on color displays when waking from sleep.
  * Resolved alerts sounding after shutdown while externally powered.
  * Corrected secondary rate and count view display after average timer expiration.
  * Fixed device state reset after quick power cycles.
  * Corrected 1-minute instantaneous averaging at high radiation levels.

## 2.0.3 "The sensitive update"

* Extended conversion factor range to 25-12,800 cpm/µSv/h.
*Capped pulse indication at 600 cpm/µSv/h for sensitive detectors to maintain ~1 pulse/second at background levels (no impact on measurements).
* Minor UI and performance improvements.

## 2.0.2 "The precise release"

* Improved instantaneous rate averaging algorithm and simplified response curves for J613/J614 tubes.
* Disabled instantaneous rate alarms during high confidence intervals (e.g., post-startup).
* Minor UI improvements.

## 2.0.1 "Nobody's perfect"

* Added data log reset.
* Reverted J613/J614 tube conversion factor to 60.0 cpm/µSv/h.

* Fixed custom HV profiles, `SET tubeHVFrequency`, `SET tubeHVDutyCycle`, and history data log download issues.
* Minor refinements.

## 2.0 "Grand première"

* Updated UI to [OpenBridge 5.0](https://www.openbridge.no/) for better sunlight legibility.
* Changed J613/J614 tube conversion factor to 30.0 cpm/µSv/h.
* Removed FNIRSI GC-01 display panel settings.
* Increased granularity for `SET tubeHVFrequency` (100 Hz–100 kHz) and `SET tubeHVDutyCycle` (0%–100%).

## 2.0rc5 "The final countdown"

* Added Bosean FS-5000 support and display panel selection menu.
* Added secondary dose view in average measurement mode.
* Extended conversion factor range to 25–1600 cpm/µSv/h.
* Added 1-second data log interval; removed 30-second and 30-minute options.
* Added `SET tubeHVFrequency` and `SET tubeHVDutyCycle` commands.
* Added 24/12-hour time format selection.
* Improved data mode usability and battery level indicators for FS2011, Bosean FS-600/FS-1000.
* Changed USB data connection EOL to "\r\n".

## 2.0rc4 "The little update"

* Added instantaneous rate averaging options: adaptive fast, adaptive precision, 1 minute, 30 seconds, and 10 seconds.
* Added secondary CPM and count displays.
* Improved vibration pulse strength, FNIRSI GC-01 pulse clicks, menu contrast, and data logging time precision.
* Fixed 24-hour history bug.

## 2.0rc3 "The BIG update"

* Updated UI with larger digits in measurement view.
* Added instantaneous rate bar and history alert zones (1 µSv, 10 µSv).
* Added average timer expiration at configurable confidence intervals.
* Added background compensation, FNIRSI GC-01 display flashes, vibration pulses, pulse threshold, and fault alarms.
* Added 10-second and 30-second data log intervals and 10-second display sleep option.
* Added `radpro-flashtool`.
* Removed measurement hold.

## 2.0rc2

* Fixed flash programming, FNIRSI GC-01 battery indicator, and conversion factor menu logic.

## 2.0rc1

* Added FNIRSI GC-01 support.

## 1.3.0

* Added data logging, custom conversion factors, and J305 Geiger tube support.
* Minor bug fixes.

## 1.2.2

* Added GD32 microcontroller support and data submission to gmcmap.com.

## 1.2.1

* Minor bug fixes.

## 1.2.0
* Added non-volatile radiation dose storage and hourly device statistics.
* Added radmon.org client.
* Improved font_tiny5, battery level calculation, flash settings, and state storage.
* Fixed startup sequence, backlight, and SBM-20 conversion ratio bugs.

## 1.1.1

* Minor bug fix.

## 1.1.0

* Added Geiger-Müller tube settings, backlight pulse flash, true random number generator, and SBM-20/SI-3BG tube support.
* Ported to platform.io.
* Improved battery sampling, fixed rounding errors, and enhanced usage instructions.

## 1.0.2

* Added M4011 Geiger-Müller tube support.
* Fixed battery indicator.

## 1.0.1

* Improved pulse click volume.
* Fixed battery indicator and game bugs.

## 1.0.0

* First release.
