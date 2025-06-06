# Installing Rad Pro on the Bosean FS-600/FS-1000

## Supplies

To install Rad Pro on your Bosean FS-600 or FS-1000 Geiger counter, you will need the following tools and components:

* An [ST-LINK V2 USB dongle (or clone)](https://www.amazon.com/s?k=st-link+v2)
* A 4-pin header
* A Philips screwdriver
* Optional: A soldering iron and solder
* If you use Windows, the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html)

## Step 1: Open the device

![Bosean FS-600 circuit board](img/fs600-board.jpg)

Follow these steps to start installing Rad Pro:

* Turn off the device.
* Remove the battery cover and batteries.
* Remove the screws holding the back case and open the device.

The board should look like in the photo above. If it does not, you may have a different hardware revision. In this case, Rad Pro may not function properly.

If your board looks different and Rad Pro does not work, create an [issue](https://github.com/Gissio/radpro/issues).

## Step 2: Connect the programmer

![Bosean FS-600 connectors](img/fs600-swd.jpg)

Now, follow these steps:

* Optional: Solder the 4-pin header to the SWD pads of the board.
* If you use Windows, install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
* Connect the ST-LINK V2 device to the SWD pads. The pins, from left to right, are:
  * 3.3V
  * SWDIO
  * SWCLK
  * GND

![ST-LINK V2 programmer](../../img/ST-LINK-V2.png)

Make sure the electrical connections are correct. You may break your device if the connections are not correct.

## Step 3: Flash the firmware

Now, download and unzip the latest `radpro-[version].zip` from the [Rad Pro releases](https://github.com/Gissio/radpro/releases).

* To install from Windows, go to the `bosean` folder and double click `bosean-[device]-[type].bat`.
* To install from Linux, go to the `bosean` folder and double click `bosean-[device]-[type].sh`.
* To install from macOS, open `Terminal.app` (in `/Applications/Utilities`), go to the `bosean` folder and drag the `bosean-[device]-[type].sh` install file onto the Terminal icon in the dock.

`[device]` is the type of your device: `fs600` or `fs1000`.

`[type]` is the type of installation: `install` or `update`. Note that not all releases support updating.

The installer will display a list of available languages as two-letter language codes. Select and enter the language code to proceed with the installation.

The installer automatically creates a backup of the original firmware in the `backup` folder, ensuring you can restore your device if needed. Store these backups securely to enable reversion to the original firmware. To restore a backup, drag it onto `bosean-[device]-install.bat` (Windows) or `bosean-[device]-install.sh` (macOS/Linux).

## Step 4: Configure your device

Start your device and go to Rad Pro's settings, select "Geiger tube", "HV Profile" and choose an appropriate HV profile: "Energy-saving" is best for measuring background levels of radiation, while "Accuracy" consumes more power but allows measurement of higher levels of radiation; "Factory default" is the profile from the original firmware and consumes considerably more power.

To establish a USB data connection under Windows, install the [CH340 driver](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523).

Last, read the [user's manual](../../manual.md) for learning how to use Rad Pro, and the [ionizing radiation field guide](../../field-guide/field-guide.md) for learning more about ionizing radiation.

## Step 5: Last steps

If you like Rad Pro, consider watching the project to get notified when new releases are out. Also, show your support by starring the project on GitHub. And don't forget to read the keyboard mapping table below!

## Hardware-specific notes

On the FS-600, the keys are mapped as follows:

  * Power on/off: long hold of the ok/power key.
  * Switch measurement mode: the up and down keys.
  * Switch measurement secondary view: the left/back.
  * Reset measurement/dismiss alarm: long hold of the left/back key.
  * Put display to sleep (measurement view only): the ok/power key.
  * Enter settings: the right/settings key.
  * Go up/down: the up and down keys.
  * Select option: the ok/power or right/settings key.
  * Go back: the left/back key.
  * Toggle pulses sound: long hold of the menu/ok key.
  * Toggle lock mode: long hold of the ok/power and left/back keys.

On the FS-1000, the keys are mapped as follows:

  * Power on/off: long hold of the set/power key.
  * Switch measurement mode: the mode key.
  * Switch measurement secondary view: the set/power key.
  * Reset measurement/dismiss alarm: long hold of the mode key.
  * Enter settings: short hold of the set/power key.
  * Go up/down: the mode and set/power key.
  * Select option: short hold of the set/power key.
  * Go back: short hold of the mode key.
  * Toggle lock mode: long hold of the set/power and mode keys.

Not all devices support the pulse LED.

To establish a USB data connection, connect your FS-600 or FS-1000 to a computer using a USB data cable.

<!-- Calculated as follows:

* With 1-byte differential values: [43 pages * (1 timestamp entry/page [16 bytes] + 2024 differential entries/page [1 byte each])] = 87075 entries
* With 2-byte differential values: [43 pages * (1 timestamp entry/page [16 bytes] + 1012 differential entries/page [2 byte each])] = 43559 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

On the FS-600 and FS-1000, Rad Pro can store up to 89100 data points. At normal radiation levels (20 cpm), this allows for 1814 days of data at 60-minute intervals, 302 days at 10-minute intervals, 60 days at 1-minute intervals, 10 days at 10-second intervals and 24 hours at 1-second intervals.

The FS-600 and FS-1000 include two Zener diodes that limit the maximum voltage to 440 V (nominal).

The FS-600 and FS-1000 HV profile settings are:

* Factory default: 40 kHz frequency and 50% duty cycle.
* Accuracy: 2.5 kHz frequency and 6.5% duty cycle.
* Energy-saving: 2.5 kHz frequency and 3.0% duty cycle.
