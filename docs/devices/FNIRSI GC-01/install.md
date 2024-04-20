# Installing Rad Pro on the FNIRSI GC-01

## Step 1: Open the device

![FNIRSI GC-01 circuit board types](img/gc-01-board-type.jpg)

Follow these steps to start installing Rad Pro on your FNIRSI GC-01:

* Turn off the device.
* Remove the screws holding the back panel and open the device.
* Take note of the MCU (microprocessor) installed in your device: CH32F103R8T6 (WCH) or APM32F103RBT6 (Geehy). The picture above should help you identify the MCU. Important: Rad Pro does not work on devices with a CACHIP microprocessor.
* Also, take note of the type of Geiger tube: the markings on the glass should be either J321 (as in the picture above), J305 or J614. If you have an unmarked tube measuring 55 or 65 mm long, you probably have a J614 or J613 tube, respectively.
* Close the device.

## Step 2: Flash the firmware

Download the firmware file `radpro-fnirsi-gc01-[mcu]-x.y.z.bin` from the [Rad Pro releases](https://github.com/Gissio/radpro/releases). `[mcu]` is the type of microprocessor of your device: `CH32F103R8T6` or `APM32F103RBT6`.

Next:

* **IMPORTANT**: Make sure you perform the following instructions from a Windows computer. Installation from macOS or Linux may fail.
* Power on the device. A USB drive should become available.
* Drag the firmware file onto the USB drive. The device should restart with Rad Pro installed.

Some devices require that you keep the power button pressed from power on until the firmware is copied to the USB drive and the device restarts. Some other devices require that you press the right/settings key while pressing the power key for the USB drive to become available.

If something goes wrong, press the reset switch next to the USB connector and repeat the previous procedure. You can recover your device with the [original firmware](firmware).

If you are unable to install Rad Pro on your device, or your device becomes unresponsive, follow the [FNIRSI GC-01 alternative installation instructions](install-stlink.md).

## Step 3: Configure your device

Go to Rad Pro's settings, select "Geiger tube", select "Conversion factor", and select the option that matches the Geiger-Müller tube installed in your device.

Within the "Geiger tube" settings, select "HV Profile" and choose an appropriate HV profile: "Energy-saving" is best for measuring background levels of radiation; "Factory default" is the profile from the original firmware and consumes considerably more power.

Last, read the [user's manual](../../users.md) for learning how to use Rad Pro.

## Step 4: Getting involved

If you like Rad Pro, consider watching the project to get notified when new releases are out. Also, show your support by starring the project on GitHub.

## Hardware-specific notes

The keys are mapped as follows:

  * Power on/off: long press of the ok/power key.
  * Go up/down: the up and down keys.
  * Toggle measurement view: the ok/power or left/back key.
  * Reset measurement: long press of the left/back key.
  * Enter settings: the right/settings key.
  * Select option: the ok/power or right/settings key.
  * Go back: the left/back key.

### FNIRSI GC-01 with a CH32F103R8 (WCH) microprocessor

On the FNIRSI GC-01 with a CH32F103R8 microprocessor, you must enter "Data mode" in the Settings to enable a USB data connection. The data connection is disabled when you leave "Data mode".

Data logging can store up to 5060 data points. At normal radiation levels (20 cpm), this allows for 105 days of data at 60-minute intervals, 52 days at 30-minute intervals, 17 days at 10-minute intervals, 8 days at 5-minute intervals, 3 days at 1-minute intervals, 1 day at 30-second intervals and 14 hours at 10-second intervals.

The HV profile settings are:

* Factory default: 9.207 kHz frequency and 75 % duty cycle.
* Energy-saving: 5 kHz frequency and 1.5 % duty cycle.

### FNIRSI GC-01 with a APM32F103R8 (Geehy) processor

On the FNIRSI GC-01 with an APM32F103R8 (Geehy) processor, the USB data connection is always available. 

Data logging can store up to 63756 data points. At normal radiation levels (20 cpm), this allows for 1328 days of data at 60-minute intervals, 664 days at 30-minute intervals, 221 days at 10-minute intervals, 110 days at 5-minute intervals, 44 days at 1-minute intervals, 22 days at 30-second intervals and 7 days at 10-second intervals.

The HV profile settings are:

* Factory default: 47.058 kHz frequency and 50 % duty cycle.
* Energy-saving: 5 kHz frequency and 1.5 % duty cycle.
