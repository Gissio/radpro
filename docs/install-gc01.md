# Installing on the FNIRSI GC-01

## Step 1: Get the firmware

Download the appropriate firmware from the [Rad Pro releases](https://github.com/Gissio/radpro/releases):
  * For initial installation, get `radpro-fnirsi-gc01-install-x.y.z.bin`.
  * For updating, get `radpro-fnirsi-gc01-update-x.y.z.bin`.

## Step 2: Flash the firmware

Now, follow these steps to install Rad Pro on your FNIRSI GC-01 Geiger counter:

* Turn off the FNIRSI GC-01 device.
* Connect the device to your computer using the USB cable.
* Power on the FNIRSI GC-01. A A USB disk should become available.
* Drag the Rad Pro firmware file to the USB disk. The device should restart automatically with Rad Pro installed.

If something goes wrong, you can recover your device with the [original firmware V0.3](devices/FNIRSI%20GC-01/FNIRSI%20GC-01%20V0.3.zip) or [original firmware V1.5](devices/FNIRSI%20GC-01/FNIRSI%20GC-01%20V1.5.bin).

## Step 3: Show your support

If you like Rad Pro, show your support by starring the project on GitHub.

## Hardware-specific notes

The keys are mapped as follows:

  * Power on/off: long press of the ok/power key.
  * Go up/down: the up and down keys.
  * Hold measurement: the ok/power or left/back key.
  * Reset measurement: long press of the left/back key.
  * Enter settings: the right/settings key.
  * Select option: the ok/power or right/settings key.
  * Bo back: the left/back key.

On the FNIRSI GC-01, Rad Pro can store up to 6084 data points. At normal radiation levels, this allows for 2 days of data at 1-minute intervals, 10 days at 5-minute intervals, 31 days at 10-minute intervals, 95 days at 30-minute intervals, and 190 days at 60-minute intervals.

The FNIRSI GC-01 HV profile values are:

* Factory default: 10 kHz frequency and [to-do] duty cycle.
* Optimized: [to-do] kHz frequency and [to-do] duty cycle.
* Energy-saving: [to-do] kHz frequency and [to-do] duty cycle.
