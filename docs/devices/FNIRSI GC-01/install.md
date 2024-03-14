# Installing on the FNIRSI GC-01

## Step 1: Open the device

![FNIRSI GC-01 circuit board types](img/gc-01-board-type.jpg)

Follow these steps to start installing Rad Pro on your FNIRSI GC-01:

* Remove the four screws holding the back panel and open the device.
* Take note of the MCU (microprocessor) installed in your device: either CH32F103R8T6 or APM32F103RBT6. The picture above should help you identify the MCU.
* Close the device.

## Step 2: Get the firmware

Download the appropriate firmware from the [Rad Pro releases page](https://github.com/Gissio/radpro/releases):
  * For initial installation, download `radpro-fnirsi-gc01-[MCU]-x.y.z-install.bin`.
  * For updating, download `radpro-fnirsi-gc01-[MCU]-x.y.z-update.bin`.

`[MCU]` is the type of microprocessor on your board: `CH32F103R8T6` or `APM32F103RBT6`.

## Step 3: Flash the firmware

Now, follow these steps to install Rad Pro on your FNIRSI GC-01 Geiger counter:

* Turn off the device.
* Connect the device to your computer using the USB cable.
* Power on the device. A USB drive should become available.
* Drag the `radpro-fnirsi-gc01-[MCU]-x.y.z-install.bin` (or `update`) file onto the USB drive. The device should restart with Rad Pro installed.

If something goes wrong, open the device again, remove the main battery connector, and connect your device to the computer with the USB cable. You can recover your device with the [original firmware](firmware).

## Step 4: Configure your device

Last, start your device and go to Rad Pro's settings, select "Geiger tube", select "HV Profile" and choose an appropriate HV profile: "Factory default 1" is the profile of the original firmware V0.3 and V1.5, "Factory default 2" is the profile of the original firmware V1.6-2.

If the indicated radiation rate is not correct, turn off your device, open it, identify the installed Geiger tube, close your device, restart it and go to Rad Pro's settings, select "Geiger tube", "Conversion factor", and select the option that matches the Geiger-Müller tube installed in your device.

## Step 5: Getting involved

If you like Rad Pro, consider watching the project to you get notified when new releases are out. Also, show your support by starring the project on GitHub.

## Hardware-specific notes

The keys are mapped as follows:

  * Power on/off: long press of the ok/power key.
  * Go up/down: the up and down keys.
  * Hold measurement: the ok/power or left/back key.
  * Reset measurement: long press of the left/back key.
  * Enter settings: the right/settings key.
  * Select option: the ok/power or right/settings key.
  * Bo back: the left/back key.

On the FNIRSI GC-01 with a CH32F103R8 processor, you must enter "USB mode" in the Settings to enable a USB connection. On the FNIRSI GC-01 with an APM32F103R8 (Geehy) processor, the USB connection is always available. 

On the FNIRSI GC-01 with a CH32F103R8 processor, Rad Pro can store up to 5060 data points. At normal radiation levels, this allows for 3 days of data at 1-minute intervals, 8 days at 5-minute intervals, 17 days at 10-minute intervals, 52 days at 30-minute intervals, and 105 days at 60-minute intervals.

On the FNIRSI GC-01 with an APM32F103RB (Geehy) processor, Rad Pro can store up to 63756 data points. At normal radiation levels, this allows for 44 days of data at 1-minute intervals, 110 days at 5-minute intervals, 221 days at 10-minute intervals, 664 days at 30-minute intervals, and 1328 days at 60-minute intervals.

The FNIRSI GC-01 HV profile settings are:

* Factory default 1: 9.207 kHz frequency and 75 % duty cycle.
* Factory default 2: 47.058 kHz frequency and 50 % duty cycle.
