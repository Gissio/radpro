# Releases

## 3.2 "Gambit"

* Added support for the GQ GMC-800V2.
* Added **Estonian** translation.
* FS2011 (GD32F103): Fixed the display backlight not turning on.
* Bosean FS-600, Bosean FS-1000, Bosean FS-5000: Fixed the counter possibly hanging at startup after losing power while saving settings or writing the data log.
* Bosean FS-600, Bosean FS-1000: Fixed the history loading slowly at startup: the faster processor clock used for loading it was not actually being applied.
* FNIRSI GC-01: Fixed the display backlight turning on at full brightness during startup.
* FNIRSI GC-03: Fixed the knob occasionally stepping in the wrong direction.
* FNIRSI GC-03: Fixed the pulse LED flashing for only half of its intended duration.
* FNIRSI GC-03: Fixed the battery level barely changing after a few hours of operation, and the counter powering off with a false low-battery indication after about 30 hours of continuous operation.
* FNIRSI GC-03: Fixed the counter turning off after a crash, instead of restarting.
* GQ GMC-800: Improved LCD image quality: deeper blacks, dark colors that no longer look washed out (especially in the Dusk and Night themes), and no more bright band along the display edges.
* GQ GMC-800, GQ GMC-800V2: The random generator now plays the alert sound when it finishes generating symbols (ASCII, alphanumeric, hexadecimal, decimal and binary modes).
* GQ GMC-800, GQ GMC-800V2: Fixed the display backlight occasionally staying on after the display turned off, and pulse vibrations occasionally being skipped at very high count rates.
* GQ GMC-800, GQ GMC-800V2: Fixed voice readouts staying silent when the rate or dose was zero (they now say "zero"), and voice alerts occasionally cutting off a voice readout that had just been requested.
* GQ GMC-800, GQ GMC-800V2: Fixed voice readouts saying "zero" for values between 0.0005 and 0.0009, instead of rounding them to 0.001.
* FNIRSI GC-01, FNIRSI GC-03, GQ GMC-800V2: Fixed the USB connection stalling when a program closed the serial port in the middle of a response, and responses occasionally containing repeated or corrupted characters when a request arrived while a response was being sent.
* FS2011, Bosean FS-600, Bosean FS-1000: Fixed the alert display flash occasionally being skipped when display sleep was set to "Always off".
* FS2011 (GD32F103), FNIRSI GC-01, FNIRSI GC-03, GQ GMC-800, GQ GMC-800V2: Fixed the clock running about 2.6 seconds per day slow after a full power loss, until the time was set.
* Bosean FS-600, Bosean FS-1000, Bosean FS-5000, FNIRSI GC-01, FNIRSI GC-03: Fixed pulse LED flashes occasionally being cut short.
* Bosean FS-5000, FNIRSI GC-01, FNIRSI GC-03, GQ GMC-800, GQ GMC-800V2: Fixed the random generator occasionally cutting off long symbol strings with "…"; strings that do not fit on one line now continue on a second line.
* FS2011, Bosean FS-600, Bosean FS-1000, Bosean FS-5000, FNIRSI GC-03, GQ GMC-800, GQ GMC-800V2: Fixed the display backlight rarely getting stuck on or off when a pulse display flash coincided with the backlight turning on or off.
* Selecting a pulse sound style (clicks, chirps or beeps) now plays a sample, like the alert sound settings.
* Fixed `GET tubeType` returning the wrong tube name for tube types after M4011.
* Improved key debouncing: brief electrical glitches, such as interference from the high-voltage generator, no longer register as key presses.
* Fixed key presses being lost when quickly moving from one key to another.
* Fixed key presses being lost when several arrive while the counter is busy.
* Fixed the counter occasionally acting as if the display were still asleep after waking it up, when a pulse display flash or the display sleep timeout ended at that moment: the next key press only woke the display again, and on the FNIRSI GC-03, knob turns were ignored.
* Fixed key presses on the startup screen delaying startup by reloading the history, and, after a firmware checksum failure, possibly preventing the history from loading.
* In lock mode, holding the power key now wakes the display, like the other keys, instead of doing nothing.
* Fixed turning off the display, or entering lock mode, occasionally being undone when it coincided with an alert display flash or with connecting USB power.
* Fixed the random generator occasionally reusing random bits, which could make consecutive outputs correlated.
* Fixed the instantaneous rate with fixed averaging intervals (1 minute, 30 seconds and 10 seconds) keeping its last value, and any active rate alert, when no pulses were detected during the averaging interval; it now reads zero, as documented.
* Fixed data logging stopping, and further data log downloads failing, after a download was interrupted (for example, by unplugging the USB cable or closing the serial port).
* Fixed the history occasionally showing spurious peaks, and data log downloads occasionally starting with an incorrect record, after the data log wrapped around.
* Fixed `SET tubePulseCount` causing a spurious peak in the history.
* Fixed the history graph line occasionally disappearing at the bottom of the graph, where low readings could look like missing data.
* Fixed the confidence interval showing "±10.0%" instead of "±10%" for values just under 10%.
* Improved data log recovery from flash write errors.
* Fixed settings and the cumulative dose no longer being saved after the counter lost power while saving them.
* Data log downloads interrupted on the counter (by a clock change, a logging mode change, a data log reset or powering off) now end with an `ERROR` record, instead of silently returning incomplete data.
* Data logging now resumes as soon as a data log download reaches its maximum number of records.
* Requests with unexpected arguments, malformed or out-of-range numbers or more than 63 characters, and requests received with lost characters, are now rejected with `ERROR` instead of being partially executed.
* Fixed `SET tubeHVFrequency` and `SET tubeHVDutyCycle` possibly setting an invalid high-voltage generator configuration when given a value with about 40 or more digits.
* Fixed `GET tubeRate` returning incorrect values at very high count rates.
* Fixed the tube dead time (shown on the statistics screen and returned by `GET tubeDeadTime`) occasionally reading lower than the tube's actual dead time: pulse detection now takes priority over other tasks, as intended, and electrical glitches shorter than 10 µs no longer count as pulse intervals.
* Fixed the date jumping into the following month when setting a month or year in which the current day does not exist (for example, changing January 31 to February now gives February 28 instead of March 3).
* `SET deviceTimeZone` now rounds fractional time zones to the nearest hour, and rejects invalid values instead of setting an incorrect time zone.
* `SET deviceTime` now rejects times outside the years 2000–2099 on devices whose clock stores only two-digit years, instead of setting an incorrect year.
* Fixed the counter hanging at startup when the real-time clock crystal fails to start.
* Improved real-time clock reliability: the time is no longer read before the clock has synchronized, after startup or after setting the time.
* Fixed the real-time clock not running when a previous firmware had left it on a different clock source; the clock is now reset.
* Improved battery voltage measurement accuracy.
* Fixed the battery level indicator switching back and forth between two levels when the battery voltage is near a level boundary.
* `radpro-tool` now discards data left over from a previous connection, which could shift every response by one.
* Reduced firmware size and memory usage through code optimizations.
* Updated the game engine to **mcu-max 2.0.0**:
  * Reworked the strength levels: the lower levels now play occasional random moves.
  * Fixed an issue causing the game to crash the counter: the search depth is now limited to the memory available on each device.
  * The search now stops when the counter powers off.
  * Fixed a possible crash in positions with a very large number of legal moves.
  * Fixed the game screen becoming unresponsive when the back key was pressed right after making a move.
  * The final move is now highlighted when the game ends.
* Updated **mcu-renderer** to 1.0.20.
* AI Disclaimer: Claude Code was used for debugging/optimizing code.

## 3.1.1 "Aligned"

* Fixed memory alignment issues.

## 3.1 "Credible Sources"

* **Major highlights**

  * Added support for the **FNIRSI GC-03**.
  * Default sensitivity settings for Geiger-Müller tubes are now based on numerical simulations from [Rad Lab](https://github.com/Gissio/radlab).
  * Introduced **source presets** — pre-configured sensitivity settings for common radiation sources (also based on Rad Lab).
  
* **New features**

  * New **Cumulative** and **Instantaneous** secondary measurement views.
  * Implemented **USB auto power-on** on supported hardware.
  * Warnings now use short beeps and brief haptic feedback.
  * Random generator "die throws" can now be re-rolled using the select key.
  * Added option to submit measurement data to [openSenseMap.org](https://opensensemap.org).
  * Split documentation into separate User Manual and Technical Reference Manual
  * Documentation: Added a procedure for setting the HV voltage without a multimeter.
  * Updated user interface to **OpenBridge 6.1**.

* **Performance & optimizations**

  * 4× faster random number generator.
  * Improved key debouncing for more reliable button response.
  * Various performance and memory optimizations throughout the firmware

* **Device-specific improvements**

  * Bosean FS-5000: Optimized MCU configuration for improved performance.
  * FNIRSI GC-01: Added USB-powered / charging status indicator.
  * FNIRSI GC-01 (CH32): Removed data mode as USB communications are now automatically enabled.
  * FNIRSI GC-01 (APM32): Now reads factory HV profile from factory flash.

* **Bug fixes**

  * Bosean FS-600, FNIRSI GC-01 (APM32): Fixed startup freeze.
  * GQ GMC-800: Eliminated LCD ghosting artifacts.
  * Fixed numerous minor bugs.

* **Localization**

  * Added new translations: **Indonesian**, **Latvian**, and **Lithuanian**.
  * Improved existing translations: **Bulgarian**, **Russian**, and **Ukrainian**.

* **Cleanup**

  * Removed **Time** secondary view from the Instantaneous view.
  * Removed legacy support for SWD data communications.

## 3.0.2 "Icebreaker"

* Fixed web installer update for the Bosean FS-5000.
* Resolved a problem causing devices to become unresponsive after software updates.
* Corrected a bug on the GQ GMC-800 that prevented alerts and voice output from functioning properly.

## 3.0.1 "The Glow Fix"

* Removed the charging indicator on the FNIRSI GC-01 due to hardware limitations preventing differentiation between USB disconnected and USB charging states.
* Resolved an issue where the battery indicator failed to display during power-off on the Bosean FS-5000.
* Improved battery indicator response time.

## 3.0 "Sundry"

* **Geiger counter compatibility**

  * Added support for the GQ GMC-800 Geiger counter with software-enhanced pulse stretching for reliable pulse sound at high radiation levels.

* **Multilingual support**

  * Expanded to 27 languages, including Bulgarian, Chinese, Croatian, Czech, Danish, Dutch, English, Finnish, French, German, Greek, Hungarian, Italian, Japanese, Korean, Norwegian, Polish, Portuguese, Romanian, Russian, Slovakian, Slovenian, Spanish, Swedish, Turkish, Ukrainian, and Vietnamese.

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

## 2.1.1 "The Dot Mender"

* **Updates**

  * Renamed **Conversion factor** to **Sensitivity** (as sensitivity is cpm/radiation rate, while conversion factor is radiation rate/cpm).
  * Renamed `GET tubeConversionFactor` to `GET tubeSensitivity`.
  * Short press of power key on 5-key devices in measurement screen activates display sleep.
  * On 3-key devices, holding left and right keys in measurement screen toggles pulse sounds.
  * Lock mode now accessible from any screen.
  * Die rolls and coin flips generate single outcomes per run.
  * Added support for 100-sided and 10-sided die rolls and binary random number generation.
  * History calculated only when instantaneous rate confidence interval is below 75%.
  * Automatic dose saving every 60 minutes at critically low battery levels.

* **Bug fixes**

  * Fixed missing measurement dot on color display devices.
  * Corrected battery voltage measurement on Bosean FS-5000.

## 2.1 "Delicious Trifles"

* **Updates**

  * Added alert dismissal via reset measurement key.
  * Configurable alert indications: sound, vibration, pulse LED, and display flash.
  * Added alert-enabled indicator.
  * Implemented keyboard lock mode, accessible from Settings.
  * Doses below 10,000 now displayed in standard notation.
  * Added new rate and dose alarm options, average timer options, and extended dead-time compensation (20–320 µs).
  * Enabled pulse sound toggle from measurement screen on supported devices.
  * Added pulse chirp sound option and extended alert sound duration.
  * Improved menu navigation and updated UI font to Open Sans for better readability.

* **Fixes**

  * Fixed unsightly flash on color displays when waking from sleep.
  * Resolved alerts sounding after shutdown while externally powered.
  * Corrected secondary rate and count view display after average timer expiration.
  * Fixed device state reset after quick power cycles.
  * Corrected 1-minute instantaneous averaging at high radiation levels.

## 2.0.3 "The Sensitive Update"

* Extended conversion factor range to 25-12,800 cpm/µSv/h.
* Capped pulse indication at 600 cpm/µSv/h for sensitive detectors to maintain ~1 pulse/second at background levels (no impact on measurements).
* Minor UI and performance improvements.

## 2.0.2 "The Precise Release"

* Improved instantaneous rate averaging algorithm and simplified response curves for J613/J614 tubes.
* Disabled instantaneous rate alarms during high confidence intervals (e.g., post-startup).
* Minor UI improvements.

## 2.0.1 "Nobody's Perfect"

* Added data log reset.
* Reverted J613/J614 tube conversion factor to 60.0 cpm/µSv/h.

* Fixed custom HV profiles, `SET tubeHVFrequency`, `SET tubeHVDutyCycle`, and history data log download issues.
* Minor refinements.

## 2.0 "Grand Première"

* Updated UI to [OpenBridge 5.0](https://www.openbridge.no/) for better sunlight legibility.
* Changed J613/J614 tube conversion factor to 30.0 cpm/µSv/h.
* Removed FNIRSI GC-01 display panel settings.
* Increased granularity for `SET tubeHVFrequency` (100 Hz–100 kHz) and `SET tubeHVDutyCycle` (0%–100%).

## 2.0rc5 "The Final Countdown"

* Added Bosean FS-5000 support and display panel selection menu.
* Added secondary dose view in average measurement mode.
* Extended conversion factor range to 25–1600 cpm/µSv/h.
* Added 1-second data log interval; removed 30-second and 30-minute options.
* Added `SET tubeHVFrequency` and `SET tubeHVDutyCycle` commands.
* Added 24/12-hour time format selection.
* Improved data mode usability and battery level indicators for FS2011, Bosean FS-600/FS-1000.
* Changed USB data connection EOL to "\r\n".

## 2.0rc4 "The Little Update"

* Added instantaneous rate averaging options: adaptive fast, adaptive precision, 1 minute, 30 seconds, and 10 seconds.
* Added secondary CPM and count displays.
* Improved vibration pulse strength, FNIRSI GC-01 pulse clicks, menu contrast, and data logging time precision.
* Fixed 24-hour history bug.

## 2.0rc3 "The BIG Update"

* Updated UI with larger digits in measurement screen.
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

* Added GD32 microcontroller support and data submission to <https://gmcmap.com>.

## 1.2.1

* Minor bug fixes.

## 1.2.0

* Added non-volatile radiation dose storage and hourly device statistics.
* Added <https://radmon.org> and <https://safecast.org> client.
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
