# Installing on the FNIRSI GC-01

## Step 1: Initiating update mode

Follow these steps to start installing Rad Pro on your FNIRSI GC-01 Geiger counter:

* Turn off the FNIRSI GC-01 device.
* Connect the device to your computer using the USB cable.
* Ponwer on the FNIRSI GC-01.
* A USB disk should become available.

## Step 2: Flash the firmware

Now, follow these steps:

* Download the appropriate firmware from the [Rad Pro releases](https://github.com/Gissio/radpro/releases):
  * For initial installation, get `radpro-fnirsi-gc01-install-x.y.z.bin`.
  * For updating, get `radpro-fnirsi-gc01-update-x.y.z.bin`.
* Drag the file to the USB disk.
* The device should restart automatically.

## Step 3: Show your support

If you like Rad Pro, consider showing your support by starring the project on GitHub. Your support is greatly appreciated.

## Hardware-specific notes

The keys are mapped as follows:

  * Power on/off: Long-press OK/POWER key.
  * Go up/down: UP and DOWN keys.
  * Go down: Short-press SET/POWER key.
  * Enter menu/select option: RIGHT/CONFIG key.
  * Go back/hold measurement: LEFT/BACK key.
  * Reset measurement: Long-press LEFT/BACK keys.

On the FNIRSI GC-01, Rad Pro can store up to 95880 data points. At normal radiation levels, this allows for 66 days of data at 1-minute intervals, 166 days at 5-minute intervals, 332 days at 10-minute intervals, 998 days at 30-minute intervals, and 1997 days at 60-minute intervals.
