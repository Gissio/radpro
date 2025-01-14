# Releases

## 2.1 The big update

* Added the ability to dismiss alarms using the reset measurement key.
* Introduced an alarm enabled indicator.
* Added options for higher rate and dose alarms.
* Extended the duration of alarm sounds.
* Implemented a lock mode accesible from Settings.
* Displays of dose counts below 10000 now uses standard notation for clarity.
* Enhanced the menu structure for improved navigation.
* Updated the UI font to Open Sans to enhance readability.
* Minor UI improvements.

* On color displays, fixed an issue causing an unsightly flash when waking from sleep mode.
* On the FNIRSI GC-01, corrected random pixel issues during the splash screen.
* On the FNIRSI GC-01, added a battery charging indicator.
* Fixed alarms continuing to sound after device shutdown while connected to external power.
* Resolved an issue where the device state wasn't reset after a quick power cycle.
* Corrected a problem where 60-second instantaneous averaging at high radiation levels didn't average correctly over the full seconds.

## 2.0.3 The sensitive update

* Extended conversion factor range to 25-12800 cpm/µSv/h.
* When using sensitive detectors with a conversion factor of 600 cpm/µSv/h or higher, pulse indication is now capped at 600 cpm/µSv/h to provide approximately one pulse per second at background levels of radiation. This does not impact the actual measurements.
* Minor UI and performance refinements.

## 2.0.2 The precise release

* Improved the instantaneous rate averaging algorithm.
* Simplified the instantaneous rate averaging response curves, resulting in improved precision when using J613/J614 tubes.
* Disabled instantaneous rate alarms in situations where the confidence interval is high, such as immediately after device startup.
* Minor UI improvements.

## 2.0.1 Nobody's perfect

* Added data log reset.
* Changed the default conversion factor for J613/J614 tubes to 60.0 cpm/µSv/h.

* Resolved issues with custom HV profiles.
* Resolved issues with the "SET tubeHVFrequency" and "SET tubeHVDutyCycle" data connection requests.
* Resolved issues with new history data log download.
* Minor refinements.

## 2.0 Grand première 

* First 2.0 release.
* Updated the UI design system to [OpenBridge 5.0](https://www.openbridge.no/), improving legibility under sunlight.
* Changed the default conversion factor for J613/J614 tubes to 30.0 cpm/µSv/h.
* Removed the FNIRSI GC-01 display panel settings.
* Increased the granularity and range of the "SET tubeHVFrequency" and "SET tubeHVDutyCycle" data connection requests to CPU clock resolution and 100 Hz-100 kHz and 0%-100%, respectively (not stored on power-off).

## 2.0rc5 The final countdown

* Added Bosean FS-5000 support.
* Added display panel menu for selecting the display panel (on supported devices).
* Added secondary dose view in average measurement mode.
* Extended conversion factor range to 25-1600 cpm/µSv/h.
* Added 1 second data log interval options and removed 30 seconds and 30 minutes data log interval options.
* Added "SET tubeHVFrequency" and "SET tubeHVDutyCycle" data connection commands.
* Added 12-hour time format.
* Devices with data mode allow using the device while data mode is active; data mode state is also preserved after powering off.
* Improved battery level indicator of the Bosean FS-600/FS-1000.
* Improved battery level readout of the FS2011(STM32F051), Bosean FS-600/FS-1000.
* Improved keyboard control scheme of the Bosean FS-1000.
* Changed USB data connection end of line to "\r\n".
* Minor refinements.

## 2.0rc4 The "little" update

* Added instantaneous rate averaging options: adaptive fast, adaptive precision, 60 seconds, 30 seconds and 10 seconds.
* Added secondary CPM and count displays in measurement views.
* Improved haptic pulse strengths.
* Improved FNIRSI GC-01 quiet pulse clicks.
* Improved selected menu item contrast.
* Improved data logging time precision.

* Fixed History (24 h) bug.

## 2.0rc3 The "BIG" update

* UI update with big digits in measurement view.
* Instantaneous rate bar with alert zones (1 µSv, 10 µSv).
* History alert zones (1 µSv, 10 µSv).
* Average timer expiration after reaching a configurable confidence interval.
* Background compensation.
* Display flashes on the FNIRSI GC-01 (after display sleep).
* Haptic pulses on the FNIRSI GC-01 (off, weak or strong).
* Pulse thresholding (disables pulse indication if instantaneous rate is below a certain value).
* Fault alarm on HV/tube failure (lack of pulses) or tube saturation (tube shorted).
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
* Fixed several bugs in Nuclear chess.

## 1.0.0

* First release
