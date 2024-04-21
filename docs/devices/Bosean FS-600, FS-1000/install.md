# Installing Rad Pro on the Bosean FS-600/FS-1000

## Supplies

To install Rad Pro on your Bosean FS-600 or FS-1000 Geiger counter, you will need the following tools and components:

* An [ST-Link V2 USB dongle (or clone)](https://www.amazon.com/s?k=st-link+v2)
* A 4-pin header
* A Philips screwdriver
* Optional: A soldering iron and solder
* If you use Windows, the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).

## Step 1: Open the device

![Bosean FS-600 circuit board](img/fs600-board.jpg)

Follow these steps to start installing Rad Pro:

* Turn off the device.
* Remove the battery cover and batteries.
* Remove the screws holding the back case.
* Disconnect the battery connected to BAT1.
* Remove the screws holding the electronics board to the front case.
* Remove the electronics board.

The board should look like in the photo above. If it does not, you may have a different hardware revision. In this case, Rad Pro may not function properly.

If your board looks different and Rad Pro does not work, create an issue on https://github.com/Gissio/radpro/issues.

## Step 2: Connect the programmer

![Bosean FS-600 connectors](img/fs600-swd.jpg)

Now, follow these steps:

* Optional: Solder the 4-pin header to the XS1 pads of the board.
* If you use Windows, install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
* Connect the ST-Link V2 device to SWD. The pins, from left to right, are:
  * +3.3V
  * SWDIO
  * SWCLK
  * GND

Make sure the electrical connections are correct. You may break your device if the connections are not correct.

## Step 3: Flash the firmware

Now, download the latest `radpro-flashtool` from the [Rad Pro releases](https://github.com/Gissio/radpro/releases).

* To install from Windows, start the `install-bosean-[device].bat` script.
* To install from macOS or Linux, start the `install-bosean-[device].sh` script.

`[device]` is your device type: `fs600` or `fs1000`.

The installation will automatically create a backup of the original firmware in the `backup` folder, so you can recover your device if something goes wrong. Make sure you keep the backups safe, so you can always return to the original firmware. To flash a backup, open a terminal and start the `install-bosean-[device]` script with the backup file as argument.

To enable USB data communications under Windows, install the [CH340 driver](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523).

## Step 4: Configure your device

Start your device and go to Rad Pro's settings, select "Geiger tube", "HV Profile" and choose an appropriate HV profile: "Energy-saving" is best for measuring background levels of radiation, while "Accuracy" consumes more power but allows measurement of higher radiation levels; "Factory default" is the profile from the original firmware and consumes considerably more power.

Last, read the [user's manual](../../users.md) for learning how to use Rad Pro.

## Step 5: Getting involved

If you like Rad Pro, consider watching the project to get notified when new releases are out. Also, show your support by starring the project on GitHub.

## Hardware-specific notes

On the FS-600, the keys are mapped as follows:

  * Power on/off: long press of the ok/power key.
  * Go up/down: the up and down keys.
  * Toggle mesurement view: the ok/power or left/back key.
  * Reset measurement: long press of the left/back key.
  * Enter settings: the right/settings key.
  * Select option: the ok/power or right/settings key.
  * Go back: the left/back key.

On the FS-1000, the keys are mapped as follows:

  * Power on/off: extended press of the set/power key.
  * Go up: the mode key.
  * Go down: the set/power key.
  * Enter settings/select option: long press of the set/power key.
  * Toggle measurement view/go back: long press of the mode key.
  * Reset measurement: long press of both the mode and set/power keys.

Not all devices support the pulse LED.

On the FS-600 and FS-1000, Rad Pro can store up to 91080 data points. At normal radiation levels (20 cpm), this allows for 1897 days of data at 60-minute intervals, 948 days at 30-minute intervals, 316 days at 10-minute intervals, 158 days at 5-minute intervals, 63 days at 1-minute intervals, 31 days at 30-second intervals and 10 days at 10-second intervals.

The FS-600 and FS-1000 include two Zener diodes that limit the maximum voltage to 440 V (nominal).

The FS-600 and FS-1000 HV profile settings are:

* Factory default: 40 kHz frequency and 50 % duty cycle.
* Accuracy: 2.5 kHz frequency and 6.5 % duty cycle.
* Energy-saving: 2.5 kHz frequency and 3.0 % duty cycle.
