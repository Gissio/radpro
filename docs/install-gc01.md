# Installing on the FNIRSI GC-01

## IMPORTANT

RAD PRO 2.0 IS NOT YET READY. The files in the releases are only for the FS2011 (STM32F051).

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

If you like Rad Pro, show your support by starring the project on GitHub. Your support is greatly appreciated.

## Hardware-specific notes

The keys are mapped as follows:

  * Power on/off: Long-press OK/POWER key.
  * Go up/down: UP and DOWN keys.
  * Enter menu: RIGHT/CONFIG key.
  * Select option: OK/POWER key.
  * Go back/hold measurement: LEFT/BACK key.
  * Reset measurement: Long-press LEFT/BACK key.

On the FNIRSI GC-01, Rad Pro can store up to 6090 data points. At normal radiation levels, this allows for 2 days of data at 1-minute intervals, 10 days at 5-minute intervals, 31 days at 10-minute intervals, 95 days at 30-minute intervals, and 190 days at 60-minute intervals.
