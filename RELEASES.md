# Releases

## 2.2 "Sundry"

Updates:
* Added language support for: Bulgarian, Chinese, Croatian, Czech, Danish, Dutch, English, Finnish, French, German, Greek, Hungarian, Italian, Japanese, Korean, Norwegian, Polish, Portuguese, Romanian, Russian, Slovakian, Slovenian, Spanish, Swedish, Turkish, Ukrainian, Vietnamese.
* Introduced new averaging periods: 3, 6, 12, 24-hour intervals, plus confidence intervals of ±50%, ±20%, ±10%, ±5%, ±2% and ±1%.
* Extended sensitivity range to 1-10,000 cpm/µSv/h to support low-sensitivity Geiger tubes (e.g., SI-3BG).
* Extended dead-time compensation range to 5-500 µs for compatibility with scintillation detectors.
* Added optional overrange alarm, triggered when dead-time compensation exceeds 10%.
* Improved loss-of-count detection, now dynamically adjusted based on selected sensitivity.
* Added timezone configuration data commands: "SET deviceTimeZone" and "GET deviceTimeZone".
* Enhanced radpro-tool, now synchronizing timezone alongside time, and improving error handling.
* Added "RESET datalog" data command to clear data logs.
* Removed background compensation, as intrinsic activity in common Geiger tubes is negligible.
* Removed "GET tubeBackgroundCompensation" data command to reflect the above change.
* Refined SBM-20 Geiger tube sensitivity to 150.5 cpm/µSv/h.

Fixes:
* Updated statistics to display battery voltage for all cells.
* Fixed an issue preventing proper updates to history and instantaneous rate maximum values for 10-second, 30-second and 60-second instantaneous rate averaging intervals.
* Corrected settings to display hours in AM/PM format when 12-hour format is selected.

## 2.1.1 "The dot mender"

Updates:
* Renamed "Conversion factor" to "Sensitivity" (as sensitivity is cpm/radiation rate, while conversion factor is radiation rate/cpm).
* Renamed the "GET tubeConversionFactor" data command to "GET tubeSensitivity".
* On 5-key devices, a short press of the power key in the measurement view now puts the display to sleep.
* On 3-key devices, holding the left and right keys in the measurement view now toggles pulse sounds.
* Lock mode can now be accessed from any screen without needing to navigate to the settings menu.
* Die rolls and coin flips now generate a single outcome per run.
* Added support for 100-sided and 10-sided die rolls, as well as binary random number generation.
* History is now calculated only when the instantaneous rate confidence interval falls below 75%.
* When the battery level is critically low (0 bars), Rad Pro now automatically saves the dose every 60 minutes.

Fixes:
* On devices with a color display, fixed an issue where the measurement dot occasionally failed to appear in the measurement view.
* Corrected battery voltage measurement issues on the Bosean FS-5000.

## 2.1 "Delicious trifles"

Updates:
* Added the ability to dismiss alarms with the reset measurement key.
* Added configurable alarm indication: sound, vibration, pulse LED and display flash.
* Added an alarm enabled indicator.
* Implemented a keyboard lock mode, accessible from Settings.
* Display of dose counts below 10,000 now shown in standard notation.
* Added new rate and dose alarm options.
* Added new average timer options.
* Extended the dead-time compensation range to 20-320 µs.
* Added the ability to toggle pulse sounds from the measurement view on supported devices.
* Added a new pulses chirps sound option.
* Extended the duration of alarm sounds.
* Enhanced the menu structure for more intuitive navigation.
* Updated the UI font to Open Sans for enhanced readability.
* Minor UI improvements.

Fixes:
* On color displays, fixed an issue causing an unsightly flash when waking from sleep mode.
* Fixed alarms continuing to sound after device shutdown while connected to external power.
* Corrected the display of secondary rate and count views after the average timer expired.
* Resolved an issue where the device state wasn't reset after a quick power cycle.
* Corrected a problem where 1 minute instantaneous averaging at high radiation levels didn't average correctly.

## 2.0.3 "The sensitive update"

* Extended conversion factor range to 25-12,800 cpm/µSv/h.
* When using sensitive detectors with a conversion factor of 600 cpm/µSv/h or higher, pulse indication is now capped at 600 cpm/µSv/h to provide approximately one pulse per second at background levels of radiation. This does not impact the actual measurements.
* Minor UI and performance refinements.

## 2.0.2 "The precise release"

* Improved the instantaneous rate averaging algorithm.
* Simplified the instantaneous rate averaging response curves, resulting in improved precision when using J613/J614 tubes.
* Disabled instantaneous rate alarms in situations where the confidence interval is high, such as immediately after device startup.
* Minor UI improvements.

## 2.0.1 "Nobody's perfect"

* Added data log reset.
* Reverted the default conversion factor for J613/J614 tubes to 60.0 cpm/µSv/h.

* Resolved issues with custom HV profiles.
* Resolved issues with the "SET tubeHVFrequency" and "SET tubeHVDutyCycle" data connection requests.
* Resolved issues with new history data log download.
* Minor refinements.

## 2.0 "Grand première"

* First 2.0 release.
* Updated the UI design system to [OpenBridge 5.0](https://www.openbridge.no/), improving legibility under sunlight.
* Changed the default conversion factor for J613/J614 tubes to 30.0 cpm/µSv/h.
* Removed the FNIRSI GC-01 display panel settings.
* Increased the granularity and range of the "SET tubeHVFrequency" and "SET tubeHVDutyCycle" data connection requests to CPU clock resolution and 100 Hz-100 kHz and 0%-100%, respectively (not stored on power-off).

## 2.0rc5 "The final countdown"

* Added Bosean FS-5000 support.
* Added display panel menu for selecting the display panel (on supported devices).
* Added secondary dose view in average measurement mode.
* Extended conversion factor range to 25-1600 cpm/µSv/h.
* Added 1 second data log interval options and removed 30 seconds and 30 minutes data log interval options.
* Added "SET tubeHVFrequency" and "SET tubeHVDutyCycle" data connection commands.
* Added 24/12-hour time format selection.
* Devices with data mode allow using the device while data mode is active; data mode state is also preserved after powering off.
* Improved battery level indicator of the Bosean FS-600/FS-1000.
* Improved battery level readout of the FS2011(STM32F051), Bosean FS-600/FS-1000.
* Improved keyboard control scheme of the Bosean FS-1000.
* Changed USB data connection end of line to "\r\n".
* Minor refinements.

## 2.0rc4 "The little update"

* Added instantaneous rate averaging options: adaptive fast, adaptive precision, 1 minute, 30 seconds and 10 seconds.
* Added secondary CPM and count displays in measurement views.
* Improved vibration pulse strengths.
* Improved FNIRSI GC-01 quiet pulse clicks.
* Improved selected menu item contrast.
* Improved data logging time precision.

* Fixed History (24 h) bug.

## 2.0rc3 "The BIG update"

* UI update with big digits in measurement view.
* Instantaneous rate bar with alert zones (1 µSv, 10 µSv).
* History alert zones (1 µSv, 10 µSv).
* Adde average timer expiration after reaching a configurable confidence interval.
* Added background compensation.
* Added display flashes on the FNIRSI GC-01 (after display sleep).
* Added vibration pulses on the FNIRSI GC-01 (off, weak or strong).
* Added pulse threshold (disables pulse indication if instantaneous rate is below a certain value).
* Added fault alarm on HV generator/Geiger tube failure (lack of pulses) or tube saturation (tube shorted).
* Added 30 second and 10 second data log interval options.
* Added 10 second display sleep option.
* Added `radpro-flashtool`.
* Removed measurement hold.

## 2.0rc2

* Fixed flash programming.
* Fixed FNIRSI GC-01 battery indicator.
* Fixed conversion factor menu logic.

## 2.0rc1

* Added FNIRSI GC-01 support.

## 1.3.0

* Added data logging.
* Added custom conversion factors.
* Added support for the J305 Geiger tube.
* Minor bug fixes.

## 1.2.2

* Added support for devices with a GD32 microcontroller.
* Added data submission to the https://gmcmap.com/ radiation monitoring website.

## 1.2.1

* Minor bugfixes.

## 1.2.0
* Implemented storage of radiation dose in non-volatile memory.
* Added the capability to store device statistics every hour.
* Added client for the https://radmon.org radiation monitoring website.
* Improved the font_tiny5 5-pixel font.
* Enhanced battery level calculation using battery discharge curves and HV pulse syncing for shot noise removal.
* Improved flash settings and state storage.
* Fixed a start-up sequence bug.
* Fixed a backlight bug when turning off the device.
* Corrected the SBM-20 conversion ratio.

## 1.1.1

* Minor bug fix

## 1.1.0

* Added Geiger-Müller tube type settings.
* Added backlight pulse flash setting.
* Added true random number generator.
* Added support for the SBM-20 and SI-3BG Geiger-Müller tube.
* Ported the project to the platform.io environment.
* Improved battery level sampling.
* Fixed rounding errors.
* Improved usage instructions.

## 1.0.2

* Added support for M4011 Geiger-Müller tube.
* Fixed battery indicator.

## 1.0.1

* Improved pulse click volume.
* Fixed battery indicator.
* Fixed several bugs in game.

## 1.0.0

* First release
