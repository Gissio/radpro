# Installing on the FNIRSI GC-01

## Step 1: Get the firmware

Download the appropriate firmware from the [Rad Pro releases](https://github.com/Gissio/radpro/releases):
  * For initial installation, get `radpro-fnirsi-gc01-install-x.y.z.bin`.
  * For updating, get `radpro-fnirsi-gc01-update-x.y.z.bin`.

## Step 2: Flash the firmware

Now, follow these steps to install Rad Pro on your FNIRSI GC-01 Geiger counter:

* Turn off the device.
* Connect the device to your computer using the USB cable.
* Power on the device. A USB disk should become available.
* Drag the Rad Pro firmware file to the USB disk. After the USB disk unmounts, you should be able to restart the device with Rad Pro installed.

If something goes wrong, you can recover your device with the [original firmware](devices/FNIRSI%20GC-01/firmware).

## Step 3: Configure your device

Last, start your device and go to Rad Pro's settings, select "Geiger tube", select "HV Profile" and choose an appropriate HV profile: "Energy-saving" reduces power consumption at the expense of lower accuracy, while "Accuracy" requires more power but provides more accurate measurements.

If the indicated radiation rate is not correct, turn off your device, open it, identify the installed Geiger tube, close your device, restart it and go to Rad Pro's settings, select "Geiger tube", "Conversion factor", and select the option that matches the Geiger-Müller tube installed in your device.

## Step 4: Getting involved

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

On the FNIRSI GC-01, Rad Pro can store up to 6072 data points. At normal radiation levels, this allows for 4 days of data at 1-minute intervals, 10 days at 5-minute intervals, 31 days at 10-minute intervals, 94 days at 30-minute intervals, and 189 days at 60-minute intervals.

The FNIRSI GC-01 HV profile settings are:

* Accuracy: 10 kHz frequency and 72 % duty cycle.
* Energy-saving: 2.5 kHz frequency and 10 % duty cycle.
