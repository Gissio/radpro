# Installing Rad Pro on the FS2011/YT-203B

## Supplies

To install Rad Pro on your FS2011 or YT-203B Geiger counter, you will need the following tools and components:

* An [ST-LINK V2 USB dongle (or clone)](https://www.amazon.com/s?k=st-link+v2)
* A 4-pin header
* A Philips screwdriver
* Optional: A soldering iron and solder
* If you use Windows, the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).

## Step 1: Open the device

![FS2011 circuit board types](img/fs2011-board-type.jpg)

Follow these steps to start installing Rad Pro:

* Turn off the device.
* Remove the battery cover and batteries.
* Remove the screws holding the back case and open the device.

The board should look like in the photos above (the tube may be different). If it does not, you may have a different hardware revision. In this case, Rad Pro may not function properly.

If your board looks different and Rad Pro does not work, create an issue on https://github.com/Gissio/radpro/issues.

## Step 2: Connect the programmer

![FS2011 connectors](img/fs2011-swd.jpg)

Now, follow these steps:

* Optional: Solder the 4-pin header to the XS1 pads of the board.
* If you use Windows, install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
* Connect the ST-Link V2 device to the XS1 pads. The pins, from top to bottom, are:
  * GND
  * SWCLK
  * SWDIO
  * +3.3V

Make sure the electrical connections are correct. You may break your device if the connections are not correct.

## Step 3: Flash the firmware

Now, download the latest `radpro-flashtool` from the [Rad Pro releases](https://github.com/Gissio/radpro/releases).

* Tp install from Windows, start the `flash-fs2011-[mcu].bat` script.
* To install from macOS or Linux, start the `flash-fs2011-[mcu].sh` script.

`[mcu]` is the type of microprocessor on your board: `stm32f051c8`, `gd32f150c8` or `gd32f103c8`.

The installation will automatically create a backup of the original firmware in the `backup` folder, so you can recover your device if something goes wrong. Make sure you keep the backups safe, so you can always return to the original firmware. To flash a backup, open a terminal and start the `flash-fs2011-[mcu]` script with the path to the backup file as argument.

After successfully flashing the firmware, close the device.

## Step 4: Configure your device

![FS2011 tube types](img/fs2011-tube-type.jpg)

Start your device and go to Rad Pro's settings, select “Geiger tube”, “Conversion factor”, and select the option that matches the Geiger-Müller tube installed in your device. The photos above should help you identify the tube.

Within the “Geiger tube” settings, select “HV Profile” and choose an appropriate HV profile: “Energy-saving” is best for measuring background levels of radiation, while “Accuracy” consumes more power but allows measurement of higher radiation levels; “Factory default” is the profile from the original firmware and consumes considerably more power.

Last, read the [user's manual](../../users.md) for learning how to use Rad Pro.

## Step 5: Optional mods

![FS2011 FTDI connection](img/fs2011-ftdi.jpg)

* Add USB connectivity. You'll need: an [3.3 V USB serial converter](https://www.amazon.com/s?k=ftdi+board) and [wire-wrap](https://www.amazon.com/s?k=wirewrap+30). Important: never use batteries when powering from USB. Follow these instructions:
  * Set up the serial converter for 3.3 V.
  * Connect the serial converter's GND and 5V pins to the FS2011 board's 0V and 5V pads, respectively.
  * Connect the PA2 pin of the microprocessor to the serial converter's RX pin.
  * Connect the PA3 pin of the microprocessor to the serial converter's TX pin.
* Add a vibrator (which you can recycle from an old cellphone). Solder it to the output of the Q5 transistor.
* For easier access to XS1, use a file to extend the hole for the battery holder clip so that a 4-pin header connector can fit. Solder four cables between XS1 and the 4-pin header. Using a glue gun, attach the header to the inside of the back case, so you can access the header with the battery case open.
* On some FS2011 devices, the tube is not aligned to the holes of the back case. For improved measurement, align the tube to the holes using a heat gun/glue gun. Be careful, as the tube's glass is very delicate.
* Some AA rechargeable batteries have low-profile caps that don't make electrical contact with the battery holder. Fix this problem by applying solder on the battery holder's pads.
* To increase the buzzer's volume, drill holes on the back case, in front of the buzzer.

## Step 6: Getting involved

If you like Rad Pro, consider watching the project to get notified when new releases are out. Also, show your support by starring the project on GitHub.

## Hardware-specific notes

The keys are mapped as follows:

  * Power on/off: long press of the power key.
  * Go up/down: the up and down keys.
  * Switch measurement view: the play/pause or power key.
  * Reset measurement: long press of the play/pause key.
  * Enter settings: the menu/ok key.
  * Select option: the menu/ok or power key.
  * Go back: the play/pause key.

On the FS2011, Rad Pro can store up to 27324 data points. At normal radiation levels (20 cpm), this allows for 569 days of data at 60-minute intervals, 284 days at 30-minute intervals, 94 days at 10-minute intervals, 47 days at 5-minute intervals, 18 days at 1-minute intervals, 9 days at 30-second intervals and 3 days at 10-second intervals.

The FS2011 includes two Zener diodes that limit the maximum voltage to 440 V (nominal).

The FS2011 HV profile settings are:

* Factory default: 40 kHz frequency and 50% duty cycle.
* Accuracy: 2.5 kHz frequency and 6.5% duty cycle.
* Energy-saving: 2.5 kHz frequency and 3.0% duty cycle.
