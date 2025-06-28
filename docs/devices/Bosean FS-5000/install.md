# Installing Rad Pro on Bosean FS-5000 Geiger counters

This guide explains how to install the Rad Pro firmware on Bosean FS-5000 Geiger counters.

## Required supplies

To install Rad Pro, you'll need:

* [ST-LINK V2 USB dongle (or compatible clone)](https://www.amazon.com/s?k=st-link+v2)
* 4-pin header
* Philips screwdriver
* Optional: Soldering iron and solder
* For Windows users: [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html)

## Step 1: Open the device

![Bosean FS-5000 circuit board](img/fs5000-board.jpg)

1. Power off the device.
2. Unscrew the back case and carefully open the device.
3. Verify that the circuit board matches the image above (the Geiger-Müller tube may vary). If the board differs, you may have a different hardware revision, and Rad Pro may not function correctly. If issues arise, report them on the [Rad Pro GitHub issues page](https://github.com/Gissio/radpro/issues).
4. Note the Geiger-Müller tube type (marked on the glass as J614, J321, J305 or M4011). For unmarked tubes measuring 55 or 65 mm, assume J614 or J613, respectively.

## Step 2: Connect the programmer

![Bosean FS-5000 connectors](img/fs5000-swd.jpg)

1. Optional: Solder a 4-pin header to the SWD pads on the board for a secure connection.
2. Connect the ST-LINK V2 to the SWD pads using the following pin configuration (top to bottom):
  * 3.3V
  * SWDIO
  * SWCLK
  * GND

![ST-LINK V2 programmer](../../img/ST-LINK-V2.png)

**WARNING:** Double-check electrical connections to prevent damage to the device.

## Step 3: Flash the firmware

1. For Windows users, install the [ST-LINK driver](https://www.st.com/en/development-tools/stsw-link009.html).
2. Download and extract the latest `radpro-[version].zip` from [Rad Pro releases](https://github.com/Gissio/radpro/releases).
3. Navigate to the `bosean` folder and run the appropriate script based on your operating system:
  * Windows: Double click `bosean-fs5000-[type].bat`.
  * Linux: Run `bosean-fs5000-[type].sh` in a terminal.
  * macOS: Open `Terminal.app` (in `/Applications/Utilities`), navigate to the `bosean` folder, and drag `bosean-fs5000-[type].sh` onto the Terminal icon in the dock.
  * `[type]`: Installation type (`install` or `update`). Note: Not all releases support updating.
4. The installer will prompt you to select a language using two-letter language codes. Enter the desired code to proceed.
5. The installer automatically backs up the original firmware to the `backup` folder. Store this securely to restore the original firmware if needed. To restore, drag the backup file onto `bosean-fs5000-install.bat` (Windows) or `bosean-fs5000-install.sh` (macOS/Linux).
6. Close and reassemble the device.

<!-- Note: check this [video](https://youtu.be/Ney8Cb1XnZk) for alternative installation instructions. -->

## Step 4: Configure the device

Use the following controls to operate your device:

  * **Power on/off:** Press and long hold the OK/Power key.
  * **Switch measurement mode:** Use the Up/Back or Down/Settings key.
  * **Switch secondary measurement view:** Press the OK/Power key.
  * **Reset measurement/dismiss alarm:** Press and long hold the Up/Back key.
  * **Toggle pulse sound (measurement view only):** Press and long hold both the Up/Back and Down/Settings key.
  * **Access settings:** Press and short hold the Down/Settings key.
  * **Navigate options:** Use the Up/Back or Down/Settings key.
  * **Select option:** Press and short hold the Right/Settings key or press the OK/Power key.
  * **Go back:** Press and short hold the Up/Back key.
  * **Toggle lock mode:** Press and long hold both the OK/Power and Up/Back keys.

To configure the device:

1. Go to **Settings > Geiger tube > Sensitivity** and select the option matching your Geiger-Müller tube.
2. For USB data connections under Windows, install the [CH340 driver](https://www.catalog.update.microsoft.com/Search.aspx?q=USB%5CVID_1A86%26PID_7523).

For detailed usage instructions, refer to the [Rad Pro User Manual](../../manual.md) and the [ionizing radiation field guide](../../field-guide/field-guide.md).

## Step 5: Final steps

If you find Rad Pro useful, consider:

* Watching the [Rad Pro GitHub repository](https://github.com/Gissio/radpro) for release notifications.
* Starring the project to show your support.

## Hardware-specific notes

* **USB data connection:** Use a USB data cable to connect the FS-5000 to a computer.

<!-- Calculated as follows:

* With 1-byte differential values: [67 pages * (1 timestamp entry/page [16 bytes] + 2024 differential entries/page [1 byte each]) + page full indicator (2 bytes)] = 196425 entries
* With 2-byte differential values: [67 pages * (1 timestamp entry/page [16 bytes] + 1012 differential entries/page [2 byte each])] = 98261 entries

* 60-minute and 10-minute intervals require 2-byte differential values.
* 1-minute intervals and less require 1-byte differential values.

 -->

* **Data storage:** Stores up to 208,575 data points. At 20 cpm (normal radiation levels), this supports:
  * 4094 days at 60-minute intervals
  * 682 days at 10-minute intervals
  * 136 days at 1-minute intervals
  * 22 days at 10-second intervals
  * 54 hours at 1-second intervals

* **HV profiles:** Not supported.
